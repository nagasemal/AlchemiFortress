#include "pch.h"
#include "JsonLoder.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include "picojson.h"

// 構造体用
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/Enemy/EnemyList/ICommand_Enemy.h"

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

Enemy_Data Json::FileLoad_EnemyData(const std::string filePath)
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

	Enemy_Data data;

	picojson::object status = val.get<picojson::object>()["Status"].get<picojson::object>();

	//	読み込んだデータを構造体に代入
	data.element = ChangeElement(status["ELEMENT"].get<std::string>());
	data.type = ChangeEnemy(status["TYPE"].get<std::string>());
	data.moveType = status["MOVETYPE"].get<std::string>();
	data.hp = (int)status["HP"].get<double>();
	data.exp = (int)status["EXP"].get<double>();
	data.power = (int)status["STR"].get<double>();

	picojson::array moves = status["MOVEING"].get<picojson::array>();

	// 要素分回す
	for (picojson::array::iterator it = moves.begin(); it != moves.end(); it++)
	{
		Enemy_MoveData param;

		param.moveName = it->get<picojson::object>()["TYPE"].get<std::string>();
		param.time = (float)it->get<picojson::object>()["TIME"].get<double>();
		param.delay = (float)it->get<picojson::object>()["DELAY"].get<double>();
		param.value = (float)it->get<picojson::object>()["VALUE"].get<double>();

		data.moveData.push_back(param);
	}

	return data;
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

	// 初めから設置されているマシンの情報を得る
	picojson::object& defaulet_machine = val.get<picojson::object>()["MACHINE_DEFAULET"].get<picojson::object>();
	picojson::array& machine = defaulet_machine["MACHINES"].get<picojson::array>();

	for (picojson::array::iterator it = machine.begin(); it != machine.end(); it++) {

		Stage_Machine stage_machine;
		stage_machine.lv = (int)it->get<picojson::object>()["LV"].get<double>();
		stage_machine.type = ChangeMachine(it->get<picojson::object>()["TYPE"].get<std::string>());
		stage_machine.element = ChangeElement(it->get<picojson::object>()["ELEMENT"].get<std::string>());
		stage_machine.number = (int)it->get<picojson::object>()["NUMBER"].get<double>();

		status.machine.push_back(stage_machine);

	}

	// ステージのResource群情報
	picojson::object& default_resource = val.get<picojson::object>()["RESOURCE_DEFAULET"].get<picojson::object>();
	Stage_Resource stage_resource;

	stage_resource.lv			= (int)default_resource["LV"].get<double>();
	stage_resource.mp			= (int)default_resource["MP"].get<double>();
	stage_resource.crystal		= (int)default_resource["CRYSTAL"].get<double>();
	stage_resource.attacker		= (int)default_resource["ATTACKER"].get<double>();
	stage_resource.upper		= (int)default_resource["UPPER"].get<double>();
	stage_resource.mining		= (int)default_resource["MINING"].get<double>();
	stage_resource.deffencer	= (int)default_resource["DEFFENCER"].get<double>();
	stage_resource.recovery		= (int)default_resource["RECOVERY"].get<double>();

	status.resource = stage_resource;

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

		Stage_Machine condition;

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
ELEMENT Json::ChangeElement(std::string element)
{
	ELEMENT elements = ELEMENT::NOMAL;

	if		(element == "Nomal") elements = ELEMENT::NOMAL;
	else if (element == "Flame") elements = ELEMENT::FLAME;
	else if (element == "Aqua")	 elements = ELEMENT::AQUA;
	else if (element == "Wind")	 elements = ELEMENT::WIND;
	else if (element == "Earth") elements = ELEMENT::EARTH;

	return elements;
}

std::string Json::ChangeElementString(const ELEMENT element)
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

DirectX::SimpleMath::Color Json::ChangeColor(ELEMENT element)
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

