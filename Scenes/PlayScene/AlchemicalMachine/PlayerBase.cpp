#include "pch.h"
#include "PlayerBase.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/DataManager.h"

#define RAGE DirectX::SimpleMath::Vector3(1, 1, 1)

PlayerBase::PlayerBase() :
	m_baseLv(),
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

	ShareData& pSD = ShareData::GetInstance();
	m_baseLv = 1;
	m_exp = 0;
	m_hp = 50;

	m_data.pos  = DirectX::SimpleMath::Vector3::Zero;
	m_data.rage = RAGE;

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), m_data.rage * 2.5);

	auto pDataM = DataManager::GetInstance();

	pDataM->MPMAXRecalculation		(m_baseLv);
	pDataM->CrystalMAXRecalculation	(m_baseLv);
	pDataM->BaseHPMAXRecalculation	(m_baseLv);

	pDataM->SetNowMP		(pDataM->GetNowMP_MAX() / 2);
	pDataM->SetNowCrystal	(pDataM->GetNowCrystal_MAX() / 2);
	pDataM->SetNowBaseHP	(pDataM->GetNowBaseHP_MAX() / 2);
}

void PlayerBase::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	int enemyKillNum = DataManager::GetInstance()->GetNowEnemyKill();

	// 仮置き
	auto mouse = pINP.GetMouseState();
	bool rightRelease = mouse.rightButton == mouse.RELEASED;

	m_hitMouseFlag = false;
	m_lvUpTiming = false;

	// 無敵時間の計測
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();
	m_invincibleTime += deltaTime;

	DirectX::SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	if (PointerToCircle(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

	// 五の倍数毎にLvUP
	if (enemyKillNum >= (2 * m_baseLv) + (4 * m_baseLv))
	{
		m_baseLv++;
		m_lvUpTiming = true;

		// 最大魔力量　最大結晶量　最大HP量 再計算
		DataManager::GetInstance()->MPMAXRecalculation(m_baseLv);
		DataManager::GetInstance()->CrystalMAXRecalculation(m_baseLv);
		DataManager::GetInstance()->BaseHPMAXRecalculation(m_baseLv);

		m_hp = (int)DataManager::GetInstance()->GetNowBaseHP_MAX();

	}

	DataManager::GetInstance()->SetNowBaseHP((int)m_hp);
}

void PlayerBase::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "Base-";
	if (m_hitMouseFlag) oss << "Hit";
	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 100.f));

	DirectX::SimpleMath::Matrix textBox = DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Chocolate);

}

void PlayerBase::Render(DirectX::Model* model)
{

	model;

}

void PlayerBase::Finalize()
{
	m_baseModel.reset();
	m_testBox.reset();
}

void PlayerBase::Damage(float damage)
{
	// 無敵時間が規定に達していればダメージを受ける
	if (m_invincibleTime >= 1.0f)
	{
		m_invincibleTime = 0;
		m_hp -= damage;
	}
}
