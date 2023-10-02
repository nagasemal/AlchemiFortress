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

	// エネミーの数値取得
	ShareJsonData::GetInstance().LoadingJsonFile_Enemy();
}

void EnemyManager::Update(SimpleMath::Vector3 basePos)
{

	basePos;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	DataManager& pDM = *DataManager::GetInstance();
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// 毎秒初期化
	m_knokDownFlag = 0;
	m_knokDownEnemyType = ENEMY_TYPE::ENMEY_NONE;

	m_timer += deltaTime;
	m_totalTimer += deltaTime;


	// ランダムで出現
	if (pSJD.GetStageData().enemys_Spawn[0].type == ENEMY_TYPE::ENMEY_NONE)
	{
		if (m_timer >= pSJD.GetStageData().enemys_Spawn[0].spawnTime)
		{
			EnemyObject object = GetRandomEnemy();

			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
			// エフェクト表示
			m_particle_spawn->OnShot(object.GetPos(), true);

			// 次のエネミーを呼び出す準備
			m_timer = 0.0f;

		}
	}
	else
	{
		// 時間が来たら生成する
		if (m_enemyNums < pSJD.GetStageData().enemys_Spawn.size())
		{
			if (m_timer >= pSJD.GetStageData().enemys_Spawn[m_enemyNums].spawnTime)
			{
				EnemyObject object = GetEnemyStatus(ENEMY_TYPE::SLIME, m_enemyNums);

				m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
				// エフェクト表示
				m_particle_spawn->OnShot(object.GetPos(), true);

				// 次のエネミーを呼び出す準備
				m_enemyNums++;
			}
		}

	}

	// 更新処理
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// 子クラスからfalseで消す
		if (it->GetHp() <= 0)
		{
			pDM.SetNowEnemyKill(pDM.GetNowEnemyKill() + 1);

			m_knokDownFlag++;
			m_knokDownEnemyType = it->GetEnemyType();

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

	///*===[ デバッグ文字描画 ]===*/
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

//　Jsonファイルから読み取った情報を元にマシンを製造する
EnemyObject EnemyManager::GetEnemyStatus(ENEMY_TYPE type,int spawnNumber)
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//int remoteness = (int)((float)pSJD.GetStageData().enemys_Spawn[spawnNumber].remoteness * 1.5f);
	//float direction = (float)pSJD.GetStageData().enemys_Spawn[spawnNumber].direction;

	EnemyObject enemy(type, pSJD.GetStageData().enemys_Spawn[spawnNumber].spawnPos, 1);
	Enemy_Data enemyData = pSJD.GetEnemyData(pSJD.GetStageData().enemys_Spawn[spawnNumber].type);

	enemy.Initialize();
	enemy.SetEnemyData(enemyData);

	return enemy;
}

EnemyObject EnemyManager::GetRandomEnemy()
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	std::uniform_int_distribution<> dist_enemyType(1, 2);
	std::random_device rd;
	int enemyType_rand = static_cast<int>(dist_enemyType(rd));

	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_real_distribution<> dist(0, XM_2PI);
	
	std::uniform_int_distribution<> dist2(20,40);
	std::mt19937 gen(rd());
	float rand = static_cast<float>(dist(engine));
	float rand2 = static_cast<float>(dist2(gen));

	EnemyObject enemy((ENEMY_TYPE)enemyType_rand, SimpleMath::Vector3(rand2 * cosf(rand), 1.0f, rand2 * sinf(rand)), 1);
	Enemy_Data enemyData = pSJD.GetEnemyData((ENEMY_TYPE)enemyType_rand);

	enemy.Initialize();

	enemy.SetEnemyData(enemyData);

	return enemy;
}

// アルケミカルマシンtoエネミー  呼び出し元含めて二重for文で回しているため、数がかさむと重くなる。
void EnemyManager::HitAMObejct(AlchemicalMachineObject* alchemicalMachines)
{
	//　現存存在するエネミー分回す
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// 反発
		it->SetStopFlag(CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()));
	}

}
