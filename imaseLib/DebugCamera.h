//-------------------------------------------------------------------
// @file		DebugCamera.h
// @brief		デバッグカメラ
// @date		2020/08/04
// @copyright	(c)2020 TRIDENT,Department of Game Science.
//-------------------------------------------------------------------
#pragma once

#include <SimpleMath.h>
#include <Mouse.h>

/// <summary>
/// デバッグ用カメラクラス
/// </summary>
class DebugCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

private:
	float mAngleX;								// 縦回転角
	float mAngleY;								// 横回転角
	int mPrevX, mPrevY;							// 前回のマウス座標(X,Y)
	int mScrollWheelValue;						// マウスホイールのスクロール値
	DirectX::SimpleMath::Matrix mView;			// 生成されたビュー行列
	DirectX::SimpleMath::Vector3 mEye;			// カメラの座標
	DirectX::SimpleMath::Vector3 mTarget;		// 注視点

public:
	// コンストラクタ
	DebugCamera();
	// デストラクタ
	~DebugCamera();

	// 更新処理
	void Update();

	// ビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const		{ return mView; }
	// デバッグカメラの座標を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const		{ return mEye; }
	// デバッグカメラの注視点座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const	{ return mTarget; }

private:
	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix();
};
