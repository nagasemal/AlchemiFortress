//--------------------------------------------------------------------------------------
// File: FieldObjectManager.h
//
//�@�t�B�[���h��ɐݒu������]���Ȃ��I�u�W�F�N�g�̊Ǘ�
//
//  
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/Field/Field.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"

// ToDo�ꏊ�ς��K�{
#include "Scenes/PlayScene/AlchemicalMachine/PlayerBase.h"
#include <list>

class FieldObjectManager
{
public:
	FieldObjectManager();
	~FieldObjectManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

public:

	// �����̐���
	void CrystalSpawn(int num);

	Field* GetFieldObject()						{ return m_field.get();}
	PlayerBase* GetPlayerBase()					{ return m_playerBase.get(); }
	std::list<Crystal>* GetCrystalObject()		{ return m_crystals.get();}

private:

	// �����f��
	std::unique_ptr<DirectX::Model> m_floorModel;
	// �������f��
	std::unique_ptr<DirectX::Model> m_crystalModel;

	// �t�B�[���h�I�u�W�F�N�g
	std::unique_ptr<Field> m_field;

	// �v���C���[�̋��_
	std::unique_ptr<PlayerBase> m_playerBase;

	// �����z��
	std::unique_ptr<std::list<Crystal>> m_crystals;

};