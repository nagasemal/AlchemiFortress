#include "pch.h"
#include "MoveCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define  CORRECTION_VALUE 0.2f


const float MoveCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

MoveCamera::MoveCamera()
	: m_AngleX(0.0f)
	, m_AngleY(0.0f)
	, m_PrevX(0)
	, m_PrevY(0)
	, m_Move{ 0.f }
	, m_ScrollWheelValue(50)
	, m_View(DirectX::SimpleMath::Matrix::Identity)
	, m_Eye(0.0f, 0.0f, 0.0f)
	, m_Target{ 0.f }
{
}

MoveCamera::~MoveCamera()
{
}

void MoveCamera::Initialize()
{
	m_AngleX = m_AngleY = 0.0f;
	m_PrevX  = m_PrevY  = 0;
	m_ScrollWheelValue = 0;


	m_View	    = DirectX::SimpleMath::Matrix::Identity;
	m_Move		= DirectX::SimpleMath::Vector3::Zero;
	m_Eye		= DirectX::SimpleMath::Vector3::Zero;
	m_Target	= DirectX::SimpleMath::Vector3::Zero;

}

void MoveCamera::Update()
{

	static float time;
	auto state = InputSupport::GetInstance().GetMouseState().GetLastState();

	// �J�n�������ׂ̏���
	if (time <= 1)
	{
		time += DeltaTime::GetInstance().GetDeltaTime();

		m_Move.y = time * 1.2;
		m_Move.z = time * 1.3;

	}

	// �}�E�X�̉E�N���b�N���h���b�O�ŃJ�������W���X�V����
	if (state.rightButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_PrevX = state.x;
	m_PrevY = state.y;

	// �}�E�X�z�C�[���̃X�N���[���l���擾
	m_ScrollWheelValue = state.scrollWheelValue;
	if (m_ScrollWheelValue > 0)
	{
		m_ScrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// �r���[�s��̎Z�o
	CalculateViewMatrix();
}

void MoveCamera::DraggedDistance(int x, int y)
{
	// �}�E�X�|�C���^�̑O�񂩂�̕ψ�
	float dx = static_cast<float>(x - m_PrevX) * CORRECTION_VALUE;
	float dy = static_cast<float>(y - m_PrevY) * CORRECTION_VALUE;

	if (dx != 0.0f || dy != 0.0f)
	{
		// �}�E�X�|�C���^�̕ψʂ����ɁA�w���x���̉�]�p�����߂�
		// X��Y�̊֌W�ɒ��ӁI�I
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// �p�x�̍X�V
		m_AngleX += angleX;
		m_AngleY += angleY;
	}
}

void MoveCamera::CalculateViewMatrix()
{
	// �r���[�s����Z�o����
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_AngleY);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_AngleX);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(m_Move.x, m_Move.y, m_Move.z);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_ScrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	m_Eye		= eye;
	m_Target	= target;
	m_View		= DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}