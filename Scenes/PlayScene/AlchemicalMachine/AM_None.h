//--------------------------------------------------------------------------------------
// File: AM_None.h
//
//�@���ʖ����̃A���P�~�J���}�V��
//  ����`�Ŏg��
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_None : public AlchemicalMachineObject
{
public:
	AM_None();
	~AM_None();

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