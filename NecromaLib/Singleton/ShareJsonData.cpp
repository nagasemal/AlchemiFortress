#include "pch.h"
#include "ShareJsonData.h"

ShareJsonData* ShareJsonData::instance = nullptr;

#define MAX_STAGE 4
#define MIN_STAGE 1

ShareJsonData::ShareJsonData():
	m_bulletData{}
{

}

void ShareJsonData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareJsonData;
	}
}

void ShareJsonData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void ShareJsonData::LoadingJsonFile_Bullet()
{

	m_bulletData[ELEMENT::NOMAL]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Nomal.json");
	m_bulletData[ELEMENT::FLAME]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Flame.json");
	m_bulletData[ELEMENT::AQUA]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Aqua.json");
	m_bulletData[ELEMENT::WIND]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Wind.json");
	m_bulletData[ELEMENT::EARTH]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Earth.json");

}

void ShareJsonData::LoadingJsonFile_Machine()
{
	m_machineData[MACHINE_TYPE::NONE]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_None.json");
	m_machineData[MACHINE_TYPE::ATTACKER]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Attacker.json");
	m_machineData[MACHINE_TYPE::DEFENSER]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Defenser.json");
	m_machineData[MACHINE_TYPE::MINING]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Mining.json");
	m_machineData[MACHINE_TYPE::RECOVERY]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Recovery.json");
	m_machineData[MACHINE_TYPE::UPPER]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Upper.json");
}

void ShareJsonData::LoadingJsonFile_Enemy()
{
	m_enemyData[ENEMY_TYPE::ENMEY_NONE] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_None.json");
	m_enemyData[ENEMY_TYPE::SLIME] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Slime.json");
	m_enemyData[ENEMY_TYPE::WORM] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Worm.json");
}

void ShareJsonData::LoadingJsonFile_GameParameter()
{

	m_gameParam = Json::FileLoad_GameParameter("Resources/Json/GameParameter/GameParameter.json");

}

void ShareJsonData::LoadingJsonFile_Stage(int number, int wave)
{

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number << "_" << wave;

	std::string filePath = "Resources/Json/StageData/StageData_" + oss.str() + ".json";

	m_stageData = Json::FileLoad_StageData(filePath);

}

void ShareJsonData::LoadingJsonFile_ClearData(int number)
{
	m_clearData = Stage_ClearData();

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number;
	std::string filePath = "Resources/Json/ClearData/ClearData_" + oss.str() + ".json";

	m_clearData = Json::FileLoad_StageClearData(filePath);
}

const Bullet_Data ShareJsonData::GetBulletData(ELEMENT element)
{
	return m_bulletData[element];
}

const Machine_Data ShareJsonData::GetMachineData(MACHINE_TYPE type)
{
	return m_machineData[type];
}

const Enemy_Data ShareJsonData::GetEnemyData(ENEMY_TYPE type)
{
	return m_enemyData[type];
}

const Stage_Data ShareJsonData::GetStageData()
{
	return m_stageData;
}

const Stage_ClearData ShareJsonData::GetClearData()
{
	return m_clearData;
}

const Game_Parameter ShareJsonData::GetGameParameter()
{
	return m_gameParam;
}

void ShareJsonData::StageDataCleanUP()
{

	m_stageData = Stage_Data();

}

SimpleMath::Color ShareJsonData::GetElementColor(ELEMENT element)
{
	return Json::ChangeColor(element);
}
