//---------
// -----------------------------------------------------------------------------
// File: JsonLoder.h
//
// �W�F�C�\����ǂݏ�������N���X
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <string>

#include "NecromaLib/GameData/CommonStruct.h"

//	json�t�@�C������ǂ݂������f�[�^���i�[����\����
struct Bullet_Data
{
	std::string element = "Nomal";	//  ����
	float str = 0.0f;				//	�U����
	float speed = 0.0f;				//	�U���̑��x
	float span = 0.0f;				//�@�U���̊Ԋu
	float life = 0.0f;				//�@�e�̐�������
	int lossMp = 0;					//  ���炷����
};

struct Machine_Data
{
	std::string element = "Nomal";		// ����
	int lv					= 0;		// Lv
	int hp					= 0;		// Hp
	float multiplier_hp		= 0.0f;		// ���x���A�b�v����HP�̏㏸��
	float effect_rage		= 0.0f;		// ���ʔ͈�
	float multiplier_effect = 0.0f;		// ���x���A�b�v���̌��ʔ͈͂̏㏸��
	int alchemi_mp			= 0;		// �������Ɏg��MP�̗�
	int alchemi_crystal		= 0;		// �������Ɏg�����̗͂�
	int repea_crystal		= 0;		// �C�U���Ɏg���N���X�^���̗�
	int lvUp_crystal		= 0;		// ���x���A�b�v���Ɏg���N���X�^���̗�
	int dismantling_crystal = 0;		// �󂵂��ۂɓ�����N���X�^���̗�
	float effect_value		= 0.0f;		// ���ʒl
};

struct Enemy_MoveData
{
	std::string moveName;
	float delay;
	float time;
	float value;
};

struct Enemy_Data
{
	ELEMENT element = ELEMENT::NOMAL;
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;
	std::string moveType = "ALL";
	int hp = 1;
	float power = 1.0f;
	int exp = 1;

	std::vector<Enemy_MoveData> moveData;
};

// �X�e�[�W�̃N���A�������i�[
struct Stage_Condition
{
	std::string condition = "None";
	int value = 0;		//�ڕW�l
	int progress = 0;	//�i�s�x
};

struct Stage_Resource
{
	int mp;
	int crystal;
	int lv;
	int attacker;
	int upper;
	int deffencer;
	int mining;
	int recovery;
};

// �X�e�[�W���N���A�����}�V���̏����i�[
struct Stage_Machine
{
	MACHINE_TYPE type = MACHINE_TYPE::NONE;
	ELEMENT element = ELEMENT::NOMAL;
	int lv = 1;
	int number = 0;
};

// �G�l�~�[���o�������鎞��
struct Enemys_Spawn
{ 
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;
	float spawnTime = 0.0f;		// ��������܂ł̎���
	int direction = 1;			// �����ꏊ(1~8�̓�����k)
	int remoteness = 1;			// ����(���C��) 
	bool condition = false;		// �������J�n����^�C�~���O
};

struct Stage_Data
{
	std::vector<Stage_Condition> condition_Machine;			// �X�e�[�W�̃N���A���� (�}�V���z�u����)
	std::vector<Stage_Condition> condition_Enemy;			// �X�e�[�W�̃N���A���� (�G�l�~�[����)
	std::vector<Stage_Condition> condition_Time;			// �X�e�[�W�̃N���A���� (��������,�K�莞��)

	std::vector<Enemys_Spawn> enemys_Spawn;					// �G�l�~�[�̏o�����

	Stage_Resource resource;								// ���߂��珊�����Ă��郊�\�[�X�Q
	std::vector<Stage_Machine> machine;						// ���߂���ݒu����Ă���}�V���Q
};

struct Stage_ClearData
{
	std::vector<Stage_Machine> machines;	// �N���A�����}�V��
	int clearTime = 0;							// �N���A�܂łɂ�����������
	int num = 0;								// �����
};

// ��������
class AlchemicalMachineObject;
class ICommand_Enemy;

namespace Json
{
	// �ǂݍ��݁FAttacker���ˏo����e�̏��
	Bullet_Data FileLoad_BulletData(const std::string filePath);
	// �ǂݍ��݁F�}�V���̊�{�f�[�^
	Machine_Data FileLoad_MachineData(const std::string filePath);
	// �ǂݍ��݁F�G�l�~�[�̊�{�f�[�^
	Enemy_Data FileLoad_EnemyData(const std::string filePath);
	// �ǂݍ��݁F�X�e�[�W�̊�{�f�[�^
	Stage_Data FileLoad_StageData(const std::string filePath);
	// �ǂݍ��݁F�X�e�[�W�N���A���
	Stage_ClearData FileLoad_StageClearData(const std::string filePath);

	//  �����ϊ�
	ELEMENT ChangeElement(const std::string element);
	std::string ChangeElementString(const ELEMENT element);

	//�@�}�V���ϊ�
	MACHINE_TYPE ChangeMachine(const std::string machine);
	std::string ChangeMachineString(const MACHINE_TYPE type);

	//�@�G�l�~�[�ϊ�
	ENEMY_TYPE ChangeEnemy(const std::string enemyName);

	//  �F�ϊ�
	DirectX::SimpleMath::Color ChangeColor(ELEMENT element);

	void WritingJsonFile_ClearData(int number,std::vector<std::shared_ptr<AlchemicalMachineObject>> alchemicalMachineList,int time);

	// �G�f�B�^�[�Ɏg�p�@�X�e�[�W�̃f�[�^����������
	void WritingJsonFile_StageData(int number,Stage_Data stageData);
}