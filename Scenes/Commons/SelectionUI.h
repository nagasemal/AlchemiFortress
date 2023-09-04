#pragma once

#include "NecromaLib/GameData/GameObject2D.h"

class SelectionUI : public GameObjct2D
{
public:
	SelectionUI();
	~SelectionUI();
	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �I������
	void Finalize()				override;

	// �}�E�X���G�ꂽ����
	bool HitMouse();
	// ���N���b�N���ꂽ����
	bool ClickMouse();
	// ���N���b�N����đI������Ă����Ԃ̏���
	bool SelectionMouse();
	// ���N���b�N�������ꑱ���Ă��鏈��
	bool HoldMouse();
	// �}�E�X�����ꂽ����
	bool ExitMouse();



	// ���ݐG����Ԃ�
	bool GetActiveFlag() { return m_activeFlag; }

	// �G�����/�G��Ȃ���Ԃւ̑J��
	void SetActiveFlag(bool flag) { m_activeFlag = flag; }

	// �}�E�X���������Ă��锻���ς����܂�
	void SetHitMouseFlag(bool flag) { m_hitMouseFlag = flag; }
	// �I����ԂɂȂ��Ă��锻���ς����܂�
	void SetSelectFlag(bool flag) { m_selectFlag = flag; }
	void SetKeySelectFlag(bool flag) { m_keySelectFlag = flag;}
	// �F�����R�ɕς����܂��B(�{�b�N�X�������������ۂȂǂɎg�p)
	void SetLuminousColor(DirectX::SimpleMath::Color color) { m_boxColor = color; }
	void SetLuminousFlag(bool flag) { m_luminousFlag = flag; };


protected:

	bool m_hitMouseFlag;
	bool m_selectFlag;
	bool m_keySelectFlag; // �L�[�{�[�h��ɂđI�����ꂽ���Ƃ������t���O

	DirectX::SimpleMath::Color m_boxColor;
	bool m_luminousFlag;

	bool m_activeFlag; //���ݐG����Ԃ�

};