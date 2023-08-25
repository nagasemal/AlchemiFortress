#pragma once

class MoveCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// �J�����̋���

	MoveCamera();
	~MoveCamera();

	void Initialize();
	// �X�V����
	void Update(bool scroll,bool move);

	// �r���[�s����擾����
	DirectX::SimpleMath::Matrix GetViewMatrix() const		{ return m_view; }
	// �f�o�b�O�J�����̍��W���擾����
	DirectX::SimpleMath::Vector3 GetEyePosition() const		{ return m_eye; }
	// �f�o�b�O�J�����̒����_���W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const	{ return m_target; }

	void SetEyeProsition(DirectX::SimpleMath::Vector3 eye)			{ m_eye = eye;}
	void SetTargetProsition(DirectX::SimpleMath::Vector3 target)	{ m_target = target; }
	void SetSaveTargetProsition(DirectX::SimpleMath::Vector3 target) { m_saveTarget = target; }

	void TargetChange(DirectX::SimpleMath::Vector3 targetA, DirectX::SimpleMath::Vector3 targetB);
	void ResetTargetChangeTimer() { m_targetChangeTime = 0; }



private:

	// �c��]�p
	float m_angleX;								
	// ����]�p
	float m_angleY;								
	// �O��̃}�E�X���W(X,Y)
	int m_prevX, m_prevY;						
	// �}�E�X�z�C�[���̃X�N���[���l
	int m_scrollWheelValue;				
	// �O��̃X�N���[���l
	int m_prevWheelValue;						
	// �ړ��l
	DirectX::SimpleMath::Vector3 m_move;		
	// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Matrix m_view;			
	// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;

	// �O��̒����_
	DirectX::SimpleMath::Vector3 m_saveTarget;
	// �����Ŏg���^�C�}�[
	float m_time;			
	// �^�[�Q�b�g�ύX�Ɏg���^�C�}�[
	float m_targetChangeTime;

	// �}�E�X�|�C���^�̃h���b�O�J�n�ʒu����̕ψ�(���Βl)
	void DraggedDistance(int x, int y);

	// �r���[�s��̎Z�o
	void CalculateViewMatrix();
};