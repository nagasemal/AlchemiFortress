#include "pch.h"
#include "AlchemicalMachineObject.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"


#define AM_RAGE DirectX::SimpleMath::Vector3(1, 1, 1)

// 修理にかかる魔力
#define REPAIR_HP 30 * m_lv

AlchemicalMachineObject::AlchemicalMachineObject():
	m_hp(1),
	m_maxHp(),
	m_active(),
	m_hitMouseFlag(),
	m_selectModeFlag(),
	m_objectName(),
	m_magicCircle(),
	m_machineID(MACHINE_TYPE::NONE),
	m_lv(1),
	m_machineEffectNum(),
	m_span(),
	m_color(1,1,1,1),
	m_rotateAnimation(),
	m_element(MACHINE_ELEMENT::NOMAL),
	m_powerUPFlag(),
	m_line()
{
	m_selectLvUpBox = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(170, 490),
		DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	m_repairBox = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(250, 490),
		DirectX::SimpleMath::Vector2(1.0f, 1.0f));
}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	DataManager& pDataM = *DataManager::GetInstance();

	// LvUp用の選択ボックスの設定
	m_selectLvUpBox->HitMouse();

	if (m_selectLvUpBox->ClickMouse())
	{
		LvUp();
	}

	// 修繕用の選択ボックスの設定
	m_repairBox->HitMouse();

	if (m_repairBox->ClickMouse() && pDataM.GetNowMP() - REPAIR_HP >= 0)
	{
		m_hp = m_maxHp;
		pDataM.SetNowMP(pDataM.GetNowMP() - REPAIR_HP);
	}

}

void AlchemicalMachineObject::HitToObject(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// オブジェクトとマウスポインターの当たり判定
	if (CircleCollider(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model, DirectX::Model* ring)
{
	ShareData& pSD = ShareData::GetInstance();

	m_rotateAnimation += DeltaTime::GetInstance().GetDeltaTime();

	// モデル情報(位置,大きさ)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);

	// ディフェンサー型は常に拠点の方向を向く
	if (m_machineID == DEFENSER)
	{
		m_rotateAnimation = 0.0f;
		modelData *= DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180.0f));
		modelData *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(LookAt({ 0.0f,m_data.pos.y,0.0f }));
	}

	// 常に右回りに回転
	modelData *= DirectX::SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	// 常に縦に揺れる
	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f), m_data.pos.z);

	// エフェクトの設定
	model->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 色変更
			lights->SetLightDiffuseColor(0, GetColor());
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	// 追加パーツが存在する場合
	if (ring != nullptr)
	{
		DirectX::SimpleMath::Matrix ringData = DirectX::SimpleMath::Matrix::Identity;
		ringData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);
		ringData *= DirectX::SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
		ringData *= DirectX::SimpleMath::Matrix::CreateTranslation
		(m_data.pos.x,
		 m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f),
		 m_data.pos.z);

		// エフェクトの設定
		ring->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0,DirectX::SimpleMath::Color((float)m_powerUPFlag,(float)m_powerUPFlag,0.0f,1.0f));
		});

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection());

	}

}

void AlchemicalMachineObject::SummonAM(DirectX::SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
}
