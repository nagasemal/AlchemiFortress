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

enum MACHINE_ELEMENT : int
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
	UPEER = 2,		// �͈͓������^
	DEFENSER = 3,	// �h��^
	MINING = 4,		// �̌@�^
	RECOVERY = 5,	// ���͉���^

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,
	SLIME = 1,

	ENEMY_NUM
};