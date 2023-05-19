//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//�@�G�l�~�[���U������A���P�~�J���}�V��
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Attacker : public AlchemicalMachineObject
{
public:
	AM_Attacker();
	~AM_Attacker();

	// ������
	void Initialize()					override;

	// �X�V
	void Update()						override;

	// �`��@(UI�֌W)
	void Draw()							override;

	// �I������
	void Finalize()						override;

//�@�A�N�Z�T
public:


private:

	float	m_speed;		// �U���̑��x

};