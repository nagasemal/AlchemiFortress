//--------------------------------------------------------------------------------------
// File: AlchemicalMachineManager.h
//
//�@�v���C���[���t�B�[���h���ɐݒu����I�u�W�F�N�g�Q�̃}�l�[�W���[
// 
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <map>
#include <string>
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/UI/MachineExplanation.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "AlchemicalMachineFilter.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void Initialize();

	void Update(bool hitFiledToMouse,bool hitBaseToMouse,MousePointer* pMP, std::list<EnemyObject> enemys);

	// �e�ۂ̏���
	void MajicBulletUpdate(int index, std::list<EnemyObject> enemys);

	void Render();

	void DrawUI();

	void Finalize();


public:

	static const int AM_MAXNUM = 256;

//�A�N�Z�T
public:

	bool GetHitMouseFlag(int number)									{ return m_AMObject[number]->GetHitMouse();}

	AlchemicalMachineObject* GetAlchemicalMachineObject()				{ return m_AMObject->get(); }
	AlchemicalMachineObject* GetAlchemicalMachineObject(int index)		{ return m_AMObject[index].get();}

	std::list<Bullet>* GetBullet()										{ return m_bullets.get();}

private:

	// ��
	void MovingMachine(int number);

private:

	std::unique_ptr<MachineSelectManager> m_selectManager;

	//std::unique_ptr<DirectX::Model> m_Model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// �A���P�~�J���}�V���m��
	std::unique_ptr<AlchemicalMachineObject> m_AMObject[AM_MAXNUM];

	// �A���P�~�J���}�V��(�A�^�b�N�^�C�v)����ˏo�����e�̃��X�g
	std::unique_ptr<std::list<Bullet>>	m_bullets;

	// �A���P�~�J���}�V�����f���̎󂯓n��
	std::unique_ptr<AlchemicalMachineFilter> m_AMFilter;

	bool m_allHitObjectToMouse;

	// ���ݑI������Ă���I�u�W�F�N�g�̔ԍ�
	int m_selectNumber;

	//===��ŏ����e�X�g�p�ϐ�
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  ���u���e���f��

};