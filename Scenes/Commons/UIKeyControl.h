//--------------------------------------------------------------------------------------
// File: UIKeyControl.h
//
// �L�[�{�[�h���UI�𑀍삷�鏈��
// 
// Date: 2023.8.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include <vector>

class SelectionUI;

class UIKeyControl
{
public:

	struct UI_Data
	{
		SelectionUI* ui;
		int low; // �s
		int col; // ��
	};

public:
	UIKeyControl();
	~UIKeyControl();

	void Update();

	void AddUI(SelectionUI* ui,int low,int col);

	void DeleteUI(SelectionUI* ui);

private:

	bool ArrowKeyPush();

private:

	std::vector<UI_Data> m_selectionUIs;

	int m_nowCol; // ���݂̍s
	int m_nowLow; // ���݂̗�

	int m_maxCol; // ���݂̍s�̍ő�l

	std::vector<int> m_maxLows;

	DirectX::SimpleMath::Vector2 m_prevMousePos; // �O�̃}�E�X�̃|�W�V�����̎擾
	bool m_keyContorlFlag;

};