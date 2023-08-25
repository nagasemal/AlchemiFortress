#include "pch.h"
#include "JsonLoder.h"

#include <fstream>
#include <cassert>
#include "picojson.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

#define COLOR_WIGHT		{1.0f,1.0f,1.0f,1.0f}
#define COLOR_RED		{1.0f,0.0f,0.0f,1.0f}
#define COLOR_BLUE		{0.0f,0.0f,1.0f,1.0f}
#define COLOR_GREEN		{0.0f,1.0f,0.0f,1.0f}
#define COLOR_YELLOW	{0.8f,0.8f,0.0f,1.0f}


Bullet_Data Json::FileLoad_BulletData(const std::string filePath)
{
	//	読み込み用変数
	std::ifstream ifs;

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	//	読み込みチェック
	//	ifs変数にデータがなければエラー
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();
	Bullet_Data status;

	//	読み込んだデータを構造体に代入
	status.element = val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["ELEMENT"].get<std::string>();

	status.str = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["STR"].get<double>();

	status.speed = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["SPEED"].get<double>();

	status.span = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["SPAN"].get<double>();

	status.life = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["LIFE"].get<double>();

	status.lossMp = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["LOSSMP"].get<double>();

	return status;
}

Machine_Data Json::FileLoad_MachineData(const std::string filePath)
{
	//	読み込み用変数
	std::ifstream ifs;

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	//	読み込みチェック
	//	ifs変数にデータがなければエラー
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();
	Machine_Data status;

	//	読み込んだデータを構造体に代入
	status.element = val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["ELEMENT"].get<std::string>();

	status.lv = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["LV"].get<double>();

	status.hp = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["HP"].get<double>();

	status.multiplier_hp = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["MULTIPLIER_HP"].get<double>();

	status.effect_rage = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["AREAEFFECT"].get<double>();

	status.multiplier_effect = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["MULTIPLIER_EFFECT"].get<double>();

	status.alchemi_mp = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["ALCHEMI_MP"].get<double>();

	status.alchemi_crystal = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["ALCHEMI_CRYSTAL"].get<double>();

	status.repea_crystal = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["REPEA_CRYSTAL"].get<double>();

	status.lvUp_crystal = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["LVUP_CRYSTAL"].get<double>();

	status.dismantling_crystal = (int)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["DISMANTLING_CRYSTAL"].get<double>();

	status.effect_value = (float)val.get<picojson::object>()
		["Status"].get<picojson::object>()
		["EFFECT_VALUE"].get<double>();

	return status;
}

