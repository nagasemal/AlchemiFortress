//--------------------------------------------------------------------------------------
// File: SelectCamera.h
//
//�@SelectScene���Ŏg����J�����N���X
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once

class SelectCamera
{
public:

	static const float DEFAULT_CAMERA_DISTANCE;	// �J�����̋���

	SelectCamera();
	~SelectCamera();

	void Initialize();
	// �X�V����
	void Update();

	void AnimationReset();

	// �r���[�s����擾����
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_view; }
	// �f�o�b�O�J�����̍��W���擾����
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return m_eye; }
	// �f�o�b�O�J�����̒����_���W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_target; }

	float GetAnimationTimer() const { return m_animationTimer_Select;}

private:

	float m_angleX;								// �c��]�p
	float m_angleY;								// ����]�p
	int m_prevX, m_prevY;						// �O��̃}�E�X���W(X,Y)
	int m_scrollWheelValue;						// �}�E�X�z�C�[���̃X�N���[���l
	int m_prevWheelValue;						// �O��̃X�N���[���l
	DirectX::SimpleMath::Vector3 m_move;		// �ړ��l
	DirectX::SimpleMath::Matrix m_view;			// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Vector3 m_eye;			// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_target;		// �����_

	float m_animationTimer_Start;				// �n�߂Ɏg����^�C�}�[

	float m_animationTimer_Select;				// �X�e�[�W�̐؂�ւ��Ŏg����^�C�}�[

	float saveAngleY;

	// �r���[�s��̎Z�o
	void CalculateViewMatrix();

};