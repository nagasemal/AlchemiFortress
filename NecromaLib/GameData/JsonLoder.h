//---------
// -----------------------------------------------------------------------------
// File: JsonLoder.h
//
// ジェイソンを読み書きするクラス
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <string>

#include "NecromaLib/GameData/CommonStruct.h"

//	jsonファイルから読みだしたデータを格納する構造体
struct Bullet_Data
{
	std::string element = "Nomal";	//  属性
	float str			= 0.0f;		//	攻撃力
	float speed			= 0.0f;		//	攻撃の速度
	float span			= 0.0f;		//　攻撃の間隔
	float life			= 0.0f;		//　弾の生存時間
	int lossMp			= 0;		//  減らす魔力
};

struct Machine_Data
{
	std::string element = "Nomal";		// 属性
	int lv					= 0;		// Lv
	int hp					= 0;		// Hp
	float multiplier_hp		= 0.0f;		// レベルアップ時のHPの上昇幅
	float effect_rage		= 0.0f;		// 効果範囲
	float multiplier_effect = 0.0f;		// レベルアップ時の効果範囲の上昇幅
	int alchemi_mp			= 0;		// 製造時に使うMPの量
	int alchemi_crystal		= 0;		// 製造時に使う魔力の量
	int repea_crystal		= 0;		// 修繕時に使うクリスタルの量
	int lvUp_crystal		= 0;		// レベルアップ時に使うクリスタルの量
	int dismantling_crystal = 0;		// 壊した際に得られるクリスタルの量
	float effect_value		= 0.0f;		// 効果値
};

struct Enemy_MoveData
{
	std::string moveName		= "Standerd";
	float delay					= 0.0f;
	float time					= 0.0f;
	float value					= 0.0f;
};

struct Enemy_Data
{
	ELEMENT element				= ELEMENT::NOMAL;
	ENEMY_TYPE type				= ENEMY_TYPE::ENMEY_NONE;
	std::string moveType		= "ALL";
	int hp						= 1;
	float power					= 1.0f;
	int exp						= 1;
	SimpleMath::Color color		= SimpleMath::Color();

	std::vector<Enemy_MoveData> moveData;
};

// ステージのクリア条件を格納
struct Stage_Condition
{
	std::string condition	= "None";
	int value				= 0;		//目標値
	int progress			= 0;		//進行度
};

struct Stage_Resource
{
	int mp			= 400;
	int crystal		= 215;
	int hp			= 50;
	int lv			= 1;
	int attacker	= 0;
	int upper		= 0;
	int deffencer	= 0;
	int mining		= 0;
	int recovery	= 0;
};

// ステージをクリアしたマシンの情報を格納
struct Stage_Machine
{
	MACHINE_TYPE type	= MACHINE_TYPE::NONE;
	ELEMENT element		= ELEMENT::NOMAL;
	int lv				= 1;
	int number			= 0;
};

// エネミーを出現させる時間
struct Enemys_Spawn
{ 
	ENEMY_TYPE type					= ENEMY_TYPE::ENMEY_NONE;	// エネミーのタイプ
	float spawnTime					= 0.0f;						// 召喚するまでの時間
	SimpleMath::Vector3 spawnPos	= SimpleMath::Vector3();	// 召喚場所
	bool condition					= false;					// 召喚を開始するタイミング
};

struct Stage_Data
{
	std::vector<Stage_Condition>		condition_Machine;			// ステージのクリア条件 (マシン配置条件)
	std::vector<Stage_Condition>		condition_Alchemi;			// ステージのクリア条件 (マシン錬金条件)
	std::vector<Stage_Condition>		condition_Enemy;			// ステージのクリア条件 (エネミー討伐)
	std::vector<Stage_Condition>		condition_BaseLv;			// ステージのクリア条件 (拠点のLV条件)
	std::vector<Stage_Condition>		condition_Time;				// ステージのクリア条件 (生存時間,規定時間)

	std::vector<Enemys_Spawn>			enemys_Spawn;				// エネミーの出現情報

	Stage_Resource						resource;					// 初めから所持しているリソース群
	std::vector<Stage_Machine>			machine;					// 初めから設置されているマシン群

	std::vector<SimpleMath::Vector2>	crystalPos;					// クリスタルの番号

	std::vector<int>					tutorial;					// チュートリアル番号
	bool								lastWave		= true;		// 次にステージが控えているか
	
};

struct Stage_ClearData
{
	std::vector<Stage_Machine> machines;		// クリアしたマシン
	int clearTime	= 0;						// クリアまでにかかった時間
	int num			= 0;						// 挑戦回数
};

// 書き込み
class AlchemicalMachineObject;
class ICommand_Enemy;

namespace Json
{
	// 読み込み：Attackerが射出する弾の情報
	Bullet_Data FileLoad_BulletData(const std::string filePath);
	// 読み込み：マシンの基本データ
	Machine_Data FileLoad_MachineData(const std::string filePath);
	// 読み込み：エネミーの基本データ
	Enemy_Data FileLoad_EnemyData(const std::string filePath);
	// 読み込み：ステージの基本データ
	Stage_Data FileLoad_StageData(const std::string filePath);
	// 読み込み：ステージクリア情報
	Stage_ClearData FileLoad_StageClearData(const std::string filePath);

	//  属性変換
	ELEMENT ChangeElement(const std::string element);
	std::string ChangeElementString(const ELEMENT element);

	//　マシン変換
	MACHINE_TYPE ChangeMachine(const std::string machine);
	std::string ChangeMachineString(const MACHINE_TYPE type);

	//　エネミー変換
	ENEMY_TYPE ChangeEnemy(const std::string enemy_Name);
	std::string ChangeEnemyString(const ENEMY_TYPE enemy_Type);

	//  色変換
	SimpleMath::Color ChangeColor(ELEMENT element);

	void WritingJsonFile_ClearData(int number,std::vector<std::shared_ptr<AlchemicalMachineObject>> alchemicalMachineList,int time);

	// エディターに使用　ステージのデータを書き込む
	void WritingJsonFile_StageData(int number,Stage_Data stageData);

	// ステージのクリアデータを初期化する
	void InitializationClearStageData();
}