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
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include <list>


class AM_Attacker : public AlchemicalMachineObject
{
public:

	AM_Attacker(MACHINE_ELEMENT element);
	~AM_Attacker();

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

	void AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)			override;

	bool BulletRequest(std::list<EnemyObject>* enemys)		override;

	Bullet GetBulletData()				override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

//�@�A�N�Z�T
public:


private:

	float	m_timer;
	float	m_speed;		// �U���̑��x
	float	m_power;		// �U���̈З�
	float	m_bulletlife;	// �e�̖�

	DirectX::SimpleMath::Vector3 m_targetPos;
	
	MACHINE_ELEMENT m_element;

	//std::list<Bullet> m_bullets;

	// �I���\�I�u�W�F�N�g
	std::unique_ptr<SelectionBox> m_selectBox[4];
};