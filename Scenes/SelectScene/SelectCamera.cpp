#include "pch.h"
#include "SelectCamera.h"


#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/Animation.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500

#define ROTATION 360.0f

const float SelectCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

SelectCamera::SelectCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.0f,1.0f,0.0f }
	, m_scrollWheelValue(0)
	, m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_animationTimer_Start()
	, m_animationTimer_Select()
	, saveAngleY()
	, m_prevWheelValue()
{
}

SelectCamera::~SelectCamera()
{
}

void SelectCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX = m_prevY = 0;
	m_scrollWheelValue = 0;
	m_animationTimer_Start = 0.0f;
	m_animationTimer_Select = 1.0f;
	saveAngleY = 0;

	auto camera = ShareData::GetInstance().GetCamera();

	m_view = camera->GetViewMatrix();
	m_move = DirectX::SimpleMath::Vector3(0.0f,1.0f,0.001f);
	m_eye = camera->GetEyePosition();
	m_target = camera->GetTargetPosition();

}

void SelectCamera::Update()
{
	// 開始時アニメーション
	if (m_animationTimer_Start <= 1)
	{
		m_animationTimer_Start += 0.5f * DeltaTime::GetInstance().GetDeltaTime();
		m_move.y = Easing::EaseInOutExpo(1.0f, 5.0f, m_animationTimer_Start);
	}

	if (m_animationTimer_Select <= 1)
	{
		m_animationTimer_Select += DeltaTime::GetInstance().GetDeltaTime();
		m_angleY = Easing::EaseOutCirc(saveAngleY, saveAngleY + ROTATION * 4.0f, m_animationTimer_Select);
	}

	// ビュー行列の算出
	CalculateViewMatrix();
}

void SelectCamera::AnimationReset()
{
	saveAngleY = m_angleY;
	m_animationTimer_Select = 0.0f;

}

void SelectCamera::CalculateViewMatrix()
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
