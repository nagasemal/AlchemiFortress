//--------------------------------------------------------------------------------------
// File: AlchemicalMachineManager.h
//
//�@�v���C���[���t�B�[���h���ɐݒu����I�u�W�F�N�g�Q�̃}�l�[�W���[
//  ���f���̃��[�h��l�̎󂯓n����S��
// 
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <string>
#include "AM_Attacker.h"
#include "AM_None.h"
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/UI/MachineExplanation.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void ModeLoader();

	void Initialize();

	void Update(bool hitFiledToMouse,bool hitBaseToMouse,MousePointer* pMP);

	void Render();

	void DrawUI();

	void Finalize();


public:

	static const int AM_MAXNUM = 256;

//�A�N�Z�T
public:

	bool GetHitMouseFlag(int number) { return m_AMObject[number]->GetHitMouse();}

private:

	// ��
	void MovingMachine(int number);

private:

	std::unique_ptr<MachineSelectManager> m_selectManager;

	std::unique_ptr<DirectX::Model> m_Model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// �A���P�~�J���}�V���m��
	std::unique_ptr<AlchemicalMachineObject> m_AMObject[AM_MAXNUM];

	bool m_allHitObjectToMouse;

	// ���ݑI������Ă���I�u�W�F�N�g�̔ԍ�
	int m_selectNumber;

};