Stage_Data Json::FileLoad_StageData(const std::string filePath)
{
	//	読み込み用変数
	std::ifstream ifs;

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	//	読み込みチェック
	//	ifs変数にデータがなければエラー
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();
	Stage_Data status;

	//	読み込んだデータを構造体に代入
	// クリア条件
	// stageDataからMACHINE_SPAWNの配列の中身を得る
	picojson::object& stageData_Machine = val.get<picojson::object>()["Conditions_Machine"].get<picojson::object>();
	picojson::array& stageCondition_Machine = stageData_Machine["MACHINE_SPAWN"].get<picojson::array>();

	// 要素分回す
	for (picojson::array::iterator it = stageCondition_Machine.begin(); it != stageCondition_Machine.end(); it++) {

		Stage_Condition condition;
		condition.condition = it->get<picojson::object>()["TYPE"].get<std::string>();
		condition.value = (int)it->get<picojson::object>()["VALUE"].get<double>();

		if (condition.condition != "False")
		{
			status.condition_Machine.push_back(condition);
		}
	}

	// stageDataからENEMY_KNOCKDOWNの配列の中身を得る
	picojson::object& stageData_Enemy = val.get<picojson::object>()["Conditions_Enemy"].get<picojson::object>();
	picojson::array& stageCondition_Enemy = stageData_Enemy["ENEMY_KNOCKDOWN"].get<picojson::array>();

	for (picojson::array::iterator it = stageCondition_Enemy.begin(); it != stageCondition_Enemy.end(); it++) {

		Stage_Condition condition;
		condition.condition = it->get<picojson::object>()["TYPE"].get<std::string>();
		condition.value = (int)it->get<picojson::object>()["VALUE"].get<double>();

		if (condition.condition != "False")
		{
			status.condition_Enemy.push_back(condition);
		}

	}

	// stageDataからTimeの配列の中身を得る
	picojson::object& stageData_Time = val.get<picojson::object>()["Conditions_Time"].get<picojson::object>();
	picojson::array& stageCondition_Time = stageData_Time["TIME"].get<picojson::array>();

	for (picojson::array::iterator it = stageCondition_Time.begin(); it != stageCondition_Time.end(); it++) {

		Stage_Condition condition;
		condition.condition = it->get<picojson::object>()["TYPE"].get<std::string>();
		condition.value = (int)it->get<picojson::object>()["VALUE"].get<double>();

		if (condition.condition != "False")
		{
			status.condition_Time.push_back(condition);
		}
	}

	// エネミーのスポーンタイミングと種類、場所を得る
	picojson::object& enemy_span = val.get<picojson::object>()["ENEMY_SPAWNTIME"].get<picojson::object>();
	picojson::array& enemys = enemy_span["ENEMYS"].get<picojson::array>();

	for (picojson::array::iterator it = enemys.begin(); it != enemys.end(); it++) {

		Enemys_Spawn enemySpawn;
		enemySpawn.type = ChangeEnemy(it->get<picojson::object>()["TYPE"].get<std::string>());
		enemySpawn.spawnTime = (float)	it->get<picojson::object>()["SPAWNTIME"].get<double>();
		enemySpawn.direction = (int)	it->get<picojson::object>()["AZIMUTH"].get<double>();
		enemySpawn.remoteness =(int)	it->get<picojson::object>()["REMOTENESS"].get<double>();

		// 説明モード時のフラグ
		if (it->get<picojson::object>()["CONDITION"].get<std::string>() == "Explanation")
		{
			enemySpawn.condition = true;
		}

		status.enemys_Spawn.push_back(enemySpawn);

	}

	return status;

}

Stage_ClearData Json::FileLoad_StageClearData(const std::string filePath)
{
	//	読み込み用変数
	std::ifstream ifs;

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	//	読み込みチェック
	//	ifs変数にデータがなければエラー
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();
	Stage_ClearData status;

	//	読み込んだデータを構造体に代入
	// クリア条件
	// stageDataからMACHINE_SPAWNの配列の中身を得る
	picojson::object& clearData = val.get<picojson::object>()["ClearData"].get<picojson::object>();
	picojson::array& machines = clearData["MACHINES"].get<picojson::array>();

	// 要素分回す
	for (picojson::array::iterator it = machines.begin(); it != machines.end(); it++) {

		Stage_ClearMachine condition;

		condition.lv		= (int)it->get<picojson::object>()["LV"].get<double>();
		condition.type		= ChangeMachine(it->get<picojson::object>()["NAME"].get<std::string>());
		condition.element	= ChangeElement(it->get<picojson::object>()["ELEMENT"].get<std::string>());
		condition.number	= (int)it->get<picojson::object>()["NUMBER"].get<double>();

		status.machines.push_back(condition);

	}

	status.clearTime = (int)clearData["TIME"].get<double>();
	status.num		 = (int)clearData["NUM"].get<double>();

	return status;
}

// string形式をMACHINE_ELEMENTに変換
MACHINE_ELEMENT Json::ChangeElement(std::string element)
{
	MACHINE_ELEMENT elements = MACHINE_ELEMENT::NOMAL;

	if		(element == "Nomal") elements = MACHINE_ELEMENT::NOMAL;
	else if (element == "Flame") elements = MACHINE_ELEMENT::FLAME;
	else if (element == "Aqua")	 elements = MACHINE_ELEMENT::AQUA;
	else if (element == "Wind")	 elements = MACHINE_ELEMENT::WIND;
	else if (element == "Earth") elements = MACHINE_ELEMENT::EARTH;

	return elements;
}

