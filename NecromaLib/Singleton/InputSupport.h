#pragma once
class InputSupport
{
private:

	InputSupport();
	static InputSupport* instance;

	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;

	//ワールド空間座標上のマウス
	SimpleMath::Vector3 m_WorldScreenMouse;

	// UIに触れると、設定されているレイヤーの番号が入る
	int m_nowLayer;

	bool m_hitUIFlag;

public:
	~InputSupport() = default;

	InputSupport(const InputSupport&) = delete;
	InputSupport& operator=(const InputSupport&) = delete;

	static void Create();
	static void Destroy();

	static InputSupport& GetInstance() { return *instance; }

	void Update();

	void SetLayer(int layer);
	bool GetLayer() { return m_nowLayer; }

	void HitUI() { m_hitUIFlag = true; }
	void ExitUI() { m_hitUIFlag = false; }

	//===[ キーボード ]===//
	void SetKeybordState(DirectX::Keyboard::KeyboardStateTracker tracker)	{m_keyboardTracker = tracker; }
	DirectX::Keyboard::KeyboardStateTracker GetKeybordState()				{ return m_keyboardTracker; }

	//===[ マウス ]===//
	void SetMouseState(DirectX::Mouse::ButtonStateTracker tracker) { m_mouseTracker = tracker; }
	DirectX::Mouse::ButtonStateTracker GetMouseState() { return m_mouseTracker; }

public:

	// マウス左が押された瞬間
	bool LeftButton_Press();
	// マウス左が離された瞬間
	bool LeftButton_Release();
	// マウス左が押され続けている
	bool LeftButton_Held();

	// マウス右が押された瞬間
	bool RightButton_Press();
	// マウス右が離された瞬間
	bool RightButton_Release();
	// マウス右が押され続けている
	bool RightButton_Held();

	// マウスホイールが押された瞬間
	bool MiddleButton_Press();
	// マウスホイールが離された瞬間
	bool MiddleButton_Release();
	// マウスホイールが押され続けている
	bool MiddleButton_Held();

	//===[ マウス座標(ワールド空間) ]===//
	SimpleMath::Vector3 GetMousePosWolrd();

	//===[ マウス座標(スクリーン空間) ]===//
	SimpleMath::Vector2 GetMousePosScreen();

};