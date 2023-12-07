#include "pch.h"
#include "PlayerBase.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

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

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_baseModel_Towor = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_1.cmo", *fx);
	m_baseModel_Pillar = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_2.cmo", *fx);

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

		// 超過分を入れる
		m_exp = saveEXP;

	}
}

void PlayerBase::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Matrix textBox = SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + 3.0f, m_data.pos.z);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Chocolate);

}

void PlayerBase::Render(DirectX::Model* model)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y - 1.5f, m_data.pos.z);

	m_baseModel_Towor->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{
			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), pSL.GetTowerBaseTexture());
			
			pSD.GetContext()->PSSetShaderResources(1, 1, pSL.GetTowerBaseTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, pSL.GetTowerBaseNormalMap().GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

	m_baseModel_Pillar->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{

			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SpriteLoder::GetInstance().GetTowerPillarTexture());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

}

void PlayerBase::WriteDepth(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y - 1.5f, m_data.pos.z);

	m_baseModel_Towor->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw(false);

		});

	m_baseModel_Pillar->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{

			ModelShader::GetInstance().ShadowModelDraw(false);

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
		DataManager::GetInstance()->SetNowBaseHP(DataManager::GetInstance()->GetNowBaseHP() - (int)damage);
	
	}
}
