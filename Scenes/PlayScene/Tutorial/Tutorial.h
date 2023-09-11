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
class PopLine;

class Gauge;
class AlchemicalMachineManager;

class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	void Update(AlchemicalMachineManager* machineManager,Gauge* gauge);
	void Render();
	void Render_Layer2(); // �\���̊֌W�������֐����K�v�ł���������
	void Finalize();

	void CreateInterfase();
	
	bool GetTutorialFlag() { return m_tutorialFlag; }

	// ���݂̑I���󋵂���ǂ��Ƀ��C�������������߂܂�
	void LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge);

private:

	static const std::vector<const wchar_t*> FILENAME;

	std::unique_ptr<DrawArrow> m_arrowR,m_arrowL;
	std::unique_ptr<SelectionBox> m_cancelButton;
	std::unique_ptr<SelectionBox> m_doubleSpeedButton; // �{���{�^��

	std::unique_ptr<PopLine> m_showLine;	// �����w���Ă���̂�������悤�ɂ���

	std::unique_ptr<UserInterface> m_backFlame;
	std::unique_ptr<UserInterface> m_textTexture;

	bool m_tutorialFlag;
	int m_selectNumber;
	int m_doubleSpeedNum; // �{���{��

};