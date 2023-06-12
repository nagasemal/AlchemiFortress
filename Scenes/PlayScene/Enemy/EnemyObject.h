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

class EnemyObject : public GameObjct3D
{
public:

	enum EnemyType : int
	{
		NONE = 0,
		

	};

public:
	EnemyObject(EnemyType type, DirectX::SimpleMath::Vector3 startPos,int lv);
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
	// �p���[	
	const float GetPower()								{ return m_power;}
	// �ړI�n�܂ł̋���
	const DirectX::SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// �����x
	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele;}

	EnemyObject* GetEnemyObject()						{ return this;}

private:

	float m_power;
	int m_hp;
	int m_lv;
	// ���x
	float m_speed;
	// �����x
	DirectX::SimpleMath::Vector3 m_accele;
	// �ړI�n�܂ł̋���
	DirectX::SimpleMath::Vector3 m_lengthVec;

};