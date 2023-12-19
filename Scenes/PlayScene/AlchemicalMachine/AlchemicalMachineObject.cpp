#include "pch.h"
#include "AlchemicalMachineObject.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define AM_RAGE				SimpleMath::Vector3(1, 1, 1)

// 修理にかかる魔力
#define REPAIR_HP			30 * m_lv

// 縦揺れ値
#define PITCHING_VAL		0.2f

// サブモデルのY軸回転速度
#define SUBMODEL_SPEED		1.5f

// 数字画像の縦横大きさ
#define SPRITE_RAGE			64.0f

//　====================[　3DUIにて使用する数値の定義　]

//　　|=> HPゲージの大きさ
#define HPGAUGE_RAGE		0.7f

//　　|=> マシンの名前の大きさ
#define MACHINENAME_RAGE	2.35f

//　　|=> Lv表示の大きさ
#define MACHINELV_RAGE		2.0f

//　　|=> マシンのアイコンの大きさ
#define MACHINEICON_RAGE	0.8f

//　　|=> 魔法陣の大きさ
#define MAGICCIRCLE_RAGE	0.2f

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
	m_spawnTime(0.0f),
	m_difRedioHp(),
	m_popHPTimer(),
	m_selectModeTime()
{
	m_color = Json::ChangeColor(m_element);

}

void AlchemicalMachineObject::Update_Common()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	//　====================[　無敵時間計測　]
	if (m_invincibleFlag)
	{
		m_invincibleTime += deltaTime;

		// 既定の時間に達する
		if (m_invincibleTime >= 1.0f)
		{
			//　　|=>　無敵時間リセット
			m_invincibleTime = 0.0f;
			//　　|=>　無敵時間フラグリセット
			m_invincibleFlag = false;
		}
	}

	//　====================[　演出用時間変数　]
	//　　|=>　設置時
	m_spawnTime += deltaTime;
	//　　|=>　マシンのY軸回転
	m_rotateAnimation += deltaTime;
	//　　|=>　HPバー出現
	m_popHPTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();

	//　====================[　HPの割合計算　]
	float radio_Hp = (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()));

	//　====================[　徐々に減るHPの処理　]
	m_difRedioHp -= 0.3f * deltaTime;
	m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

	// マウスに触れているor選択がされているならばタイマーを加算する
	if (m_hitMouseFlag || m_selectModeFlag || m_hp <= 0)
	{
		m_popHPTimer += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;
	}

	m_popHPTimer = std::min(std::max(0.0f, m_popHPTimer), 1.0f);


	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//　====================[　効果範囲位置/範囲の更新　]
	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage + (float)(m_lv / pSJD.GetMachineData(m_machineID).effect_rage_rate);

}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;
	m_selectModeTime += DeltaTime::GetInstance().GetDeltaTime();

}

void AlchemicalMachineObject::SelectRenderUI_Common()
{

}

void AlchemicalMachineObject::RenderHP()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	//　====================[　徐々に上方向へ出現させる変数　]
	float easingValCirc  = Easing::EaseInOutCirc(0.0f, 1.0f, m_popHPTimer);
	SimpleMath::Vector2 billboardUIPosition = SimpleMath::Vector2(GetPos().x, GetPos().y - easingValCirc * 50.0f);

	//　====================[　魔法陣を回転させる変数　]
	float easingValRotate = Easing::EaseOutQuint(0.0f, XMConvertToRadians(360.0f), m_popHPTimer);

	//　====================[　徐々に透明度を下げる変数　]
	SimpleMath::Color color(1.0f , 1.0f, 1.0f, easingValCirc);


	//　====================[　マシンのHPゲージ(外枠)　]
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetBaseGage();
	RECT rect = { 0,0,texData.width,texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);

	// マシンアイコンの描画位置の設定に使用
	float gaugeWidthHalf = static_cast<float>(texData.width / 2);


	//　====================[　マシンのHPゲージ(徐々に減少)　]
	texData = SpriteLoder::GetInstance().GetMainGage();
	rect = { 0,0,static_cast<LONG>(texData.width * m_difRedioHp),texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::Red * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
			HPGAUGE_RAGE);


	//　====================[　マシンのHPゲージ(一気に減少)　]
	rect = { 0,0,static_cast<LONG>(texData.width * (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()))),texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::HotPink * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);


	//　====================[　マシンの名前描画　]
	texData = SpriteLoder::GetInstance().GetMachineNameTexture();
	rect = SpriteCutter(texData.width / MACHINE_TYPE::NUM,texData.height,(int)m_machineID,0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>((texData.width / MACHINE_TYPE::NUM) / 2),
							static_cast<float>(texData.height / 2)),
		MACHINENAME_RAGE);


	//　====================[　マシンの魔法陣描画　]
	texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture((int)m_machineID);
	rect = { 0,0, texData.width,texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.7f) * color,
		easingValRotate,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		MAGICCIRCLE_RAGE);


	//　====================[　マシンのアイコン描画　]
	texData = SpriteLoder::GetInstance().GetMachineIconTexture();
	rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, (int)m_machineID - 1, 0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f,1.0f,1.0f,0.6f) * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / (MACHINE_TYPE::NUM - 1 ) / 2),
							static_cast<float>(texData.height / 2)),
		MACHINEICON_RAGE);


	//　====================[　マシンのLv描画　]
	rect = SpriteCutter((int)SPRITE_RAGE, (int)SPRITE_RAGE, m_lv , 0);
	pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Black * color,
		0.0f,
		SimpleMath::Vector2(SPRITE_RAGE / 2, SPRITE_RAGE / 2),
		MACHINELV_RAGE);

}

