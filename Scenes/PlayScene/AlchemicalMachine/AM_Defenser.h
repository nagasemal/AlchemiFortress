//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//�@�G�l�~�[����̍U�����󂯎~�߂�A���P�~�J���}�V��
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Defenser : public AlchemicalMachineObject
{
public:
	AM_Defenser();
	~AM_Defenser();

	// ������
	void Initialize()					override;

	// �X�V
	void Update()						override;

	// �`��@(UI�֌W)
	void Draw()							override;

	// �I������
	void Finalize()						override;

	void AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)			override;

	bool BulletRequest(std::list<EnemyObject>* enemys) override;

	Bullet GetBulletData() override;

	//�@�A�N�Z�T
public:

private:

};