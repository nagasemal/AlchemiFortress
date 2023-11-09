#include "pch.h"
#include "AM_Recovery.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

AM_Recovery::AM_Recovery()
{
}

AM_Recovery::~AM_Recovery()
{
}

void AM_Recovery::Initialize()
{

	m_machineID = MACHINE_TYPE::RECOVERY;
	m_objectName = "Recovery";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_maxHp = m_hp		 = pSJD.GetMachineData(m_machineID).hp;
	m_machineEffectValue = pSJD.GetMachineData(m_machineID).effect_value;


}

void AM_Recovery::Update()
{
	m_magicCircle.p = m_data.pos;

}

void AM_Recovery::SelectUpdate()
{

}

void AM_Recovery::Draw()
{
}

void AM_Recovery::Finalize()
{
}

void AM_Recovery::RenderUI()
{
}

void AM_Recovery::LvUp()
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

void AM_Recovery::HitEnemy(std::list<EnemyObject>* enemy)
{

	// TRUEならば早急に弾く
	if (m_invincibleFlag) return;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemy->begin(); it != enemy->end(); it++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		if (hitMachine)
		{
			// 体力減少
			m_hp -= (int)it->GetPower();
			m_invincibleFlag = true;

		}
	}
}

void AM_Recovery::MPPuls(DataManager* pDM)
{
	// MPを増やす
	pDM->SetNowMP(pDM->GetNowMP() + (int)(m_lv * m_machineEffectValue));

}
