#include "pch.h"
#include "MoveCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500


const float MoveCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

MoveCamera::MoveCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.f }
	, m_scrollWheelValue(0)
	, m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_saveTarget{ 0.f }
	, m_time()
	, m_targetChangeTime()
	, m_prevWheelValue()
{
}

MoveCamera::~MoveCamera()
{
}

void MoveCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX  = m_prevY  = 0;
	m_scrollWheelValue = 0;
	m_time = 0;


	m_view	    = DirectX::SimpleMath::Matrix::Identity;
	m_move		= DirectX::SimpleMath::Vector3::Zero;
	m_eye		= DirectX::SimpleMath::Vector3::Zero;
	m_target	= DirectX::SimpleMath::Vector3::Zero;

}

void MoveCamera::Update(bool scroll, bool move)
{
	auto state = InputSupport::GetInstance().GetMouseState().GetLastState();
	auto keyboard = InputSupport::GetInstance().GetKeybordState().GetLastState();

	// �J�n�������ׂ̏���
	if (m_time <= 1)
	{
		m_time += DeltaTime::GetInstance().GetDeltaTime();

		m_move.y = m_time * 1.2f;
		m_move.z = m_time * 1.3f;

	}

	m_targetChangeTime += DeltaTime::GetInstance().GetDeltaTime() * 1.15f;

	if (m_targetChangeTime <= 0) m_targetChangeTime = 0;
	if (m_targetChangeTime >= 1) m_targetChangeTime = 1;

	// �J�����ړ������邩�ۂ�
	if (move)
	{
		// �}�E�X�̉E�N���b�N���h���b�O�ŃJ�������W���X�V����
		if (state.rightButton && keyboard.LeftShift)
		{
			DraggedDistance(state.x, state.y);
		}
	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevX = state.x;
	m_prevY = state.y;

	}

	// �X�N���[�������邩�ۂ�
	if (scroll)
	{
		int value = state.scrollWheelValue - m_prevWheelValue;
		int newValue = m_scrollWheelValue + value;

		// ��������ݒ�(clamp)
		if (newValue <= MAX_SAVEWHELL) newValue = MAX_SAVEWHELL;
		if (newValue > 0) newValue = 0;

		if (newValue != 0 && newValue != MAX_SAVEWHELL)
		{
			m_scrollWheelValue = newValue;
		}
		else
		{
			value = 0;
		}

		if (m_scrollWheelValue == MAX_SAVEWHELL && value > 0)
		{
			m_scrollWheelValue -= value;
		}
		else if (m_scrollWheelValue == 0 && value < 0)
		{
			m_scrollWheelValue -= value;
		}

		if (m_scrollWheelValue > 0)
		{
			m_scrollWheelValue = 0;
			DirectX::Mouse::Get().ResetScrollWheelValue();
		}
	}

	// �O��̒l��ۑ�
	m_prevWheelValue = state.scrollWheelValue;

	// �r���[�s��̎Z�o
	CalculateViewMatrix();
}

void MoveCamera::TargetChange(DirectX::SimpleMath::Vector3 targetA, DirectX::SimpleMath::Vector3 targetB)
{
	m_target.x = Easing::EaseOutQuint(targetA.x, targetB.x, m_targetChangeTime);
	m_target.y = Easing::EaseOutBounce(targetA.y, targetB.y, m_targetChangeTime);
	m_target.z = Easing::EaseOutQuint(targetA.z, targetB.z, m_targetChangeTime);
}

void MoveCamera::DraggedDistance(int x, int y)
{
	// �}�E�X�|�C���^�̑O�񂩂�̕ψ�
	float dx = static_cast<float>(x - m_prevX) * CORRECTION_VALUE;
	float dy = static_cast<float>(y - m_prevY) * CORRECTION_VALUE;

	if (dx != 0.0f || dy != 0.0f)
	{
		// �}�E�X�|�C���^�̕ψʂ����ɁA�w���x���̉�]�p�����߂�
		// X��Y�̊֌W�ɒ��ӁI�I
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// �p�x�̍X�V Y�ɂ͉�]�̐�����������
		m_angleX += (m_angleX + angleX) >= 0.8f ? 0 : (m_angleX + angleX) <= -0.7f ? 0 : angleX;
		m_angleY += angleY;
	}
}

void MoveCamera::CalculateViewMatrix()
{
	// �r���[�s����Z�o����
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_angleX);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(m_move.x, m_move.y, m_move.z);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_scrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye		= eye;
	m_target	= target;
	m_view		= DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
