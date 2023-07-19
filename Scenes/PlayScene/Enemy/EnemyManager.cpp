#include "pch.h"
#include "EnemyManager.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/DataManager.h"
#include <algorithm>
#include <random>

EnemyManager::EnemyManager():
	m_timer(),
	m_totalTimer(),
	m_nextEnemyTime()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), DirectX::SimpleMath::Vector3(1,1,1));
	m_enemyObject = std::make_unique<std::list<EnemyObject>>();

	//m_particle_hit = std::make_unique<Particle>(Particle::HIT_BULLET);
	//m_particle_hit->Initialize();

	m_particle_spawn = std::make_unique<Particle>(Particle::SPAWN_ENEMY);
	m_particle_spawn->Initialize();

	m_particle_delete = std::make_unique<Particle>(Particle::DELETE_ENEMY);
	m_particle_delete->Initialize();
}

void EnemyManager::Update(DirectX::SimpleMath::Vector3 basePos)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	DataManager& pDM = *DataManager::GetInstance();

	m_timer += deltaTime;
	m_totalTimer += deltaTime;

	// 10�b���ɐ���
	if (m_timer >= 5.0f)
	{
		EnemyObject object = GetEnemyStatus(EnemyObject::EnemyType::NONE);

		m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
		// �G�t�F�N�g�\��
		m_particle_spawn->OnShot(object.GetPos(), true);
		m_timer = 0.0f;
	}

	// �X�V����
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// �q�N���X����false�ŏ���
		if ((it)->GotoTarget(basePos))
		{
			pDM.SetNowEnemyKill(pDM.GetNowEnemyKill() + 1);
			m_particle_delete->OnShot(it->GetPos(), true);
			it = m_enemyObject->erase(it);
			if (it == m_enemyObject->end()) break;
		}

		it->Update();
	}

	m_particle_spawn->UpdateParticle();
	m_particle_delete->UpdateParticle();
}

void EnemyManager::Render()
{

	ShareData& pSD = ShareData::GetInstance();

	///*===[ �f�o�b�O�����`�� ]===*/
	//std::wostringstream oss;
	//oss << "EnemyNum - " << m_enemyObject->size();
	//pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(400.f, 20.f));

	//m_particle_hit		->Render();
	m_particle_delete	->Render();
	m_particle_spawn	->Render();

	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		it->Draw();
		it->Render(m_testBox.get());
	}

}

void EnemyManager::Finalize()
{
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		it->Finalize();
		//delete& it;
	}
	m_enemyObject->clear();
	m_enemyObject.reset();

	delete m_enemyObject.get();

	m_testBox.reset();
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
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		if (CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()))
		{
			// ����
			it->GotoTarget(-alchemicalMachines->GetPos());

		}
	}

}
