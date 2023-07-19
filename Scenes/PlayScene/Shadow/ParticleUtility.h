//--------------------------------------------------------------------------------------
// File: ParticleUtility.h
//
//  Particle�N���X�ɂĎg�p����l�̑J�ڂɎg�p�@
//  
// Date: 2023.7.3
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class ParticleUtility
{
	//�ϐ�
private:


	//�֐�
private:
	//���W
	DirectX::SimpleMath::Vector3 m_position;

	//���x
	DirectX::SimpleMath::Vector3 m_velocity;
	//�����x
	DirectX::SimpleMath::Vector3 m_accele;

	//�X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//��������
	float m_life;
	float m_startLife;

	//���[�e�[�V����
	DirectX::SimpleMath::Quaternion m_quaternion;
	DirectX::SimpleMath::Vector3	m_rotate;

	//�J���[
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;

	//�֐�
public:
	/// <summary>
	/// �����ݒ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="velocity">����</param>
	/// <param name="accele">�����x</param>
	/// <param name="startScale">�����X�P�[��</param>
	/// <param name="endScale">�ŏI�X�P�[��</param>
	/// <param name="startColor">�����J���[</param>
	/// <param name="endColor">�ŏI�J���[</param>
	ParticleUtility(
		float life = 1.0f,
		DirectX::SimpleMath::Vector3 pos = {0.0f,0.0f,0.0f},
		DirectX::SimpleMath::Vector3 velocity = {0.0f,0.0f,0.0f},
		DirectX::SimpleMath::Vector3 accele = {0.0f,0.0f,0.0f},
		DirectX::SimpleMath::Vector3 startScale = {1.0f,1.0f,1.0f}, DirectX::SimpleMath::Vector3 endScale = {0.0f,0.0f,0.0f},
		DirectX::SimpleMath::Color startColor = {1.0f,1.0f,1.0f,1.0f}, DirectX::SimpleMath::Color endColor = {0.0f,0.0f,0.0f,0.0f});
	//�f�X�g���N�^
	~ParticleUtility();

	// �X�V
	bool Update();

	//getter

	// �|�W�V����
	const DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// ����
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	// �����x
	const DirectX::SimpleMath::Vector3 GetAccele() { return m_accele; }

	// �傫�� �F ����
	const DirectX::SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	// �傫�� �F �J�n��
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	// �傫�� �F �I����
	const DirectX::SimpleMath::Vector3 GetEndScale() { return m_endScale; }

	// �������� : ����
	const float GetLife() { return m_life; }
	// �������� : �J�n��
	const float GetStartLife() { return m_startLife; }

	// �F : ����
	const DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }
	// �F : �J�n��
	const DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }
	// �F : �I����
	const DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }

	// ��]�N�I�[�^�j�I��
	const DirectX::SimpleMath::Quaternion GetQuaternion() { return m_quaternion; }
	// ��]�p
	const DirectX::SimpleMath::Vector3 GetRad() { return m_rotate; }

	// setter

	// �|�W�V����
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// ����
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { m_velocity = vel; }
	// �����x
	void SetAccele(DirectX::SimpleMath::Vector3 acc) { m_accele = acc; }

	// �傫�� �F ����
	void SetNowScale(DirectX::SimpleMath::Vector3 sca) { m_nowScale = sca; }
	// �傫�� �F �J�n��
	void SetStartScale(DirectX::SimpleMath::Vector3 sca) { m_startScale = sca; }
	// �傫�� �F �I����
	void SetEndScale(DirectX::SimpleMath::Vector3 sca) { m_endScale = sca; }

	// �������� �F ����
	void SetLife(float life) { m_life = life; }
	// �������� �F �J�n��
	void SetStartLife(float life) { m_startLife = life; }

	// �F �F ����
	void SetNowColor(DirectX::SimpleMath::Color col) { m_nowColor = col; }
	// �F �F �J�n��
	void SetStartColor(DirectX::SimpleMath::Color col) { m_startColor = col; }
	// �F �F �I����
	void SetEndColor(DirectX::SimpleMath::Color col) { m_endColor = col; }

	// ��]�p
	void SetRad(DirectX::SimpleMath::Vector3 rad) { m_rotate = rad; }


};