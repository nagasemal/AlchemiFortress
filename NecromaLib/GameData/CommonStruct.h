#pragma once

// Enemy�̃p�����[�^
struct EnemyParam
{
	// �ړ����x
	float speed;
	// �U����
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

// �L�[��񋓌^�Ƃ��Ď����Ă�����悤�ɂ���
enum MACHINE_TYPE : int
{
	NONE = 0,		// �w��Ȃ�
	ATTACKER = 1,	// �U���^
	DEFENSER = 2,	// �h��^
	UPPER = 3,		// �͈͓������^
	RECOVERY = 4,	// ���͉���^
	MINING = 5,		// �̌@�^

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,
	SLIME = 1,
	WORM = 2,

	ENEMY_NUM
};