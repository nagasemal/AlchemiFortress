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

	//m_move.x = Easing::EaseInOutBack(0.0f, 2.0f, m_testAnimation->Animatior());

	//// カメラ移動をするか否か
	//if (true)
	//{
	//	// マウスの右クリック＆ドラッグでカメラ座標を更新する
	//	if (state.rightButton)
	//	{
	//		DraggedDistance(state.x, state.y);
	//	}
	//	// マウスの座標を前回の値として保存
	//	m_prevX = state.x;
	//	m_prevY = state.y;
	//}
	//// スクロールをするか否か
	//if (true)
	//{
	//	int value = state.scrollWheelValue - m_prevWheelValue;
	//	int newValue = m_scrollWheelValue + value;
	//	// 上限下限設定(clamp)
	//	if (newValue <= MAX_SAVEWHELL) newValue = MAX_SAVEWHELL;
	//	if (newValue > 0) newValue = 0;
	//	if (newValue != 0 && newValue != MAX_SAVEWHELL)
	//	{
	//		m_scrollWheelValue = newValue;
	//	}
	//	else
	//	{
	//		value = 0;
	//	}
	//	if (m_scrollWheelValue == MAX_SAVEWHELL && value > 0)
	//	{
	//		m_scrollWheelValue -= value;
	//	}
	//	else if (m_scrollWheelValue == 0 && value < 0)
	//	{
	//		m_scrollWheelValue -= value;
	//	}
	//	if (m_scrollWheelValue > 0)
	//	{
	//		m_scrollWheelValue = 0;
	//		DirectX::Mouse::Get().ResetScrollWheelValue();
	//	}
	//}
	//// 前回の値を保存
	//m_prevWheelValue = state.scrollWheelValue;

	// ビュー行列の算出
	CalculateViewMatrix();
}

void TitleCamera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	float dx = static_cast<float>(x - m_prevX) * CORRECTION_VALUE;
	float dy = static_cast<float>(y - m_prevY) * CORRECTION_VALUE;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		// XとYの関係に注意！！
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新 Yには回転の制限をかける
		m_angleX += (m_angleX + angleX) >= 0.8f ? 0 : (m_angleX + angleX) <= -0.7f ? 0 : angleX;
		m_angleY += angleY;
	}
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
