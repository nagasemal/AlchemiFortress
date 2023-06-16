//--------------------------------------------------------------------------------------
// File: PlayerBase.h
//
// �Q�[�����̋��_(�h�q�Ώ�)�ƂȂ�I�u�W�F�N�g
//  
// 
// Date: 2023.5.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

class PlayerBase : public GameObjct3D
{

public:
	PlayerBase();
	~PlayerBase();

	// ������
	void Initialize()	override;

	// �X�V
	void Update()		override;

	// �`��
	void Draw()			override;

	// �I������
	void Finalize()		override;

 /*===[�@�A�N�Z�T�@]===*/
public:

	int			GetBaseLv()		const { return m_baseLv;}
	float		GetHP()			const { return m_hp; }
	float		GetExp()		const { return m_exp; }
	bool		GetHitMouse()	const { return m_hitMouseFlag; }

	void		PulsExp(float pulsExp)	{ m_exp += pulsExp;}
	void		Damage(float damage)	{ m_hp  -= damage;  }

private:

	int			m_baseLv;				//  ���_Lv
	float		m_hp;					//  ���_Hp
	float		m_exp;					//  ���݂̌o���l��
	bool		m_hitMouseFlag;			//  �}�E�X���������Ă��邩�ۂ�

	std::unique_ptr<DirectX::Model> m_baseModel;	//  ���_��3D���f��

	//===��ŏ����e�X�g�p�ϐ�
	std::unique_ptr<GeometricPrimitive> m_testBox;	//  ���u���l�p���f��

};