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

enum ELEMENT : int
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
	DEFENSER = 2,	// 防御型
	UPPER = 3,		// 範囲内強化型
	RECOVERY = 4,	// 魔力回収型
	MINING = 5,		// 採掘型

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,
	SLIME = 1,
	WORM = 2,

	ENEMY_NUM
};