#include "pch.h"
#include "AlchemicalMachineObject.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define AM_RAGE SimpleMath::Vector3(1, 1, 1)

// 修理にかかる魔力
#define REPAIR_HP 30 * m_lv

AlchemicalMachineObject::AlchemicalMachineObject():
	m_hp(1),
	m_maxHp(1),
	m_active(),
	m_hitMouseFlag(),
	m_selectModeFlag(),
	m_objectName(),
	m_magicCircle(),
	m_machineID(MACHINE_TYPE::NONE),
	m_lv(1),
	m_machineEffectValue(),
	m_span(),
	m_color(1,1,1,1),
	m_rotateAnimation(),
	m_element(ELEMENT::NOMAL),
	m_powerUPFlag(),
	m_line(),
	m_dismantlingFlag()
{
	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(170, 490),
		SimpleMath::Vector2(1.0f, 1.0f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(250, 490),
		SimpleMath::Vector2(1.0f, 1.0f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(250, 650),
		SimpleMath::Vector2(1.0f, 1.0f));

}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;

	DataManager& pDataM = *DataManager::GetInstance();
	//auto pSJD = &ShareJsonData::GetInstance();

	// LvUp用の選択ボックスの設定
	m_selectLvUpBox->HitMouse();

	if (m_selectLvUpBox->ClickMouse()) 		LvUp();

	// 修繕用の選択ボックスの設定
	m_repairBox->HitMouse();

	// 修繕選択ボックスを押す　現在のCrystal量から修繕に掛かるCrystal量が0以上ならば実行
	if (m_repairBox->ClickMouse() && pDataM.GetNowCrystal() - GetRepairCrystal() >= 0)
	{
		m_hp = m_maxHp;
		pDataM.SetNowMP(pDataM.GetNowCrystal() - GetRepairCrystal());
	}

	// 破壊用の選択ボックス
	m_dismantlingBox->HitMouse();

	// 破壊選択ボックスを押す　現在のCrystal量から増加するCrystal量が最大値以下ならば実行
	if (m_dismantlingBox->ClickMouse() && pDataM.GetNowCrystal() + GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX())
	{
		m_dismantlingFlag = true;

		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + GetDismantlingCrystal());
	}

}

void AlchemicalMachineObject::SelectRenderUI_Common()
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	// LVUP用UI
	RECT rect_UI = SpriteCutter(64, 64, m_lv, 0);
	m_selectLvUpBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetNumberTexture(), rect_UI);

	// 修繕用UI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::REPAIR, 0);
	m_repairBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
						SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));

	// 解体用UI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::DISMATIONG, 0);
	m_dismantlingBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
							 SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

}

void AlchemicalMachineObject::HitToObject(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	//InputSupport& pINP = InputSupport::GetInstance();
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
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	// ディフェンサー型は常に拠点の方向を向く
	if (m_machineID == DEFENSER)
	{
		m_rotateAnimation = 0.0f;
		modelData *= SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180.0f));
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(LookAt({ 0.0f,m_data.pos.y,0.0f }));
	}

	// 常に右回りに回転
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	// 常に縦に揺れる
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f), m_data.pos.z);

	// 追加パーツが存在する場合
	if (ring != nullptr)
	{
		SimpleMath::Matrix ringData = SimpleMath::Matrix::Identity;
		ringData = SimpleMath::Matrix::CreateScale(m_data.rage);
		ringData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
		ringData *= SimpleMath::Matrix::CreateTranslation
		(m_data.pos.x,
			m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f),
			m_data.pos.z);

		// エフェクトの設定
		ring->UpdateEffects([&](IEffect* effect)
			{
				// 今回はライトだけ欲しい
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// 色変更
				lights->SetLightDiffuseColor(0, SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f));
			});

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// 深度ステンシルステートの設定
				pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilShadow().Get(), 1);
				pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
			});

		pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
		pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// 深度ステンシルステートの設定
				pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
			});

	}

	// エフェクトの設定
	model->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, GetColor());
		});

	{
		// 重なった際、影を描画
		model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// 深度ステンシルステートの設定
				pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilShadow().Get(), 1);
				pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
			});

		pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
		pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

		model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// 深度ステンシルステートの設定
				pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
			});

	}
}

void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
}

const int AlchemicalMachineObject::GetNextLvCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).lvUp_crystal;
}

const int AlchemicalMachineObject::GetRepairCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).repea_crystal;
}

const int AlchemicalMachineObject::GetDismantlingCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).dismantling_crystal;
}
