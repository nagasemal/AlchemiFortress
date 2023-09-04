//--------------------------------------------------------------------------------------
// File: ShareJsonData.h
//
// json�œǂݍ��񂾐��l��n��
//
// ���e��
// �E�o���b�g�̃p�����[�^
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

#include "NecromaLib/GameData/JsonLoder.h"

class ShareJsonData
{
public:
	~ShareJsonData() = default;

	ShareJsonData(const ShareJsonData&) = delete;
	ShareJsonData& operator=(const ShareJsonData&) = delete;

	static void Create();
	static void Destroy();

	static ShareJsonData& GetInstance() { return *instance; }

	// �o���b�g��json�t�@�C����ǂݍ���
	void LoadingJsonFile_Bullet();

	// �}�V����json�t�@�C����ǂݍ���
	void LoadingJsonFile_Machine();

	// �G�l�~�[��json�t�@�C����ǂݍ���
	void LoadingJsonFile_Enemy();

	// �X�e�[�W��json�t�@�C����ǂݍ���
	void LoadingJsonFile_Stage(int number);

	// �N���A�f�[�^��json�t�@�C����ǂݍ���
	void LoadingJsonFile_ClearData(int number);

public:

	// �����ɑΉ������o���b�g�̃p�����[�^��n��
	const Bullet_Data GetBulletData(ELEMENT element);

	// �}�V���^�C�v�ɑΉ������}�V���̃p�����[�^��n��
	const Machine_Data GetMachineData(MACHINE_TYPE type);

	// �G�l�~�[�^�C�v�ɑΉ������G�l�~�[�̃p�����[�^��n��
	const Enemy_Data GetEnemyData(ENEMY_TYPE type);

	// �X�e�[�W�̃f�[�^��n��
	const Stage_Data GetStageData();

	// �X�e�[�W�̃N���A�f�[�^��n��
	const Stage_ClearData GetClearData();

	// �X�e�[�W�f�[�^�̒��g��S�ď���(�������̈�̈�����h������)
	void StageDataCleanUP();

	// �G�������g�ɕR�Â��F�����擾
	DirectX::SimpleMath::Color GetElementColor(ELEMENT element);


private:
	ShareJsonData();
	static ShareJsonData* instance;

	// Attacker���J��o���e�̃p�����[�^
	Bullet_Data m_bulletData[ELEMENT::Num];

	// �e�}�V���̃f�[�^
	Machine_Data m_machineData[MACHINE_TYPE::NUM];

	// �e�G�l�~�[�̃f�[�^
	Enemy_Data m_enemyData[ENEMY_TYPE::ENEMY_NUM];

	// �ǂݍ��񂾃X�e�[�W�̃f�[�^(�S�Ă�ǂݍ��ނƃ����������������ׁA�����ǂݍ��ޕ������̗p)
	Stage_Data m_stageData;

	// �ǂݍ��񂾃X�e�[�W�̃N���A���f�[�^
	Stage_ClearData m_clearData;

};