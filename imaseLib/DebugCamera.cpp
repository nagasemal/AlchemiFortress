//-------------------------------------------------------------------
// @file		DebugCamera.cpp
// @brief		デバッグカメラ
// @date		2020/08/04
// @copyright	(c)2020 TRIDENT,Department of Game Science.
//-------------------------------------------------------------------
#include "pch.h"
#include "DebugCamera.h"

// 定数の初期化
const float DebugCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;

// コンストラクタ
DebugCamera::DebugCamera()
	: mAngleX(0.0f)
	, mAngleY(0.0f)
	, mPrevX(0)
	, mPrevY(0)
	, mScrollWheelValue(0)
	, mView(DirectX::SimpleMath::Matrix::Identity)
	, mEye{0.0f}
	, mTarget{0.0f}
{
}

// デストラクタ
DebugCamera::~DebugCamera()
{
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void DebugCamera::Update()
{
	// どこかでMouseインスタンスが作られていれば、マウスの状態が取得できる(参考：Impl)
	auto state = DirectX::Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (state.leftButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	mPrevX = state.x;
	mPrevY = state.y;

	// マウスホイールのスクロール値を取得
	mScrollWheelValue = state.scrollWheelValue;
	if (mScrollWheelValue > 0)
	{
		mScrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列の算出
	CalculateViewMatrix();
}

//-------------------------------------------------------------------
// マウスポインタのドラッグ開始位置からの変位(相対値)
//-------------------------------------------------------------------
void DebugCamera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	// なお、0.2fは適当な補正値 -> ドラッグの移動量を調整する
	float dx = static_cast<float>(x - mPrevX) * 0.2f;
	float dy = static_cast<float>(y - mPrevY) * 0.2f;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		// XとYの関係に注意！！
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新
		mAngleX += angleX;
		mAngleY += angleY;
	}
}

//-------------------------------------------------------------------
// ビュー行列の算出と、カメラ座標・注視点の取得
//-------------------------------------------------------------------
void DebugCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(mAngleY);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(mAngleX);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(0.0f, 0.0f, 1.0f);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - mScrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	mEye = eye;
	mTarget = target;
	mView = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
