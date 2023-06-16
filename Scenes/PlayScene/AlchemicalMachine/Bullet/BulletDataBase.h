//--------------------------------------------------------------------------------------
// File: BulletDataBase.h
//
//�@�o���b�g�̃X�e�[�^�X��S�ĕێ����Ă���
//  LvUp�ɂ��Čv�Z���@Lv5���ő�
// 
// Date: 2023.6.16
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

	// �ڍׂȐݒ�
	struct BulletStatus
	{
		float damage;
		float life;
		float speed;
		float span;		// �U���Ԋu
		float lossMp;	// �����
	};

	// �����ɉ������e�̌��X�e�[�^�X��Ԃ�
	BulletStatus GetBaseBulletStatus(AlchemicalMachineObject::MACHINE_ELEMENT element);

	// �X�e�[�^�X�̍Čv�Z
	BulletStatus RecalculationStatus(AlchemicalMachineObject::MACHINE_ELEMENT element,int lv);