std::string Json::ChangeElementString(const MACHINE_ELEMENT element)
{
	switch (element)
	{
	case NOMAL:
		return "Nomal";
	case FLAME:
		return "Flame";
	case AQUA:
		return "Aqua";
	case WIND:
		return "Wind";
	case EARTH:
		return "Earth";
	default:
		return "Nomal";
	}

}

// string形式をMACHINE_TYPEに変換
MACHINE_TYPE Json::ChangeMachine(const std::string machine)
{
	MACHINE_TYPE type = MACHINE_TYPE::NONE;

	if		(machine == "None")		type = MACHINE_TYPE::NONE;
	else if (machine == "Attacker")	type = MACHINE_TYPE::ATTACKER;
	else if (machine == "Upper")	type = MACHINE_TYPE::UPPER;
	else if (machine == "Defenser")	type = MACHINE_TYPE::DEFENSER;
	else if (machine == "Mining")	type = MACHINE_TYPE::MINING;
	else if (machine == "Recovery")	type = MACHINE_TYPE::RECOVERY;

	return type;
}

std::string Json::ChangeMachineString(const MACHINE_TYPE type)
{
	switch (type)
	{
	case NONE:
		return "None";
	case ATTACKER:
		return "Attacker";
	case UPPER:
		return "Upper";
	case DEFENSER:
		return "Defenser";
	case MINING:
		return "Mining";
	case RECOVERY:
		return "Recovery";
	default:
		return "None";
	}

}

ENEMY_TYPE Json::ChangeEnemy(const std::string machine)
{
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;

	if (machine == "None")			type = ENEMY_TYPE::ENMEY_NONE;
	else if (machine == "Slime")	type = ENEMY_TYPE::SLIME;
	else if (machine == "Worm")		type = ENEMY_TYPE::WORM;

	return type;
}

DirectX::SimpleMath::Color Json::ChangeColor(MACHINE_ELEMENT element)
{
	DirectX::SimpleMath::Color colors = COLOR_WIGHT;

	switch (element)
	{
	case NOMAL:
		return COLOR_WIGHT;
	case FLAME:
		return COLOR_RED;
	case AQUA:
		return COLOR_BLUE;
	case WIND:
		return COLOR_GREEN;
	case EARTH:
		return COLOR_YELLOW;
	default:
		break;
	}

	return colors;
}

void Json::WritingJsonFile_ClearData(int number,std::vector<std::shared_ptr<AlchemicalMachineObject>> alchemicalMachineList,int time)
{
	time;

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number;
	std::string filePath = "Resources/Json/ClearData/ClearData_" + oss.str() + ".json";

	//	読み込み用変数
	std::ifstream ifs;

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	//	読み込みチェック
	//	ifs変数にデータがなければエラー
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();

	// 書き換える情報をjsonから取得
	picojson::object& clearMachineObj = val.get<picojson::object>()["ClearData"].get<picojson::object>();
	picojson::array& clearMachineArr = clearMachineObj["MACHINES"].get<picojson::array>();

	//picojson::object& timeObj = val.get<picojson::object>()["TIME"].get<picojson::object>();

	// 初期化
	clearMachineArr.clear();

	for (int i = 0; i < alchemicalMachineList.size(); i++)
	{
		// 内容追加
		picojson::object id;
		id.insert(std::make_pair("LV", picojson::value((double)alchemicalMachineList[i]->GetLv())));
		id.insert(std::make_pair("NAME", picojson::value(alchemicalMachineList[i]->GetObjectName())));
		id.insert(std::make_pair("ELEMENT", picojson::value(ChangeElementString(alchemicalMachineList[i]->GetElement()))));
		id.insert(std::make_pair("NUMBER", picojson::value((double)i)));
		// 内容書き込み
		clearMachineArr.emplace_back(picojson::value(id));

	}

	std::ofstream ofs(filePath);

	ofs << picojson::value(val).serialize(true) << std::endl;
}
