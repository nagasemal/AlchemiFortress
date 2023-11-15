#include "pch.h"
#include "AlchemicalMachineObject.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define AM_RAGE SimpleMath::Vector3(1, 1, 1)

// 修理にかかる魔力
#define REPAIR_HP 30 * m_lv

AlchemicalMachineObject::AlchemicalMachineObject() :
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
	m_color(1, 1, 1, 1),
	m_subColor(1, 1, 1, 1),
	m_rotateAnimation(),
	m_element(ELEMENT::NOMAL),
	m_powerUPFlag(),
	m_line(),
	m_dismantlingFlag(),
	m_invincibleTime(),
	m_invincibleFlag(),
	m_spawnTime(1.0f)
{
	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(150, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(210, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(270, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_color = Json::ChangeColor(m_element);

}

void AlchemicalMachineObject::Update_Common()
{
	// 無敵時間計測
	if (m_invincibleFlag)
	{
		m_invincibleTime += DeltaTime::GetInstance().GetDeltaTime();

		// 既定の時間に達したらフラグをFalseにして無敵状態を解除する
		if (m_invincibleTime >= 1.0f)
		{
			m_invincibleTime = 0.0f;
			m_invincibleFlag = false;
		}
	}

	// 出現時の演出に使うタイマー
	m_spawnTime += DeltaTime::GetInstance().GetDeltaTime();
	// 回転に使用する値
	m_rotateAnimation += DeltaTime::GetInstance().GetDeltaTime();
}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;

	DataManager& pDataM = *DataManager::GetInstance();
	//auto pSJD = &ShareJsonData::GetInstance();

	// LvUp用の選択ボックスの設定
	m_selectLvUpBox->HitMouse();

	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	m_selectLvUpBox->SetActiveFlag(m_lv <= MAX_LV && pDM.GetNowCrystal() - GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse()) 		LvUp();

	// 修繕用の選択ボックスの設定
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - GetRepairCrystal() >= 0 && m_hp < m_maxHp);

	// 修繕選択ボックスを押す　現在のCrystal量から修繕に掛かるCrystal量が0以上ならば実行
	if (m_repairBox->ClickMouse())
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
	m_selectLvUpBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetNumberTexture(), rect_UI,SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));

	// 修繕用UI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::REPAIR, 0);
	m_repairBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
						SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));

	// 解体用UI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::DISMATIONG, 0);
	m_dismantlingBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
							 SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

}

void AlchemicalMachineObject::HitToMouse(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	//InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// オブジェクトとマウスポインターの当たり判定
	if (CircleCollider(GetCircle(), mouseWolrdPos))
	{
		// マシンに当たったことを知らせる
		pMP->HitMachine(m_data.pos);
		m_hitMouseFlag = true;
	}

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model, DirectX::Model* ring, bool silhouette)
{
	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	// ディフェンサー型は常に拠点の方向を向く
	if (m_machineID == DEFENSER)
	{
		// 拠点との距離
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		m_rotateAnimation = 0.0f;
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	// 常に右回りに回転
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	// 常に縦に揺れる
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.2f), m_data.pos.z);

	SimpleMath::Matrix ringData = SimpleMath::Matrix::Identity;
	ringData = SimpleMath::Matrix::CreateScale(m_data.rage);
	ringData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
	ringData *= SimpleMath::Matrix::CreateTranslation
	(m_data.pos.x,
		m_data.pos.y + (sinf(m_rotateAnimation) * 0.2f),
		m_data.pos.z);

	// 追加パーツが存在する場合
	if (m_machineID == ATTACKER)
	{
		m_subColor = SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f);

		// エフェクトの設定
		ring->UpdateEffects([&](IEffect* effect)
			{
				// 今回はライトだけ欲しい
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// 色変更
				lights->SetLightDiffuseColor(0,m_subColor);
				lights->SetLightDiffuseColor(1,m_subColor);
				lights->SetLightDiffuseColor(2,m_subColor);

			});

	}

	// エフェクトの設定
	model->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 色変更
			lights->SetLightDiffuseColor(0, GetColor());
			lights->SetLightDiffuseColor(1, GetColor());
			lights->SetLightDiffuseColor(2, GetColor());

		});

	//// 重なった際、影を描画
	//model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
	//	{
	//		//画像用サンプラーの登録
	//		ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap()};
	//		pSD.GetContext()->PSSetSamplers(0, 1, sampler);
	//		//半透明描画指定
	//		ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	//		// 透明判定処理
	//		pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	//
	//		// 深度ステンシルステートの設定
	//		pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilShadow().Get(), 1);
	//		
	//		// カリングは左周り
	//		pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());
	//
	//		pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
	//	});

	// シルエット描画
	if (silhouette)
	{
		SilhouetteRender(model, modelData);
		// シェーダーの適応
		if (ring != nullptr) 		SilhouetteRender(ring, ringData);

	}
	// 通常描画
	else
	{
		NomalRender(model, modelData, m_color);
		// 通常描画
		if (ring != nullptr) 		NomalRender(ring, ringData, m_subColor);
	}

	// シェーダーの解除
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

}

// 召喚時に呼ばれる関数
void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
	m_spawnTime = 0.0f;
}

bool AlchemicalMachineObject::GetRepairFlag()
{
	return m_repairBox->ClickMouse();
}

bool AlchemicalMachineObject::GetLvUpFlag()
{
	return m_selectLvUpBox->ClickMouse();
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

SelectionBox* AlchemicalMachineObject::GetMenuButton(int buttonType)
{
	switch (buttonType)
	{
	case 0:
		return m_repairBox.get();
	case 1:
		return m_dismantlingBox.get();
	case 2:
		return m_selectLvUpBox.get();
	}

	// それ以外の場合はとりあえず修繕選択Boxを選択する
	return m_repairBox.get();

}

void AlchemicalMachineObject::SilhouetteRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	// 出現時演出が終わるまで処理をしない
	if (m_spawnTime <= 1.0f) return;

	ShareData& pSD = ShareData::GetInstance();

	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

}

void AlchemicalMachineObject::NomalRender(DirectX::Model* model, SimpleMath::Matrix matrix, SimpleMath::Color color)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().ModelDrawShader(color,SimpleMath::Vector4(m_spawnTime, m_span, m_invincibleTime,1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(m_element).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(m_element).GetAddressOf());

		});

	//シェーダの登録を解除しておく
	pSD.GetContext()->VSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);

}

void AlchemicalMachineObject::TransparentRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{

	ShareData& pSD = ShareData::GetInstance();

	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().ToransparentShader();


		});

}

void AlchemicalMachineObject::NotShaderRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			//半透明描画指定
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// 透明判定処理
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		});
}
