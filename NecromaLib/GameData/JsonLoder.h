//--------------------------------------------------------------------------------------
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
	float str = 0.0f;				//	攻撃力
	float speed = 0.0f;				//	攻撃の速度
	float span = 0.0f;				//　攻撃の間隔
	float life = 0.0f;				//　弾の生存時間
	int lossMp = 0;					//  減らす魔力
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

// ステージのクリア条件を格納
struct Stage_Condition
{
	std::string condition = "None";
	int value = 0;		//目標値
	int progress = 0;	//進行度
};

// ステージをクリアしたマシンの情報を格納
struct Stage_ClearMachine
{
	MACHINE_TYPE type = MACHINE_TYPE::NONE;
	int lv = 1;
	int number = 0;
};

// エネミーを出現させる時間
struct Enemys_Spawn
{ 
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;
	float spawnTime = 0.0f;		// 召喚するまでの時間
	int direction = 1;			// 召喚場所(1~8の東西南北)
	int remoteness = 1;			// 遠さ(ライン) 
	bool condition = false;		// 召喚を開始するタイミング
};

struct Stage_Data
{
	std::vector<Stage_Condition> condition_Machine;			// ステージのクリア条件 (マシン配置条件)
	std::vector<Stage_Condition> condition_Enemy;			// ステージのクリア条件 (エネミー討伐)
	std::vector<Stage_Condition> condition_Time;			// ステージのクリア条件 (生存時間,規定時間)

	std::vector<Enemys_Spawn> enemys_Spawn;					// エネミーの出現情報

	std::vector<Stage_ClearMachine> clearMachine;			// クリアした際のマシンの情報

};


namespace Json
{
	Bullet_Data FileLoad_BulletData(const std::string filePath);

	Machine_Data FileLoad_MachineData(const std::string filePath);

	Stage_Data FileLoad_StageData(const std::string filePath);

	//  属性変換
	MACHINE_ELEMENT ChangeElement(const std::string element);

	//　マシン変換
	MACHINE_TYPE ChangeMachine(const std::string machine);
	std::string ChangeMachineString(const MACHINE_TYPE type);

	//　エネミー変換
	ENEMY_TYPE ChangeEnemy(const std::string machine);

	//  色変換
	DirectX::SimpleMath::Color ChangeColor(MACHINE_ELEMENT element);

}