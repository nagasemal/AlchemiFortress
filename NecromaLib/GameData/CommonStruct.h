#pragma once

enum ELEMENT : int
{
	NOMAL = 0,
	FLAME = 1,
	AQUA = 2,
	WIND = 3,
	EARTH = 4,
	Num
};

// ミッションの種類
enum MISSION_TYPE : int
{
	SPAWN,		// マシンの生成
	ALCHEMI,	// マシンの錬金
	LVUP,		// マシンのレベルアップ
	REPAIR,		// マシンの修繕
	DESTROY,	// マシンの破壊
	ENEMY_KILL,		// エネミーの討伐
	BASELV,		// 拠点レベル
	RESOURCE,	// リソース特定量取得
	TIMER,		// 時間

	MISSION_NUM

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

	SLIME	= 1,
	WORM	= 2,
	HOP		= 3,
	RETREAT = 4,

	ENEMY_NUM
};

enum OBJECT_TAG
{
	MAHINE		= 0,
	SPAWN_ENEMY = 1,
	ENEMY		= 2,
	CRYSTAL		= 3,



};