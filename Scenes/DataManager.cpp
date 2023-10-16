#include <pch.h>
#include "DataManager.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

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
	m_stageNum		(1),
	m_stageClearFlag()
{

}

void DataManager::Initialize()
{
	auto resource = ShareJsonData::GetInstance().GetStageData().resource;

	m_nowMP_MAX			= STANDARD_MP;
	m_nowMP				= resource.mp;

	m_nowCrystal_MAX	= STANDARD_CRYSTAL;
	m_nowCrystal		= resource.crystal;

	m_nowBaseHp_MAX		= STANDARD_BASEHP;
	m_nowBaseHp			= STANDARD_BASEHP;

	m_nowEnemyKill = 0;
	m_round = 1;
}

void DataManager::Update()
{
	// è„å¿â∫å¿ê›íË
	m_nowMP = std::min(std::max(m_nowMP, 0), m_nowMP_MAX);

	m_nowCrystal = std::min(std::max(m_nowCrystal, 0), m_nowCrystal_MAX);

	m_nowBaseHp = std::min(std::max(m_nowBaseHp, 0), m_nowBaseHp_MAX);

}

void DataManager::MPMAXRecalculation(int lv)
{
	m_nowMP_MAX = STANDARD_MP + (STANDARD_MP / 2 * lv);
}

void DataManager::CrystalMAXRecalculation(int lv)
{
	m_nowCrystal_MAX = STANDARD_CRYSTAL + (STANDARD_CRYSTAL / 2 * lv);
}

void DataManager::BaseHPMAXRecalculation(int lv)
{
	m_nowBaseHp_MAX = STANDARD_BASEHP + (STANDARD_BASEHP / 2 * lv);

	// ëSâÒïú
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