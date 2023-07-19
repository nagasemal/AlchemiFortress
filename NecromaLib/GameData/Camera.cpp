//
// Camera.cpp
//
#include "pch.h"
#include "Camera.h"

#include "DeviceResources.h"
#include "NecromaLib/Singleton/ShareData.h"


using namespace DirectX;

// 定数の初期化
const float Camera::DEFAULT_CAMERA_DISTANCE = 5.0f;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
Camera::Camera()
	: mView(DirectX::SimpleMath::Matrix::Identity)
	, mEye(0.0f, 15.0f, 1.0f)
	, mTarget(0.0f, 0.0f, 0.0f)
	, mUp(0.0f, 1.0f, 0.0f)
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}


//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
Camera::~Camera()
{
}

//-------------------------------------------------------------------
// ビュー行列の算出
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	mView = DirectX::SimpleMath::Matrix::CreateLookAt(mEye, mTarget, mUp);
}

//-------------------------------------------------------------------
// プロジェクション行列の算出
//-------------------------------------------------------------------
void Camera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = ShareData::GetInstance().GetDeviceResources();

	// ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // 画角
	float aspectRatio = width / height;							// 画面縦横比
	float nearPlane = 1.0f;                                     // カメラから一番近い投影面
	float farPlane = 300.0f;                                    // カメラから一番遠い投影面

	mProjection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

}
