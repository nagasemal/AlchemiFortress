#include "pch.h"
#include "EnemyManager.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include <algorithm>
#include <random>

EnemyManager::EnemyManager():
	m_timer(),
	m_totalTimer()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), DirectX::SimpleMath::Vector3(1,1,1));

}

void EnemyManager::Update(DirectX::SimpleMath::Vector3 basePos)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;
	m_totalTimer += deltaTime;

	// 10�b���ɐ���
	if (m_timer >= 5.0f)
	{

		m_enemyObject.push_back(GetEnemyStatus(EnemyObject::EnemyType::NONE));

		m_timer = 0.0f;

	}

	// �X�V����
	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		it->Update();

		// �q�N���X����false�ŏ���
		if ((it)->GotoTarget(DirectX::SimpleMath::Vector3::Zero))
		{
			it = m_enemyObject.erase(it);
			if (it == m_enemyObject.end()) break;
		}
	}
}

void EnemyManager::Render()
{

	ShareData& pSD = ShareData::GetInstance();

	/*===[ �f�o�b�O�����`�� ]===*/
	std::wostringstream oss;
	oss << "EnemyNum - " << m_enemyObject.size();
	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(400.f, 20.f));

	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		it->Draw();
		it->Render(m_testBox.get());
	}

}

void EnemyManager::Finalize()
{
	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		it->Finalize();
	}
}

EnemyObject EnemyManager::GetEnemyStatus(EnemyObject::EnemyType type)
{
	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_real_distribution<> dist(0, XM_2PI);

	float rand = static_cast<float>(dist(engine));

	EnemyObject enemy(type,DirectX::SimpleMath::Vector3(50 * cosf(rand), 0, 50 * sinf(rand)),1);

	return enemy;
}

// �A���P�~�J���}�V��to�G�l�~�[  �Ăяo�����܂߂ē�dfor���ŉ񂵂Ă��邽�߁A���������ނƏd���Ȃ�B
void EnemyManager::HitAMObejct(AlchemicalMachineObject* alchemicalMachines)
{
	//�@�������݂���G�l�~�[����
	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		if (CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()))
		{



		}
	}

}