void AlchemicalMachineObject::HitToMouse(MousePointer* pMP)
{
	//　====================[　フラグリセット　]
	m_hitMouseFlag = false;

	//　====================[　マウス判定を拡大　]
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	//　====================[　マシンToマウス　]
	if (CircleCollider(GetCircle(), mouseWolrdPos))
	{
		//　　|=>　接触を通知
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

	//　====================[　モデルの行列　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	//　====================[　ディフェンサーは常に拠点の方向を向く　]
	if (m_machineID == DEFENSER)
	{
		//　　|=> 拠点との距離
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		//　　|=> 回転アニメーションを打ち消す
		m_rotateAnimation = 0.0f;

		//　　|=> 角度を合わせるために90度回転させる
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));

		//　　|=> 拠点方向に面を向ける
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	//　====================[　常に右回りに回転　]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	//　====================[　常に縦に揺れる　]
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);

	//　====================[　追加パーツの行列　]
	SimpleMath::Matrix subModelData = SimpleMath::Matrix::Identity;
	subModelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	subModelData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * SUBMODEL_SPEED);
	subModelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x,m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL),m_data.pos.z);

	//　====================[　追加パーツが存在する場合　]
	if (m_machineID == ATTACKER)
	{
		m_subColor = SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f);

		// エフェクトの設定
		ring->UpdateEffects([&](IEffect* effect)
			{
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// 色変更
				lights->SetLightDiffuseColor(0,m_subColor);
				lights->SetLightDiffuseColor(1,m_subColor);
				lights->SetLightDiffuseColor(2,m_subColor);

			});

	}

	//　====================[　エフェクトの設定　]
	model->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);

			//　====================[　色変更　]
			lights->SetLightDiffuseColor(0, GetColor());
			lights->SetLightDiffuseColor(1, GetColor());
			lights->SetLightDiffuseColor(2, GetColor());

		});

	//　====================[　シルエット描画　]
	if (silhouette)
	{
		SilhouetteRender(model, modelData);
		// シェーダーの適応
		if (ring != nullptr) 		SilhouetteRender(ring, subModelData);

	}
	//　====================[　通常描画　]
	else
	{
		NomalRender(model, modelData, m_color);
		// 通常描画
		if (ring != nullptr) 		NomalRender(ring, subModelData, m_subColor);
	}

	//　====================[　シェーダーの解除　]
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

}

//　====================[　召喚時に呼ばれる関数　]
void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
	m_spawnTime = 0.0f;
}

//　====================[　次のレベルアップに必要な結晶量　]
const int AlchemicalMachineObject::GetNextLvCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).lvUp_crystal;
}

//　====================[　修繕に必要な結晶量　]
const int AlchemicalMachineObject::GetRepairCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).repea_crystal;
}

//　====================[　破壊時手に入る結晶量　]
const int AlchemicalMachineObject::GetDismantlingCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).dismantling_crystal;
}

//　====================[　シルエット描画　]
void AlchemicalMachineObject::SilhouetteRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	//　====================[　出現時演出が終わるまで処理をしない　]
	if (m_spawnTime <= 1.0f) return;

	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

}

//　====================[　通常描画　]
void AlchemicalMachineObject::NomalRender(DirectX::Model* model, SimpleMath::Matrix matrix, SimpleMath::Color color)
{
	ShareData& pSD = ShareData::GetInstance();

	ModelShader& pMS = ModelShader::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			pMS.MachineDrawShader(color,SimpleMath::Vector4(m_spawnTime, m_span, m_invincibleTime,1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(m_element).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(m_element).GetAddressOf());

		});


	//シェーダの登録を解除しておく
	pSD.GetContext()->VSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);

}

//　====================[　半透明描画　]
void AlchemicalMachineObject::TransparentRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{

	ShareData& pSD = ShareData::GetInstance();

	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().ToransparentShader();
		});

}

//　====================[　カスタムシェーダー無し描画　]
void AlchemicalMachineObject::NotShaderRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			//　====================[　半透明描画指定　]
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// 透明判定処理
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		});
}

//　====================[　ツーパスレンダリングに用いる描画　]
void AlchemicalMachineObject::WriteDepathRender(DirectX::Model* model, DirectX::Model* secondModel)
{
	ShareData& pSD = ShareData::GetInstance();

	//　====================[　モデルの行列　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	//　====================[　ディフェンサーは常に拠点の方向を向く　]
	if (m_machineID == DEFENSER)
	{
		//　　|=> 拠点との距離
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		//　　|=> 回転アニメーションを打ち消す
		m_rotateAnimation = 0.0f;

		//　　|=> 角度を合わせるために90度回転させる
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));

		//　　|=> 拠点方向に面を向ける
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	//　====================[　常に右回りに回転　]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	//　====================[　常に縦に揺れる　]
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);

	//　====================[　追加パーツの行列　]
	SimpleMath::Matrix subModelData = SimpleMath::Matrix::Identity;
	subModelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	subModelData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * SUBMODEL_SPEED);
	subModelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);


	//　====================[　モデルの描画　]
	//　　|=>　本体
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw();
		});

	//　　|=>　サブモデル
	if (secondModel != nullptr)
	{
		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), subModelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
			{
				ModelShader::GetInstance().ShadowModelDraw();
			});
	}
}
