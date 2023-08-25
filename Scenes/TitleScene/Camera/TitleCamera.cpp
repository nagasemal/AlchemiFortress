#include "pch.h"
#include "TitleCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/Animation.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500


const float TitleCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

TitleCamera::TitleCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.0f,1.0f,0.0f}
	, m_scrollWheelValue(0)
	, m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_animationTimer()
	, m_prevWheelValue()
{
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX = m_prevY = 0;
	m_scrollWheelValue = 0;
	m_animationTimer = 0;


	m_view = DirectX::SimpleMath::Matrix::Identity;
	m_move = DirectX::SimpleMath::Vector3::Zero;
	m_eye = DirectX::SimpleMath::Vector3::Zero;
	m_target = DirectX::SimpleMath::Vector3::Zero;

}

void TitleCamera::Update()
{
	// 開始時アニメーション
	if (m_animationTimer <= 1)
	{
		m_animationTimer += 0.5f * DeltaTime::GetInstance().GetDeltaTime();

		m_move.x = Easing::EaseInOutBack(0.0f, 2.0f, m_animationTimer);
		m_move.z = Easing::EaseInOutBack(0.0f, 2.0f, m_animationTimer);
		m_move.y = Easing::EaseInOutBack(1.0f, 3.0f, m_animationTimer);

		m_angleX = Easing::EaseInOutQuart(0.0f, 55.0f, m_animationTimer);
		m_angleY = Easing::EaseInOutQuart(0.0f, 90.0f, m_animationTimer);

	}

	// ビュー行列の算出
	CalculateViewMatrix();
}

void TitleCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angleX));

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(m_move);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_scrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye = eye;
	m_target = target;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
