//--------------------------------------------------------------------------------------
// File: Tutorial.h
//
//�@�`���[�g���A���X�e�[�W�ł���ꍇ�A
//	�Ή������`���[�g���A���摜���o��
// 
// Date: 2023.9.4
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class SelectionBox;
class DrawArrow;
class UserInterface;
class DrawLine;
class DrawBox;

class Gauge;
class AlchemicalMachineManager;
class MissionRender;
class AlchemicalMachineObject;
class MachineExplanation;

class Tutorial
{
public:
	enum INSTRUCTION_TYPE : int
	{
		NONE = 0,		// �w��Ȃ�
		ATTACKER = 1,		// �U���^
		DEFENSER = 2,		// �h��^
		UPPER = 3,		// �͈͓������^
		RECOVERY = 4,		// ���͉���^
		MINING = 5,		// �̌@�^

		OPERATION = 6,		// ������@
		OPERATION_MACHINE = 7,	// ������@_�}�V��
		MACHINE_UI = 8,		// �}�V��UI�̑���ɂ���

		GAUGE_HP = 9,		// HP�ɂ���
		GAUGE_MP = 10,		// MP�ɂ���
		GAUGE_CRYSTAL = 11,		// �N���X�^���ɂ���

		MISSION = 12,		// �~�b�V�����ɂ���
		ALCHEMI = 13,		// �B���ɂ���
		SPAWN = 14,		// �ݒu�ɂ���

		NUM
	};

public:
	Tutorial();
	~Tutorial();

	void Initialize(std::vector<int> tutorialNumber);
	void Update(AlchemicalMachineManager* machineManager,Gauge* gauge, MissionRender* missionRender,bool stopFlag = false);
	void Render();
	void Render_Layer2(); // �\���̊֌W�������֐����K�v�ł���������
	void Finalize();

	void RelodeTutorial(std::vector<int> tutorialNumber);
	void CreateInterfase();
	
	bool GetTutorialFlag() { return m_explanationFlag || (m_tutorialFlag && m_cameraFlag); }

	// ���݂̑I���󋵂���ǂ��Ƀ��C�������������߂܂�
	void LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge, MissionRender* missionRender,int number);

private:

	static const std::vector<const wchar_t*> FILENAME;

	std::unique_ptr<DrawArrow> m_arrowR,m_arrowL;

	// �������[�h�ֈڍs,��������{�^��
	std::unique_ptr<SelectionBox> m_explanationButton;
	// �`���[�g���A�����[�h����������{�^��
	std::unique_ptr<SelectionBox> m_tutorialExitButton;

	std::unique_ptr<DrawLine> m_showLine;	// �����w���Ă���̂�������悤�ɂ���
	std::unique_ptr<DrawBox>  m_showBox;

	std::unique_ptr<UserInterface> m_backFlame;
	std::unique_ptr<UserInterface> m_textTexture;

	std::unique_ptr<AlchemicalMachineObject> m_amMachine;
	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// �������[�h�ɓ����Ă��邩�ۂ�
	bool m_explanationFlag;

	// �`���[�g���A�����[�h�ɓ����Ă��邩�ۂ�
	bool m_tutorialFlag;
	bool m_cameraFlag;

	int m_selectNumber;
	// 0 �����Ȃ�
	std::vector<int> m_tutorialNumber;

	int m_maxSelectVal;

};