#pragma once

class MoveCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// �J�����̋���

	MoveCamera();
	~MoveCamera();

	void Initialize();
	// �X�V����
	void Update();

	// �r���[�s����擾����
	DirectX::SimpleMath::Matrix GetViewMatrix() const		{ return m_View; }
	// �f�o�b�O�J�����̍��W���擾����
	DirectX::SimpleMath::Vector3 GetEyePosition() const		{ return m_Eye; }
	// �f�o�b�O�J�����̒����_���W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const	{ return m_Target; }

private:

	float m_AngleX;								// �c��]�p
	float m_AngleY;								// ����]�p
	int m_PrevX, m_PrevY;						// �O��̃}�E�X���W(X,Y)
	int m_ScrollWheelValue;						// �}�E�X�z�C�[���̃X�N���[���l
	DirectX::SimpleMath::Vector3 m_Move;		// �ړ��l
	DirectX::SimpleMath::Matrix m_View;			// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Vector3 m_Eye;			// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_Target;		// �����_

	float m_time;								// �����Ŏg���^�C�}�[

	// �}�E�X�|�C���^�̃h���b�O�J�n�ʒu����̕ψ�(���Βl)
	void DraggedDistance(int x, int y);

	// �r���[�s��̎Z�o
	void CalculateViewMatrix();
};