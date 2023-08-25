//--------------------------------------------------------------------------------------
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

// �X�e�[�W�̃N���A�������i�[
struct Stage_Condition
{
	std::string condition = "None";
	int value = 0;		//�ڕW�l
	int progress = 0;	//�i�s�x
};

// �X�e�[�W���N���A�����}�V���̏����i�[
struct Stage_ClearMachine
{
	MACHINE_TYPE type = MACHINE_TYPE::NONE;
	MACHINE_ELEMENT element = MACHINE_ELEMENT::NOMAL;
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

};

struct Stage_ClearData
{
	std::vector<Stage_ClearMachine> machines;	// �N���A�����}�V��
	int clearTime = 0;							// �N���A�܂łɂ�����������
	int num = 0;								// �����
};

// ��������
class AlchemicalMachineObject;

namespace Json
{
	Bullet_Data FileLoad_BulletData(const std::string filePath);

	Machine_Data FileLoad_MachineData(const std::string filePath);

	Stage_Data FileLoad_StageData(const std::string filePath);

	Stage_ClearData FileLoad_StageClearData(const std::string filePath);

	//  �����ϊ�
	MACHINE_ELEMENT ChangeElement(const std::string element);
	std::string ChangeElementString(const MACHINE_ELEMENT element);

	//�@�}�V���ϊ�
	MACHINE_TYPE ChangeMachine(const std::string machine);
	std::string ChangeMachineString(const MACHINE_TYPE type);

	//�@�G�l�~�[�ϊ�
	ENEMY_TYPE ChangeEnemy(const std::string machine);

	//  �F�ϊ�
	DirectX::SimpleMath::Color ChangeColor(MACHINE_ELEMENT element);

	void WritingJsonFile_ClearData(int number,std::vector<std::shared_ptr<AlchemicalMachineObject>> alchemicalMachineList,int time);
}