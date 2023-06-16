#include "pch.h"
#include "MoveCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define  CORRECTION_VALUE 0.2f


const float MoveCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

MoveCamera::MoveCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.f }
	, m_scrollWheelValue(50)
	, m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_time()
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

void MoveCamera::Update()
{
	auto state = InputSupport::GetInstance().GetMouseState().GetLastState();

	// 開始時引く為の処理
	if (m_time <= 1)
	{
		m_time += DeltaTime::GetInstance().GetDeltaTime();

		m_move.y = m_time * 1.2f;
		m_move.z = m_time * 1.3f;

	}

	// マウスの右クリック＆ドラッグでカメラ座標を更新する
	if (state.rightButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	m_prevX = state.x;
	m_prevY = state.y;

	// マウスホイールのスクロール値を取得
	m_scrollWheelValue = state.scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列の算出
	CalculateViewMatrix();
}

void MoveCamera::DraggedDistance(int x, int y)
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

void MoveCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
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
