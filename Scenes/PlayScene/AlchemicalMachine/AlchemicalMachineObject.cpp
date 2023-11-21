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

// HPゲージの大きさ
#define HPGAUGE_RAGE 0.7f

// マシンの名前の大きさ
#define MACHINENAME_RAGE 2.35f

// Lv表示の大きさ
#define MACHINELV_RAGE 2.0f

// マシンのアイコンの大きさ
#define MACHINEICON_RAGE 0.8f

// 魔法陣の大きさ
#define MAGICCIRCLE_RAGE 0.2f

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
	m_spawnTime(1.0f),
	m_difRedioHp(),
	m_popHPTimer()
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
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 無敵時間計測
	if (m_invincibleFlag)
	{
		m_invincibleTime += deltaTime;

		// 既定の時間に達したらフラグをFalseにして無敵状態を解除する
		if (m_invincibleTime >= 1.0f)
		{
			m_invincibleTime = 0.0f;
			m_invincibleFlag = false;
		}
	}

	// 出現時の演出に使うタイマー
	m_spawnTime += deltaTime;
	// 回転に使用する値
	m_rotateAnimation += deltaTime;

	float radio_Hp = (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()));

	// 徐々に減るHPの処理
	m_difRedioHp -= 0.3f * deltaTime;
	m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

	// 触れたら出現するHPバーUI用の時間変数を更新する
	m_popHPTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();

	// マウスに触れているor選択がされているならばタイマーを加算する
	if (m_hitMouseFlag || m_selectModeFlag || m_hp <= 0)
	{
		m_popHPTimer += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;
	}

	m_popHPTimer = std::min(std::max(0.0f, m_popHPTimer), 1.0f);

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
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - GetRepairCrystal());
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
	m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

	// 修繕用UI
	m_repairBox->DrawUI(SpriteLoder::REPAIR);

	// 解体用UI
	m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);

}

void AlchemicalMachineObject::RenderHP()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	// 徐々に上方向へ出現させる変数
	float easingValCirc  = Easing::EaseInOutCirc(0.0f, 1.0f, m_popHPTimer);
	SimpleMath::Vector2 billboardUIPosition = SimpleMath::Vector2(GetPos().x, GetPos().y - easingValCirc * 50.0f);

	// 魔法陣を回転させる変数
	float easingValRotate = Easing::EaseOutQuint(0.0f, XMConvertToRadians(360.0f), m_popHPTimer);

	SimpleMath::Color color(1.0f , 1.0f, 1.0f, easingValCirc);

	// マシンのHPゲージ(外枠)========
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetBaseGage();
	RECT rect = { 0,0,texData.width,texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	// マシンアイコンの描画位置の設定に使用
	float gaugeWidthHalf = texData.width / 2;

	// マシンのHPゲージ(徐々に減少)===========
	texData = SpriteLoder::GetInstance().GetMainGage();
	rect = { 0,0,static_cast<LONG>(texData.width * m_difRedioHp),texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::Red * color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	// マシンのHPゲージ(一気に減少)===========
	rect = { 0,0,static_cast<LONG>(texData.width * (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()))),texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::HotPink * color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	//　マシンの名前を描画========
	texData = SpriteLoder::GetInstance().GetMachineNameTexture();
	rect = SpriteCutter(texData.width / MACHINE_TYPE::NUM,texData.height,(int)m_machineID,0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2((texData.width / MACHINE_TYPE::NUM) / 2, texData.height / 2),
		MACHINENAME_RAGE);

	// マシンの魔法陣を描画========
	texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture((int)m_machineID);
	rect = { 0,0, texData.width,texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.7f) * color,
		easingValRotate,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		MAGICCIRCLE_RAGE);

	//　マシンのアイコンを描画========
	texData = SpriteLoder::GetInstance().GetMachineIconTexture();
	rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, (int)m_machineID - 1, 0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f,1.0f,1.0f,0.6f) * color,
		0.0f,
		SimpleMath::Vector2(texData.width / (MACHINE_TYPE::NUM - 1 ) / 2, texData.height / 2),
		MACHINEICON_RAGE);

	//　マシンのレベルを描画=======
	rect = SpriteCutter(64, 64, m_lv , 0);
	pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Black * color,
		0.0f,
		SimpleMath::Vector2(64 / 2, 64 / 2),
		MACHINELV_RAGE);

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
