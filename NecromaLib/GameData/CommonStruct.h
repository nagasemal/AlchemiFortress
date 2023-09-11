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

// ƒL[‚ð—ñ‹“Œ^‚Æ‚µ‚ÄŽ‚Á‚Ä‚¨‚¯‚é‚æ‚¤‚É‚·‚é
enum MACHINE_TYPE : int
{
	NONE = 0,		// Žw’è‚È‚µ
	ATTACKER = 1,	// UŒ‚Œ^
	DEFENSER = 2,	// –hŒäŒ^
	UPPER = 3,		// ”ÍˆÍ“à‹­‰»Œ^
	RECOVERY = 4,	// –‚—Í‰ñŽûŒ^
	MINING = 5,		// ÌŒ@Œ^

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,
	SLIME = 1,
	WORM = 2,

	ENEMY_NUM
};