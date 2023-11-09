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
	m_enemyNums(),
	m_falmeTotalEnemyExp()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), SimpleMath::Vector3(1,1,1));
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

	// �G�l�~�[�̐��l�擾
	auto pSJD = &ShareJsonData::GetInstance(); 
	pSJD->LoadingJsonFile_Enemy();

	ReloadEnemyData();

	//// �����_�������łȂ��ꍇ���߂��琶�������Ă���
	//if (pSJD->GetStageData().enemys_Spawn[0].type != ENEMY_TYPE::ENMEY_NONE)
	//{
	//	// ���߂���o��������G�l�~�[�̏����擾���Ă���
	//	for (int i = 0; i < pSJD->GetStageData().enemys_Spawn.size(); i++)
	//	{
	//		
	//		EnemyObject object = GetEnemyStatus(ENEMY_TYPE::SLIME, i);
	//		m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
	//	
	//	}
	//}
	//else
	//{
	//	// ���߂���o��������G�l�~�[�̏����擾���Ă���
	//	for (int i = 0; i < pSJD->GetStageData().condition_Enemy.size(); i++)
	//	{
	//
	//		EnemyObject object = GetRandomEnemy();
	//		m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
	//	
	//	}
	//}
}

void EnemyManager::Update(SimpleMath::Vector3 basePos)
{
	// ��Ƀ��Z�b�g��������
	m_falmeTotalEnemyExp = 0;
	basePos;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	DataManager& pDM = *DataManager::GetInstance();

	// ���b������
	m_knokDownFlag = 0;
	m_knokDownEnemyType = ENEMY_TYPE::ENMEY_NONE;

	m_timer += deltaTime;
	m_totalTimer += deltaTime;


	//// �����_���ŏo��
	//if (pSJD.GetStageData().enemys_Spawn[0].type == ENEMY_TYPE::ENMEY_NONE)
	//{
	//	if (m_timer >= pSJD.GetStageData().enemys_Spawn[0].spawnTime)
	//	{
	//		EnemyObject object = GetRandomEnemy();
	//
	//		m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
	//		// �G�t�F�N�g�\��
	//		m_particle_spawn->OnShot(object.GetPos(), true);
	//
	//		// ���̃G�l�~�[���Ăяo������
	//		m_timer = 0.0f;
	//
	//	}
	//}
	//else
	//{
	//	// ���Ԃ������琶������
	//	if (m_enemyNums < pSJD.GetStageData().enemys_Spawn.size())
	//	{
	//		if (m_timer >= pSJD.GetStageData().enemys_Spawn[m_enemyNums].spawnTime)
	//		{
	//			EnemyObject object = GetEnemyStatus(ENEMY_TYPE::SLIME, m_enemyNums);
	//
	//			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
	//			// �G�t�F�N�g�\��
	//			m_particle_spawn->OnShot(object.GetPos(), true);
	//
	//			// ���̃G�l�~�[���Ăяo������
	//			m_enemyNums++;
	//		}
	//	}
	//
	//}

	// �X�V����
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// �������ꂽ�u�Ԃ��擾���ăp�[�e�B�N�����o��
		m_particle_spawn->OnShot(it->GetPos(), it->GetAliveTimer() <= 0.0f);

		// �q�N���X����false�ŏ���
		if (it->GetDethFlag())
		{
			pDM.SetNowEnemyKill(pDM.GetNowEnemyKill() + 1);

			m_knokDownFlag++;
			m_knokDownEnemyType = it->GetEnemyType();

			// EXP���l���o����悤�ɂ���
			m_falmeTotalEnemyExp += it->GetEXP();

			m_particle_delete->OnShot(it->GetPos(), true);

			it->Finalize();
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
	//pSD.GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(400.f, 20.f));
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

void EnemyManager::ReloadEnemyData()
{
	m_enemyNums = 0;
	m_timer = 0;

	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// ���g�����݂��Ȃ��ꍇ�͔�΂�
	if(pSJD.GetStageData().enemys_Spawn.size() == 0) return;

	// �����_�������łȂ��ꍇ���߂��琶�������Ă���
	if (pSJD.GetStageData().enemys_Spawn[0].type != ENEMY_TYPE::ENMEY_NONE)
	{
		// ���߂���o��������G�l�~�[�̏����擾���Ă���
		for (int i = 0; i < pSJD.GetStageData().enemys_Spawn.size(); i++)
		{

			EnemyObject object = GetEnemyStatus(ENEMY_TYPE::SLIME, i);
			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
		
		}
	}
	else
	{
		// ���߂���o��������G�l�~�[�̏����擾���Ă���
		for (int i = 0; i < pSJD.GetStageData().condition_Enemy[0].value; i++)
		{

			EnemyObject object = GetRandomEnemy();
			object.SetAliveTimer(pSJD.GetStageData().enemys_Spawn[0].spawnTime * (1 + i));
			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));

		}
	}

}

//�@Json�t�@�C������ǂݎ�����������Ƀ}�V���𐻑�����
EnemyObject EnemyManager::GetEnemyStatus(ENEMY_TYPE type,int spawnNumber)
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	Enemys_Spawn enemySpawn = pSJD.GetStageData().enemys_Spawn[spawnNumber];

	// �G�l�~�[�I�u�W�F�N�g�𐶐�
	EnemyObject enemy(type, enemySpawn.spawnPos, enemySpawn.lv,enemySpawn.spawnTime);

	// �G�l�~�[�̃p�����[�^�𐶐�
	Enemy_Data enemyData = pSJD.GetEnemyData(pSJD.GetStageData().enemys_Spawn[spawnNumber].type);

	// ����������
	enemy.Initialize();

	// �G�l�~�[�̃p�����[�^������
	enemy.SetEnemyData(enemyData);

	return enemy;
}

EnemyObject EnemyManager::GetRandomEnemy()
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	std::uniform_int_distribution<> dist_enemyType(1,ENEMY_NUM - 1);
	std::random_device rd;
	int enemyType_rand = static_cast<int>(dist_enemyType(rd));

	std::random_device seed;
	std::default_random_engine engine(seed());
	// �~����̂ǂ��ɐݒu���邩�����߂�
	std::uniform_real_distribution<> dist(0, XM_2PI);
	// ���_����̋������擾
	std::uniform_int_distribution<> dist2(20,40);
	std::mt19937 gen(rd());
	float rand = static_cast<float>(dist(engine));
	float rand2 = static_cast<float>(dist2(gen));

	EnemyObject enemy((ENEMY_TYPE)enemyType_rand, SimpleMath::Vector3(rand2 * cosf(rand), 1.0f, rand2 * sinf(rand)), 1, 0.0f);
	Enemy_Data enemyData = pSJD.GetEnemyData((ENEMY_TYPE)enemyType_rand);

	enemy.Initialize();

	enemy.SetEnemyData(enemyData);

	return enemy;
}

// �A���P�~�J���}�V��to�G�l�~�[  �Ăяo�����܂߂ē�dfor���ŉ񂵂Ă��邽�߁A���������ނƏd���Ȃ�B
void EnemyManager::HitAMObejct(AlchemicalMachineObject* alchemicalMachines)
{
	//�@�������݂���G�l�~�[����
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// ����
		it->SetStopFlag(CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()));
	}

}

int EnemyManager::GetNockDownEnemyExp()
{
	return m_falmeTotalEnemyExp;
}
