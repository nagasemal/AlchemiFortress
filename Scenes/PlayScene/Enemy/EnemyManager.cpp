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

	// 10秒毎に生成
	if (m_timer >= 5.0f)
	{

		m_enemyObject.push_back(GetEnemyStatus(EnemyObject::EnemyType::NONE));

		m_timer = 0.0f;

	}

	// 更新処理
	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		it->Update();

		// 子クラスからfalseで消す
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

	/*===[ デバッグ文字描画 ]===*/
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

// アルケミカルマシンtoエネミー  呼び出し元含めて二重for文で回しているため、数がかさむと重くなる。
void EnemyManager::HitAMObejct(AlchemicalMachineObject* alchemicalMachines)
{
	//　現存存在するエネミー分回す
	for (std::list<EnemyObject>::iterator it = m_enemyObject.begin(); it != m_enemyObject.end(); it++)
	{
		if (CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()))
		{



		}
	}

}
