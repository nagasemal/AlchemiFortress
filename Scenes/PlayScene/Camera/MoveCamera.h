#pragma once

class MoveCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

	MoveCamera();
	~MoveCamera();

	void Initialize();
	// 更新処理
	void Update(bool scroll,bool move);

	// ビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const		{ return m_view; }
	// デバッグカメラの座標を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const		{ return m_eye; }
	// デバッグカメラの注視点座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const	{ return m_target; }

	void SetEyeProsition(DirectX::SimpleMath::Vector3 eye)			{ m_eye = eye;}
	void SetTargetProsition(DirectX::SimpleMath::Vector3 target)	{ m_target = target; }
	void SetSaveTargetProsition(DirectX::SimpleMath::Vector3 target) { m_saveTarget = target; }

	void TargetChange(DirectX::SimpleMath::Vector3 targetA, DirectX::SimpleMath::Vector3 targetB);
	void ResetTargetChangeTimer() { m_targetChangeTime = 0; }



private:

	// 縦回転角
	float m_angleX;								
	// 横回転角
	float m_angleY;								
	// 前回のマウス座標(X,Y)
	int m_prevX, m_prevY;						
	// マウスホイールのスクロール値
	int m_scrollWheelValue;				
	// 前回のスクロール値
	int m_prevWheelValue;						
	// 移動値
	DirectX::SimpleMath::Vector3 m_move;		
	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;			
	// カメラの座標
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

	// 前回の注視点
	DirectX::SimpleMath::Vector3 m_saveTarget;
	// 引きで使うタイマー
	float m_time;			
	// ターゲット変更に使うタイマー
	float m_targetChangeTime;

	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix();
};