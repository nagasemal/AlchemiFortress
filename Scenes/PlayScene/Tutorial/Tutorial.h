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

#include "NecromaLib/GameData/Animation.h"

class SelectionBox;
class DrawArrow;
class UserInterface;
class DrawLine;
class DrawBox;

class AlchemicalMachineObject;
class MachineExplanation;
class Particle;

class PlayScene;

class Tutorial
{
public:
	enum INSTRUCTION_TYPE : int
	{
		NONE = 0,			// �w��Ȃ�
		ATTACKER = 1,		// �U���^
		DEFENSER = 2,		// �h��^
		UPPER = 3,			// �͈͓������^
		RECOVERY = 4,		// ���͉���^
		MINING = 5,			// �̌@�^

		OPERATION = 6,		// ������@
		OPERATION_MACHINE = 7,	// ������@_�}�V��
		MACHINE_UI = 8,		// �}�V��UI�̑���ɂ���

		GAUGE_HP = 9,		// HP�ɂ���
		GAUGE_MP = 10,		// MP�ɂ���
		GAUGE_CRYSTAL = 11,	// �N���X�^���ɂ���

		MISSION = 12,		// �~�b�V�����ɂ���
		ALCHEMI = 13,		// �B���ɂ���
		SPAWN = 14,			// �ݒu�ɂ���

		ROTATE_STOP = 15,
		LINE_SELECT = 16,
		CAMERA_MOVE = 17,

		TUTORIAL = 18,

		NUM
	};

public:
	Tutorial();
	~Tutorial();

	void Initialize(std::vector<int> tutorialNumber, PlayScene* pPlayScene);
	void Update(PlayScene* pPlayScene,bool stopFlag = false);
	void Render();
	void Render_Layer2(); // �\���̊֌W�������֐����K�v�ł���������
	void Finalize();

	void RelodeTutorial(std::vector<int> tutorialNumber, PlayScene* pPlayScene);
	void CreateInterfase();
	
	bool GetTutorialFlag() { return m_explanationFlag || (m_tutorialFlag && m_cameraFlag); }
	bool GetExplanationFlag() { return m_explanationFlag; }

	// ���݂̑I���󋵂���ǂ��Ƀ��C�������������߂܂�
	void LinePosSet(PlayScene* pPlayScene,int number);
	
	void NextTutorial(PlayScene* pPlayScene);

	SelectionBox* GetTitleSceneButton() { return m_titleSceneBox.get(); }
	SelectionBox* GetSelectSceneButton() { return m_selectSceneBox.get(); }

private:

	// NoneType��T���o���A���̈ʒu�ɃJ�[�\���{�b�N�X���o�������܂��B
	void NonePosSearch(PlayScene* pPlayScene);

private:

	static const std::vector<const wchar_t*> FILENAME;

	std::unique_ptr<DrawArrow> m_arrowR,m_arrowL;

	// �������[�h�ֈڍs,��������{�^��
	std::unique_ptr<SelectionBox> m_explanationButton;
	// �`���[�g���A�����[�h����������{�^��
	std::unique_ptr<SelectionBox> m_tutorialExitButton;
	// �������w���Ă���̂��𖾊m�ɂ���I���{�b�N�X
	std::unique_ptr<DrawBox>  m_showBox;

	// �^�C�g���ɖ߂�I���{�b�N�X
	std::unique_ptr<SelectionBox>  m_titleSceneBox;
	// �Z���N�g�V�[���ɖ߂�I���{�b�N�X
	std::unique_ptr<SelectionBox>  m_selectSceneBox;
	// ������߂ă��U���g�V�[���Ɉڍs����I���{�b�N�X
	std::unique_ptr<SelectionBox>  m_resultSceneBox;

	std::unique_ptr<UserInterface> m_backFlame;
	std::unique_ptr<UserInterface> m_textTexture;

	std::unique_ptr<AlchemicalMachineObject> m_amMachine;
	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// �������[�h�ɓ����Ă��邩�ۂ�
	bool m_explanationFlag;

	// �`���[�g���A�����[�h�ɓ����Ă��邩�ۂ�
	bool m_tutorialFlag;
	bool m_cameraFlag;

	// �`���[�g���A�����̃A�j���[�V����
	AnimationData m_tutorialTime;

	int m_selectNumber;
	// 0 �����Ȃ� �E�F�[�u�̃`���[�g���A���ԍ����i�[����Ă���
	std::vector<int> m_tutorialNumber;

	int m_maxSelectVal;

	// particle�̏o���𐧌䂷��ϐ�
	float m_particleTime;

	std::unique_ptr<Particle> m_particle_Select;

	//std::unique_ptr<TutorialStateContext> m_tutorialState;

};