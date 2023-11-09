//--------------------------------------------------------------------------------------
// File		: MachineSelect.h
//
// �I�����ꂽ�ۂɕ\������g�p���\�[�X�̕\���Ɛ����{�^���̏������s��
// 
// �����{�^�� �N���b�N �� MachineSelectManager �� AlchemicalMachineManager��m_AMnums[]��1���₷;
// 
// Use		: MachineSelectManager
// Date		: 2023.5.18
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "SelectionBox.h"
#include "Scenes/PlayScene/UI/Number.h"

class MachineSelect : public GameObjct2D
{
public:
	MachineSelect();
	~MachineSelect();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	// ���f����`�悷�邽�߂̊֐�
	void DisplayObject();

public:

	// �����{�^�����������u�Ԃ��擾
	bool GetManufacturingFlag()										{ return m_manufacturingFlag; }

	bool GetHitMouseFlag()											{ return m_hitMouseFlag; }
	void SetHitMouseFlag(bool flag)									{ m_hitMouseFlag = flag; }

	void SetPosition(SimpleMath::Vector2 pos)						{ m_data.pos = pos; }

	void SetMachineType(MACHINE_TYPE type)							{ m_selectMachineType = type;}
	MACHINE_TYPE GetMachineType()									{ return m_selectMachineType;}

	SelectionBox* GetMachineBox()									{ return m_machineBox.get();}

	void SetChangeColorFlag(bool flag)								{ m_changeColorFlag = flag;}

	// ����UI���K��ʒu�ɑ��݂��邩�ۂ�
	bool SelectUIFlag();

private:

	bool m_onMouseFlag;
	bool m_hitMouseFlag;
	bool m_manufacturingFlag;

	MACHINE_TYPE m_selectMachineType;

	// �I���{�b�N�X(�}�V��)
	std::unique_ptr<SelectionBox> m_machineBox;

	// �{�b�N�X�̐F��
	SimpleMath::Color m_boxColor;
	bool m_changeColorFlag;

	// �F��ω������鎞�ԕϐ�
	float m_colorChangeTime;
};