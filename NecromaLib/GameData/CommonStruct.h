#pragma once

// Enemyのパラメータ
struct EnemyParam
{
	// 移動速度
	float speed;
	// 攻撃力
	float power;
	// 

};

enum MACHINE_ELEMENT : int
{
	NOMAL = 0,
	FLAME = 1,
	AQUA = 2,
	WIND = 3,
	EARTH = 4,
	Num
};

// キーを列挙型として持っておけるようにする
enum MACHINE_TYPE : int
{
	NONE = 0,		// 指定なし
	ATTACKER = 1,	// 攻撃型
	UPEER = 2,		// 範囲内強化型
	DEFENSER = 3,	// 防御型
	MINING = 4,		// 採掘型
	RECOVERY = 5,	// 魔力回収型

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,
	SLIME = 1,

	ENEMY_NUM
};