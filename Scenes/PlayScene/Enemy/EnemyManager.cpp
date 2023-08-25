#include "pch.h"
#include "EnemyManager.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/DataManager.h"
#include <algorithm>
#include <random>

#include "NecromaLib/Singleton/ShareJsonData.h"

EnemyManager::EnemyManager() :
	m_timer(),
	m_totalTimer(),
	m_nextEnemyTime(),
	m_knokDownEnemyType(ENEMY_TYPE::ENMEY_NONE),
	m_knokDownFlag(0),
	m_enemyNums()
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

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_enemyModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Slime.cmo", *fx);
}

void EnemyManager::Update(DirectX::SimpleMath::Vector3 basePos)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	DataManager& pDM = *DataManager::GetInstance();
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// ���b������
	m_knokDownFlag = 0;
	m_knokDownEnemyType = ENEMY_TYPE::ENMEY_NONE;

	m_timer += deltaTime;
	m_totalTimer += deltaTime;

	// ���Ԃ������琶������

	if (m_enemyNums < pSJD.GetStageData().enemys_Spawn.size())
	{
		if (m_timer >= pSJD.GetStageData().enemys_Spawn[m_enemyNums].spawnTime)
		{
			EnemyObject object = GetEnemyStatus(ENEMY_TYPE::SLIME, m_enemyNums);

			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
			// �G�t�F�N�g�\��
			m_particle_spawn->OnShot(object.GetPos(), true);

			// ���̃G�l�~�[���Ăяo������
			m_enemyNums++;
		}
	}

	// �X�V����
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// �q�N���X����false�ŏ���
		if ((it)->GotoTarget(basePos))
		{
			pDM.SetNowEnemyKill(pDM.GetNowEnemyKill() + 1);

			m_knokDownFlag++;
			m_knokDownEnemyType = it->GetEnemyType();

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

	///*===[ �f�o�b�O�����`�� ]===*/
	//std::wostringstream oss;
	//oss << "EnemyNum - " << m_enemyObject->size();
	//pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(400.f, 20.f));

	//m_particle_hit	->Render();
	m_particle_delete	->Render();
	m_particle_spawn	->Render();

	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		it->Draw();
		it->Render(m_enemyModel.get());
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

//�@Json�t�@�C������ǂݎ�����������Ƀ}�V���𐻑�����
EnemyObject EnemyManager::GetEnemyStatus(ENEMY_TYPE type,int spawnNumber)
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//std::random_device seed;
	//std::default_random_engine engine(seed());
	//std::uniform_real_distribution<> dist(0, XM_2PI);
	//float rand = static_cast<float>(dist(engine));

	int remoteness = (int)((float)pSJD.GetStageData().enemys_Spawn[spawnNumber].remoteness * 1.5f);
	float direction = (float)pSJD.GetStageData().enemys_Spawn[spawnNumber].direction;

	EnemyObject enemy(type,DirectX::SimpleMath::Vector3(remoteness * cosf(direction), 0.0f, remoteness * sinf(direction)),1);

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
