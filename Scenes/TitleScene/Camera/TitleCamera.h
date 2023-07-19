#pragma once

class Animation;

class TitleCamera
{
public:

	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

	TitleCamera();
	~TitleCamera();

	void Initialize();
	// 更新処理
	void Update();

	// ビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_view; }
	// デバッグカメラの座標を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return m_eye; }
	// デバッグカメラの注視点座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_target; }

private:

	float m_angleX;								// 縦回転角
	float m_angleY;								// 横回転角
	int m_prevX, m_prevY;						// 前回のマウス座標(X,Y)
	int m_scrollWheelValue;						// マウスホイールのスクロール値
	int m_prevWheelValue;						// 前回のスクロール値
	DirectX::SimpleMath::Vector3 m_move;		// 移動値
	DirectX::SimpleMath::Matrix m_view;			// 生成されたビュー行列
	DirectX::SimpleMath::Vector3 m_eye;			// カメラの座標
	DirectX::SimpleMath::Vector3 m_target;		// 注視点

	float m_animationTimer;								// 引きで使うタイマー

	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix();

};