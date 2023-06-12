//--------------------------------------------------------------------------------------
// File: AlchemicalMachineFilter.h
//
//�@�󂯎���������ɉ����ă��f����N���X��n���N���X
// 
// Date: 2023.6.5
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

class AlchemicalMachineFilter
{
public:
	AlchemicalMachineFilter();
	~AlchemicalMachineFilter();

	/// <summary>
    /// �}�V���^�C�v�ɉ����ă��f����n��
    /// </summary>
    /// <param name="type">�A���P�~�J���}�V���̃^�C�v</param>
    /// <returns></returns>
	DirectX::Model* HandOverAMModel(AlchemicalMachineObject::MACHINE_TYPE type);

	/// <summary>
	/// �}�V���^�C�v�ɉ�����AlchemicalMachineObject���p�������N���X��n��
	/// </summary>
	/// <param name="type">�A���P�~�J���}�V���̃^�C�v</param>
	/// <returns></returns>
	AlchemicalMachineObject* HandOverAMClass(AlchemicalMachineObject::MACHINE_TYPE type);

private:

	std::unique_ptr<DirectX::Model> m_model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

};