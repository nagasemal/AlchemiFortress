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
	, m_time()
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
	m_time = 0;


	m_View	    = DirectX::SimpleMath::Matrix::Identity;
	m_Move		= DirectX::SimpleMath::Vector3::Zero;
	m_Eye		= DirectX::SimpleMath::Vector3::Zero;
	m_Target	= DirectX::SimpleMath::Vector3::Zero;

}

void MoveCamera::Update()
{
	auto state = InputSupport::GetInstance().GetMouseState().GetLastState();

	// 開始時引く為の処理
	if (m_time <= 1)
	{
		m_time += DeltaTime::GetInstance().GetDeltaTime();

		m_Move.y = m_time * 1.2f;
		m_Move.z = m_time * 1.3f;

	}

	// マウスの右クリック＆ドラッグでカメラ座標を更新する
	if (state.rightButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	m_PrevX = state.x;
	m_PrevY = state.y;

	// マウスホイールのスクロール値を取得
	m_ScrollWheelValue = state.scrollWheelValue;
	if (m_ScrollWheelValue > 0)
	{
		m_ScrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列の算出
	CalculateViewMatrix();
}

void MoveCamera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	float dx = static_cast<float>(x - m_PrevX) * CORRECTION_VALUE;
	float dy = static_cast<float>(y - m_PrevY) * CORRECTION_VALUE;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		// XとYの関係に注意！！
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新
		m_AngleX += angleX;
		m_AngleY += angleY;
	}
}

void MoveCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_AngleY);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_AngleX);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(m_Move.x, m_Move.y, m_Move.z);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_ScrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	m_Eye		= eye;
	m_Target	= target;
	m_View		= DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
