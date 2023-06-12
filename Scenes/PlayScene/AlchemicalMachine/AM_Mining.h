//--------------------------------------------------------------------------------------
// File: AM_Mining.h
//
//�@�͈͓��̍z���ɑ΂���
//  ���ʂ𔭊�����
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Mining : public AlchemicalMachineObject
{
public:
	AM_Mining();
	~AM_Mining();

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
public:

private:

};