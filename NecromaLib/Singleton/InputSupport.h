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

	//===[ マウス座標(ワールド空間) ]===//
	SimpleMath::Vector3 GetMousePosWolrd();

	//===[ マウス座標(スクリーン空間) ]===//
	SimpleMath::Vector2 GetMousePosScreen();

	// UIに触れると、設定されているレイヤーの番号が入る
	int m_nowLayer;

	bool m_hitUIFlag;

};