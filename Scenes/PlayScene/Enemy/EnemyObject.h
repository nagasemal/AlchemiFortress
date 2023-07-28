//--------------------------------------------------------------------------------------
// File: EnemyObject.h
//
//  �G�l�~�[�̓��������@�e�N���X
//  �X�e�[�^�X��ς��邱�ƂŎ�ނ���������
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "NecromaLib/GameData/CommonStruct.h"

class EnemyObject : public GameObjct3D
{
public:
	EnemyObject(ENEMY_TYPE type, DirectX::SimpleMath::Vector3 startPos,int lv);
	~EnemyObject();

	// ������
	void Initialize()					override;

	// �X�V
	void Update()						override;

	// �`��@(UI�֌W)
	void Draw()							override;

	void Render(GeometricPrimitive* geo);

	// �I������
	void Finalize()						override;

	// �v���C���[���_�ւƌ�����
	bool GotoTarget(DirectX::SimpleMath::Vector3 target);

	// �i�܂Ȃ��悤�ɂ��鏈��
	void HitMachine(bool flag);

	// �o�E���h������
	void Bouns();

// �A�N�Z�T
public:

	// �̗�
	void SetHp(int hp)									{ m_hp = hp; }

	// �X�s�[�h
	void SetSpeed(float speed)							{ m_speed = speed; }

	// ���x��
	const int GetLv()									{ return m_lv; }
	// �̗�
	const int GetHp()									{ return m_hp; }
	// EXP
	const int GetEXP()									{ return m_exp; }
	// �p���[	
	const float GetPower()								{ return m_power;}
	// �ړI�n�܂ł̋���
	const DirectX::SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// �����x
	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele;}
	// ���g�̃G�l�~�[�^�C�v
	const ENEMY_TYPE GetEnemyType()						{ return m_enemyType; }

	EnemyObject* GetEnemyObject()						{ return this;}

private:

	// �U����
	float m_power;
	// HP
	int m_hp;
	// ���݃��x��
	int m_lv;
	// �o���l
	int m_exp;
	// ���x
	float m_speed;
	// �ړ����~�߂�
	bool m_stopFlag;
	// ���g�̃^�C�v
	ENEMY_TYPE m_enemyType;

	// �����x
	DirectX::SimpleMath::Vector3 m_accele;
	// �ړI�n�܂ł̋���
	DirectX::SimpleMath::Vector3 m_lengthVec;

};