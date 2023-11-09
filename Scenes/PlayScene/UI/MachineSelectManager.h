//--------------------------------------------------------------------------------------
// File		: MachineSelectManager.h
//
// �A���P�~�J���}�V���֘A��UI�Ɋւ��N���X�����
// 
// Use		: AlchemicalMachineManager
// Date		: 2023.5.18
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelect.h"
#include "NecromaLib/GameData/Camera.h"

#include "NecromaLib/GameData/CommonStruct.h"

class FieldObjectManager;
class DisplayMagicCircle;
class UIKeyControl;
class Number;

class MachineSelectManager
{
public:
	MachineSelectManager();
	~MachineSelectManager();

	void TextuerLoader();

	void Initialize();

	// fieldObjecytManager = UI�ɕω���^����I�u�W�F�N�g�̌��o(�N���X�^��)
	void Update(FieldObjectManager* fieldObjectManager);

	void Render();

	void RenderUI(int machineNum[]);

	/// <summary>
	/// ���f����`�悷��
	/// </summary>
	/// <param name="index">�C���f�b�N�X�ԍ�(�}�V���^�C�vID)</param>
	void ModelRender(int index);

	void MagicCircleRender();

	void Finalize();

	// �}�V���A�C�R������
	void RotationMachineList();

public:

	void SetSelectNoneFlag(bool flag) { m_selectNoneFlag = flag; }

	// �I�𒆂̃}�V���̃^�C�v��Ԃ�
	MACHINE_TYPE GetSelectMachineType()	{ return m_selectMachineType; }


	int GetMachineUISelectNumber() { return m_selectNumber; }

	// �I���{�b�N�X��������Ă��邩��Type�ɉ����ĕԂ�
	bool GetHitMouseToSelectBox(int index)							{ return m_machineSelect[index]->GetHitMouseFlag();}

	// �I���{�b�N�X��Type�ɉ����ĕԂ�
	MachineSelect* GetMachineSelect(int index)						{ return m_machineSelect[index].get();}

	bool GetManufacturingFlag()										{ return m_manufacturingFlag;}

	// ��̗v�f�ł�true�Ȃ�
	bool GetHitMouseToSelectBoxEven()								{ return m_selectBoxAll;}

private:

	// ���\�[�X�Q�����炷
	void ReduceResource(MACHINE_TYPE type);

private:

	std::unique_ptr<MachineSelect> m_machineSelect[MACHINE_TYPE::NUM];

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

	// ������������
	std::unique_ptr<Number> m_machineNumRender;

	MACHINE_TYPE m_selectMachineType;

	bool m_selectBoxAll;
	bool m_manufacturingFlag;

	// �t�B�[���h���None��I�������ۂɁA�S�Ă�MachineSelect�̑I��Box�̐F��ύX����
	bool m_selectNoneFlag;

	// �}�V���̖��O�\��
	std::unique_ptr<SelectionBox> m_machineName;

	// �I���{�b�N�X(�B���{�^���p)
	std::unique_ptr<SelectionBox> m_selectionAlchemi;

	// �I���{�b�N�X(UI�I��p)
	std::unique_ptr<SelectionBox> m_selectionLeft,m_selectionRight;

	//��ʂɑ傫���o�������閂�@�w
	std::unique_ptr<DisplayMagicCircle>			m_displayMagicCircle;

	// �A�C�R���̉�]���s�����ԕϐ�
	float m_rotateTime;

	// �I�����Ă���}�V���̔ԍ�
	int m_selectNumber;

	// �OFrame�̃}�V���̔ԍ�
	int m_prevSelectNumber;

	// �J�[�\���ɂ����Ă���}�V���̔ԍ�
	int m_cursorMachineNumber;

	// UI��]�����]���ۂ�
	bool m_forwardOrderFlag;

};