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

// �~�b�V�����̎��
enum MISSION_TYPE : int
{
	SPAWN,		// �}�V���̐���
	ALCHEMI,	// �}�V���̘B��
	LVUP,		// �}�V���̃��x���A�b�v
	REPAIR,		// �}�V���̏C�U
	DESTROY,	// �}�V���̔j��
	ENEMY_KILL,		// �G�l�~�[�̓���
	BASELV,		// ���_���x��
	RESOURCE,	// ���\�[�X����ʎ擾
	TIMER,		// ����

	MISSION_NUM

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