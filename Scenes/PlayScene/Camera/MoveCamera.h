#pragma once

class MoveCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

	MoveCamera();
	~MoveCamera();

	void Initialize();
	// 更新処理
	void Update();

	// ビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const		{ return m_View; }
	// デバッグカメラの座標を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const		{ return m_Eye; }
	// デバッグカメラの注視点座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const	{ return m_Target; }

private:

	float m_AngleX;								// 縦回転角
	float m_AngleY;								// 横回転角
	int m_PrevX, m_PrevY;						// 前回のマウス座標(X,Y)
	int m_ScrollWheelValue;						// マウスホイールのスクロール値
	DirectX::SimpleMath::Vector3 m_Move;		// 移動値
	DirectX::SimpleMath::Matrix m_View;			// 生成されたビュー行列
	DirectX::SimpleMath::Vector3 m_Eye;			// カメラの座標
	DirectX::SimpleMath::Vector3 m_Target;		// 注視点

	float m_time;								// 引きで使うタイマー

	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix();
};