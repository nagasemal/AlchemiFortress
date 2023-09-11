//--------------------------------------------------------------------------------------
// File: ScreenToWorld.h
//
// �X�N���[�����W�����[���h���W�ɕϊ����邽�߂̃N���X
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

SimpleMath::Vector3 CalcScreenToXZN(
	int sX,		// �X�N���[����X���W
	int sY,		// �X�N���[����Y���W
	int screen_W, // ����
	int screen_H, // ����
	SimpleMath::Matrix view,
	SimpleMath::Matrix prj
);

// �Ăяo���ۂ͂�����
SimpleMath::Vector3 CalcScreenToWorldN(
	int sX,		// �X�N���[����X���W
	int sY,		// �X�N���[����Y���W
	float fZ,	// �ˉe��Ԃł�Z�l(0~1)
	int screen_W, // ����
	int screen_H, // ����
	SimpleMath::Matrix view,
	SimpleMath::Matrix prj
);