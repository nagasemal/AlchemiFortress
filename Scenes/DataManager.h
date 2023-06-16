//--------------------------------------------------------------------------------------
// File: DataManager.h
//
//�@���̃N���X���狤�ʂŎg���ϐ��̎󂯓n����S������V���O���g���N���X
//  �������́@���������@�|�����G�̐��@�ݒu���Ă���A���P�~�J���}�V���̑���/��ޕ�
// 
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class DataManager
{
public:
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;

	//static void Create();
	//static void Destroy();

	//static DataManager& GetInstance() { return *instance; }

	//	�C���X�^���X�̊m��
	static DataManager* GetInstance() { return instance; }

	//	�C���X�^���X�̐���
	static void Create() { if (instance == nullptr) instance = new DataManager; }

	//	�C���X�^���X�̍폜
	static void Destroy()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

// �Q�b�^�[�@�Z�b�^�[
public:

	//���͑���
	int GetNowMP()			const { return m_nowMP; }
	void SetNowMP(int value)		{ m_nowMP = value; }

	//��������
	int GetNowCrystal()		const { return m_nowCrystal; }
	void SetNowCrystal(int value)		{ m_nowCrystal = value; }

	//�G������
	int GetNowEnemyKill()		const { return m_nowEnemyKill; }
	void SetNowEnemyKill(int value)		{ m_nowEnemyKill = value; }

private:
	DataManager();
	static DataManager* instance;


	// ���݂̖��͑���
	int m_nowMP;

	// ���݂̌�������
	int m_nowCrystal;

	// ���݂̓G������
	int m_nowEnemyKill;


};