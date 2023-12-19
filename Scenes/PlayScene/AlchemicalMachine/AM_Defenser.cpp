#include "pch.h"
#include "AM_Defenser.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "NecromaLib/Singleton/DeltaTime.h"

AM_Defenser::AM_Defenser():
	m_isBreak(),
	m_counterFlag()
{
}

AM_Defenser::~AM_Defenser()
{
}

void AM_Defenser::Initialize()
{
	m_machineID = MACHINE_TYPE::DEFENSER;
	m_objectName = "Defenser";

	// Jsonから読み取ったマシンのデータを適応する
	float machineHP = (float)ShareJsonData::GetInstance().GetMachineData(m_machineID).hp;

	m_hp = (int)machineHP;
	m_maxHp = (int)machineHP;

}

void AM_Defenser::Update()
{
	m_data.pos.y = 0.0f;
}

void AM_Defenser::SelectUpdate()
{

}

void AM_Defenser::Draw()
{
}

void AM_Defenser::Finalize()
{
}

void AM_Defenser::RenderUI()
{

}

void AM_Defenser::LvUp()
{
	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;

	// HP強化
	m_maxHp = (int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv));
	// HP回復
	m_hp = m_maxHp;


}

void AM_Defenser::EnemyHit(std::list<EnemyObject>* enemys)
{

	m_counterFlag = false;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(),it->GetObject3D());

		if (hitMachine)
		{
			it->HitMachine(hitMachine);
			it->Bouns();

			if (m_invincibleFlag) continue;

			// 体力減少
			m_hp -= (int)it->GetPower();
			it->HitBullet(it->GetPos() - m_data.pos, m_lv);
			m_invincibleFlag = true;
			m_counterFlag = true;
		}
	}
}

bool AM_Defenser::CounterAttack()
{
	return m_counterFlag;
}
