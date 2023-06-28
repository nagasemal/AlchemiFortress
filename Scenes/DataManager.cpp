#include <pch.h>
#include "DataManager.h"

DataManager* DataManager::instance = nullptr;

DataManager::DataManager() :
	m_nowMP(1000),
	m_nowCrystal(1000),
	m_nowEnemyKill(0)
{

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