void Json::WritingJsonFile_StageData(int number, Stage_Data stageData)
{

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number;
	std::string filePath = "Resources/Json/StageData/StageData_" + oss.str() + ".json";

	//	読み込み用変数
	std::ifstream ifs;

	std::ifstream empty_file; // 空のフォーマット
	std::ofstream writing_file;// 書き込むファイル

	// ファイル生成(空のフォーマットをコピーする 読み取り専用)
	empty_file.open("Resources/Json/StageData/StageData_EMPTY.json", std::ios::in);
	writing_file.open(filePath, std::ios::out);

	std::string line;
	while (std::getline(empty_file, line)) {
		writing_file << line << std::endl;
	}

	//	ファイル読み込み
	ifs.open(filePath, std::ios::binary);

	empty_file.close();
	writing_file.close();

	//	読み込みチェック
	//	ifs変数にデータがなければエラーを吐く。上コードで生成しているため、エラーは基本吐かれない
	assert(ifs);

	//	Picojsonへ読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();

	// 書き換える情報をjsonから取得
	// ミッション内容：マシン
	{
		picojson::object& stageData_Machine = val.get<picojson::object>()["Conditions_Machine"].get<picojson::object>();
		picojson::array& machine_array = stageData_Machine["MACHINE_SPAWN"].get<picojson::array>();

		machine_array.clear();
		for (int i = 0; i < stageData.condition_Machine.size(); i++)
		{
			picojson::object id;
			id.insert(std::make_pair("TYPE", stageData.condition_Machine[i].condition));
			id.insert(std::make_pair("VALUE", (double)stageData.condition_Machine[i].value));

			machine_array.emplace_back(picojson::value(id));
		}
	}

	// ミッション内容：エネミー
	{
		picojson::object& stageData_enemy = val.get<picojson::object>()["Conditions_Enemy"].get<picojson::object>();
		picojson::array& enemy_array = stageData_enemy["ENEMY_KNOCKDOWN"].get<picojson::array>();
		enemy_array.clear();
		for (int i = 0; i < stageData.condition_Enemy.size(); i++)
		{
			picojson::object id;
			id.insert(std::make_pair("TYPE", stageData.condition_Enemy[i].condition));
			id.insert(std::make_pair("VALUE", (double)stageData.condition_Enemy[i].value));

			enemy_array.emplace_back(picojson::value(id));
		}
	}

	// ミッション内容：時間
	{
		picojson::object& stageData_Time = val.get<picojson::object>()["Conditions_Time"].get<picojson::object>();
		picojson::array& time_array = stageData_Time["TIME"].get<picojson::array>();
		time_array.clear();
		for (int i = 0; i < stageData.condition_Time.size(); i++)
		{
			picojson::object id;
			id.insert(std::make_pair("TYPE", stageData.condition_Time[i].condition));
			id.insert(std::make_pair("VALUE", (double)stageData.condition_Time[i].value));

			time_array.emplace_back(picojson::value(id));
		}
	}

	// 出現するエネミー情報
	{
		picojson::object& stageData_SpawnEnemy = val.get<picojson::object>()["ENEMY_SPAWNTIME"].get<picojson::object>();
		picojson::array& spawnEnemy_array = stageData_SpawnEnemy["ENEMYS"].get<picojson::array>();

		spawnEnemy_array.clear();
		for (int i = 0; i < stageData.enemys_Spawn.size(); i++)
		{
			picojson::object id;
			id.insert(std::make_pair("TYPE", stageData.enemys_Spawn[i].condition));
			id.insert(std::make_pair("AZIMUTH", (double)stageData.enemys_Spawn[i].direction));
			id.insert(std::make_pair("REMOTENESS", (double)stageData.enemys_Spawn[i].remoteness));
			id.insert(std::make_pair("SPAWNTIME", (double)stageData.enemys_Spawn[i].spawnTime));
			id.insert(std::make_pair("CONDITION", stageData.enemys_Spawn[i].condition));

			spawnEnemy_array.emplace_back(picojson::value(id));
		}

	}

	// 初めから設置されているマシン情報
	{
		// 書き換える情報をjsonから取得
		picojson::object& default_Machine = val.get<picojson::object>()["MACHINE_DEFAULET"].get<picojson::object>();
		picojson::array& defaultMachine_array = default_Machine["MACHINES"].get<picojson::array>();
		// 初期化
		defaultMachine_array.clear();

		for (int i = 0; i < stageData.machine.size(); i++)
		{
			// 内容追加
			picojson::object id;
			id.insert(std::make_pair("LV", picojson::value((double)stageData.machine[i].lv)));
			id.insert(std::make_pair("NAME", picojson::value(ChangeMachineString(stageData.machine[i].type))));
			id.insert(std::make_pair("ELEMENT", picojson::value(ChangeElementString(stageData.machine[i].element))));
			id.insert(std::make_pair("NUMBER", picojson::value((double)stageData.machine[i].number)));
			// 内容書き込み
			defaultMachine_array.emplace_back(picojson::value(id));

		}
	}

	// ステージのリソース群
	{
		// 書き換える情報をjsonから取得
		picojson::object& default_Resource = val.get<picojson::object>()["RESOURCE_DEFAULET"].get<picojson::object>();

		// 初期化
		default_Resource.clear();

		// 書き込み
		default_Resource.insert(std::make_pair("CRYSTAL", picojson::value((double)stageData.resource.crystal)));
		default_Resource.insert(std::make_pair("MP", picojson::value((double)stageData.resource.mp)));
		default_Resource.insert(std::make_pair("LV", picojson::value((double)stageData.resource.lv)));
		default_Resource.insert(std::make_pair("ATTACKER", picojson::value((double)stageData.resource.attacker)));
		default_Resource.insert(std::make_pair("UPPER", picojson::value((double)stageData.resource.upper)));
		default_Resource.insert(std::make_pair("DEFFENCER", picojson::value((double)stageData.resource.deffencer)));
		default_Resource.insert(std::make_pair("MINING", picojson::value((double)stageData.resource.mining)));
		default_Resource.insert(std::make_pair("RECOVERY", picojson::value((double)stageData.resource.recovery)));
	}

	std::ofstream ofs(filePath);

	ofs << picojson::value(val).serialize(true) << std::endl;
}
