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
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/BulletDataBase.h"
#include <list>


class AM_Attacker : public AlchemicalMachineObject
{
public:

	AM_Attacker();
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

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	//  ===LvUp����===
	// �U���́@�@
    // �e���@�@�@
    // �򋗗��@�@
    // ����́@
	// ���ˊԊu�@
	// ���ʔ͈́@
	void LvUp() override;

	// �S�ẴA���P�~�J���}�V���̏����󂯎��
	void AllAlchemicalMachine(AlchemicalMachineObject* object);

	// �ˏo�̃��N�G�X�g���o��
	bool BulletRequest(std::list<EnemyObject>* enemys);

	// 
	Bullet GetBulletData();

//�@�A�N�Z�T
public:

	BulletStatus GetBulletStatus() { return m_bulletStatus;}

	int GetNextLvCrystal()			{ return (15 * m_lv) * (5 * m_lv); }

private:



private:

	float	m_timer;

	BulletStatus m_bulletStatus;		// �o���b�g�f�[�^
	BulletStatus m_saveBulletStatus;	// ���H���̃o���b�g�f�[�^

	DirectX::SimpleMath::Vector3 m_targetPos;

	//std::list<Bullet> m_bullets;

	// �I���\�I�u�W�F�N�g
	std::unique_ptr<SelectionBox> m_selectBox[4];
};