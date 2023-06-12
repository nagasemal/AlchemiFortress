#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

AM_Attacker::AM_Attacker():
	 m_speed(),
	m_timer(),
	m_targetPos()
{
}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

}

void AM_Attacker::Update()
{

	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = 5.f;

	//// 更新処理
	//for (std::list<Bullet>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	//{
	//	it->Update();
	//	// 子クラスからfalseで消す
	//	if ((it)->deleteRequest())
	//	{
	//		it = m_bullets.erase(it);
	//		if (it == m_bullets.end()) break;
	//	}
	//}

}

void AM_Attacker::Draw()
{
	// 更新処理
	//for (std::list<Bullet>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	//{
	//	it->Render(m_testBox.get());
	//}

}

void AM_Attacker::Finalize()
{

}

void AM_Attacker::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{

	for (int i = 0; i < maxNum; i++)
	{
		//	存在チェック
		if(object[i].GetActiv())
		{

		}
	}

}

bool AM_Attacker::BulletRequest(std::list<EnemyObject>* enemys)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		if (CircleCollider(it->GetCircle(), m_magicCircle))
		{
			// 5秒毎に生成
			if (m_timer >= 0.5f)
			{

				m_timer = 0.0f;
				m_targetPos = it->GetPos();
				return true;
			}
		}
		//// balletToEnemy
		////　ToDO::balletクラス内で何ともならなかった為。
		//for (std::list<Bullet>::iterator bulletIt = m_bullets.begin(); bulletIt != m_bullets.end(); bulletIt++)
		//{
		//	if (CircleCollider(it->GetCircle(), bulletIt->GetCircle()))
		//	{
		//		bulletIt->SetLife(0);
		//		it->SetHp(it->GetHp() - (int)bulletIt->GetDamage());
		//	}
		//}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	return Bullet(2.f, 10.f, 5.f, m_data.pos,m_targetPos);
}
