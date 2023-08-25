#include <pch.h>
#include "DataManager.h"

#define STANDARD_MP			400
#define STANDARD_CRYSTAL	215
#define STANDARD_BASEHP		50

DataManager* DataManager::instance = nullptr;

DataManager::DataManager() :
	m_nowMP			(),
	m_nowCrystal	(),
	m_nowBaseHp		(),
	m_nowCrystal_MAX(),
	m_nowMP_MAX		(),
	m_nowBaseHp_MAX	(),
	m_round			(),
	m_nowEnemyKill	(),
	m_stageNum		()
{

}

void DataManager::Initialize()
{
	m_nowMP_MAX			= STANDARD_MP;
	m_nowMP				= STANDARD_MP / 2;

	m_nowCrystal_MAX	= STANDARD_CRYSTAL;
	m_nowCrystal		= STANDARD_CRYSTAL / 2;

	m_nowBaseHp_MAX		= STANDARD_BASEHP;
	m_nowBaseHp			= STANDARD_BASEHP / 2;

	m_nowEnemyKill = 0;
	m_round = 1;
}

void DataManager::Update()
{
	// �ő�l������ɍs���Ȃ�
	if (m_nowMP >= m_nowMP_MAX) m_nowMP = m_nowMP_MAX;

	if (m_nowCrystal >= m_nowCrystal_MAX) m_nowCrystal = m_nowCrystal_MAX;

	if (m_nowBaseHp >= m_nowBaseHp_MAX) m_nowBaseHp = m_nowBaseHp_MAX;

}

void DataManager::MPMAXRecalculation(int lv)
{
	m_nowMP_MAX = STANDARD_MP * lv;
}

void DataManager::CrystalMAXRecalculation(int lv)
{
	m_nowCrystal_MAX = STANDARD_CRYSTAL * lv;
}

void DataManager::BaseHPMAXRecalculation(int lv)
{
	m_nowBaseHp_MAX = STANDARD_BASEHP * lv;

	// �S��
	m_nowBaseHp = m_nowBaseHp_MAX;

}


//void DataManager::Create()
//{
//	if (instance == nullptr)
//	{
//		instance = new DataManager;
//	}
//}
//
//void DataManager::Destroy()
//{
//	if (instance != nullptr)
//	{
//		delete instance;
//		instance = nullptr;
//	}
//}