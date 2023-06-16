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

	// �I�����ɍX�V����鏈��
	void SelectUpdate()					override;

	// �`��@(UI�֌W)
	void Draw()							override;

	// �I������
	void Finalize()						override;

	void AllAlchemicalMachine(AlchemicalMachineObject* object)				override;

	bool BulletRequest(std::list<EnemyObject>* enemys)						override;

	void AllFieldObject(FieldObjectManager* fieldManager)					override;

	Bullet GetBulletData() override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	void LvUp() override;

	//�@�A�N�Z�T
public:

private:

};