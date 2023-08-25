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

	// �X�e�[�W��json�t�@�C����ǂݍ���
	void LoadingJsonFile_Stage(int number);

	// �N���A�f�[�^��json�t�@�C����ǂݍ���
	void LoadingJsonFile_ClearData(int number);

public:

	// �����ɑΉ������o���b�g�̃p�����[�^��n��
	const Bullet_Data GetBulletData(MACHINE_ELEMENT element);

	// �}�V���^�C�v�ɑΉ������}�V���̃p�����[�^��n��
	const Machine_Data GetMachineData(MACHINE_TYPE type);

	// �X�e�[�W�̃f�[�^��n��
	const Stage_Data GetStageData();

	// �X�e�[�W�̃N���A�f�[�^��n��
	const Stage_ClearData GetClearData();

	// �X�e�[�W�f�[�^�̒��g��S�ď���(�������̈�̈�����h������)
	void StageDataCleanUP();

	DirectX::SimpleMath::Color GetElementColor(MACHINE_ELEMENT element);


private:
	ShareJsonData();
	static ShareJsonData* instance;

	Bullet_Data m_bulletData[MACHINE_ELEMENT::Num];

	Machine_Data m_machineData[MACHINE_TYPE::NUM];

	Stage_Data m_stageData;

	Stage_ClearData m_clearData;

};