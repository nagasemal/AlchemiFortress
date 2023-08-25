#include "pch.h"
#include "ShareJsonData.h"

ShareJsonData* ShareJsonData::instance = nullptr;

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

	m_bulletData[MACHINE_ELEMENT::NOMAL]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Nomal.json");
	m_bulletData[MACHINE_ELEMENT::FLAME]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Flame.json");
	m_bulletData[MACHINE_ELEMENT::AQUA]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Aqua.json");
	m_bulletData[MACHINE_ELEMENT::WIND]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Wind.json");
	m_bulletData[MACHINE_ELEMENT::EARTH]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Earth.json");

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

void ShareJsonData::LoadingJsonFile_Stage(int number)
{

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number;
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

const Bullet_Data ShareJsonData::GetBulletData(MACHINE_ELEMENT element)
{
	return m_bulletData[element];
}

const Machine_Data ShareJsonData::GetMachineData(MACHINE_TYPE type)
{
	return m_machineData[type];
}

const Stage_Data ShareJsonData::GetStageData()
{
	return m_stageData;
}

const Stage_ClearData ShareJsonData::GetClearData()
{
	return m_clearData;
}

void ShareJsonData::StageDataCleanUP()
{

	m_stageData = Stage_Data();

}

DirectX::SimpleMath::Color ShareJsonData::GetElementColor(MACHINE_ELEMENT element)
{
	return Json::ChangeColor(element);
}
