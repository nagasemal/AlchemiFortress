#pragma once
class InputSupport
{
private:

	InputSupport();
	static InputSupport* instance;

	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;

	//���[���h��ԍ��W��̃}�E�X
	DirectX::SimpleMath::Vector3 m_WorldScreenMouse;

public:
	~InputSupport() = default;

	InputSupport(const InputSupport&) = delete;
	InputSupport& operator=(const InputSupport&) = delete;

	static void Create();
	static void Destroy();

	static InputSupport& GetInstance() { return *instance; }

	void Update();

	//===[ �L�[�{�[�h ]===//
	void SetKeybordState(DirectX::Keyboard::KeyboardStateTracker tracker)	{m_keyboardTracker = tracker; }
	DirectX::Keyboard::KeyboardStateTracker GetKeybordState()				{ return m_keyboardTracker; }

	//===[ �}�E�X ]===//
	void SetMouseState(DirectX::Mouse::ButtonStateTracker tracker) { m_mouseTracker = tracker; }
	DirectX::Mouse::ButtonStateTracker GetMouseState() { return m_mouseTracker; }

	//===[ �}�E�X���W(���[���h���) ]===//
	DirectX::SimpleMath::Vector3 GetMousePosWolrd();

};