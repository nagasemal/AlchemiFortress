//
// Camera.h
//
#pragma once

#include <SimpleMath.h>

/// <summary>
/// デバッグ用カメラクラス
/// </summary>
class Camera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix mView;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix mProjection;

	// カメラ座標
	DirectX::SimpleMath::Vector3 mEye;

	// 注視点
	DirectX::SimpleMath::Vector3 mTarget;

	// 上向きベクトル
	DirectX::SimpleMath::Vector3 mUp;

public:
	// コンストラクタ
	Camera();

	// デストラクタ
	virtual ~Camera();

	// ビュー行列のアクセサ
	void SetViewMatrix(DirectX::SimpleMath::Matrix view) { mView = view; }

	DirectX::SimpleMath::Matrix GetViewMatrix()
	{
		CalculateViewMatrix();
		return mView;
	}

	// プロジェクション行列のアクセサ
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { mProjection = projection; }

	DirectX::SimpleMath::Matrix GetProjectionMatrix()
	{
		CalculateProjectionMatrix();
		return mProjection;
	}

	// カメラ座標のアクセサ
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye) { mEye = eye; }
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return mEye; }

	// 注視点座標のアクセサ
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { mTarget = target; }
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return mTarget; }

	// 上向きベクトルのアクセサ
	void SetUpVector(DirectX::SimpleMath::Vector3 up) { mUp = up; }
	DirectX::SimpleMath::Vector3 GetUpVector() const { return mUp; }

protected:

	// ビュー行列の算出
	virtual void CalculateViewMatrix();

	// プロジェクション行列の算出
	virtual void CalculateProjectionMatrix();
};
