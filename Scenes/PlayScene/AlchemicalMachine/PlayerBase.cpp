#include "pch.h"
#include "PlayerBase.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "Scenes/DataManager.h"

#define RAGE SimpleMath::Vector3(3, 3, 3)

PlayerBase::PlayerBase() :
	m_baseLv(1),
	m_hp(1),
	m_exp(),
	m_testBox(),
	m_hitMouseFlag(),
	m_lvUpTiming(),
	m_invincibleTime()
{
}

PlayerBase::~PlayerBase()
{
} 

void PlayerBase::Initialize()
{
	m_baseLv = ShareJsonData::GetInstance().GetStageData().resource.lv;
	m_exp = 0;
	m_hp = (float)DataManager::GetInstance()->GetNowBaseHP();

	m_data.pos  = SimpleMath::Vector3(0,0,0);
	m_data.rage = RAGE;

	auto pDataM = DataManager::GetInstance();

	pDataM->MPMAXRecalculation		(m_baseLv);
	pDataM->CrystalMAXRecalculation	(m_baseLv);
	pDataM->BaseHPMAXRecalculation	(m_baseLv);

	pDataM->Initialize();

}

void PlayerBase::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	int enemyKillNum = 0;
		
	enemyKillNum = DataManager::GetInstance()->GetNowEnemyKill();

	// 仮置き
	auto mouse = pINP.GetMouseState();
	bool rightRelease = pINP.RightButton_Release();
	rightRelease;

	m_hitMouseFlag = false;
	m_lvUpTiming = false;

	// 無敵時間の計測
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();
	m_invincibleTime += deltaTime;

	SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	if (PointerToCircle(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

	if (m_exp >= GetNextLvEXP() && m_baseLv < ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX)
	{
		// 超過分は繰越す
		float saveEXP = GetNextLvEXP() - m_exp;
		m_baseLv++;
		m_lvUpTiming = true;

		// 最大魔力量　最大結晶量　最大HP量 再計算
		DataManager::GetInstance()->MPMAXRecalculation(m_baseLv);
		DataManager::GetInstance()->CrystalMAXRecalculation(m_baseLv);
		DataManager::GetInstance()->BaseHPMAXRecalculation(m_baseLv);

		//m_hp = (float)DataManager::GetInstance()->GetNowBaseHP_MAX()

		// 超過分を入れる
		m_exp = saveEXP;

	}

	//DataManager::GetInstance()->SetNowBaseHP((int)m_hp);
}

void PlayerBase::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	///*===[ デバッグ文字描画 ]===*/
	//std::wostringstream oss;
	//oss << "Base-";
	//if (m_hitMouseFlag) oss << "Hit";
	//pSD.GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 100.f));

	SimpleMath::Matrix textBox = SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + 3.0f, m_data.pos.z);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Chocolate);

}

void PlayerBase::Render(DirectX::Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y - 1.5f, m_data.pos.z);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{

			//ModelShader::GetInstance().SilhouetteShader();

			// 深度ステンシルステートの設定
			//pSD.GetContext()->OMSetDepthStencilState(ModelShader::GetInstance().GetStencilBase().Get(), 0);
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

}

void PlayerBase::Finalize()
{

	m_baseModel.reset();
	m_testBox.reset();

}

int PlayerBase::GetNextLvEXP()
{
	auto gameParam = ShareJsonData::GetInstance().GetGameParameter();

	return gameParam.needExp + (gameParam.needExp_Growthrate * m_baseLv);
}

void PlayerBase::Damage(float damage)
{
	// 無敵時間が規定に達していればダメージを受ける
	if (m_invincibleTime >= 1.0f)
	{

		m_invincibleTime = 0.0f;
		DataManager::GetInstance()->SetNowBaseHP(DataManager::GetInstance()->GetNowBaseHP() - damage);
	
	}
}
