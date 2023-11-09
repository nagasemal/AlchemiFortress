#pragma once
class InputSupport
{
private:

	InputSupport();
	static InputSupport* instance;

	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;

	//���[���h��ԍ��W��̃}�E�X
	SimpleMath::Vector3 m_WorldScreenMouse;

	// UI�ɐG���ƁA�ݒ肳��Ă��郌�C���[�̔ԍ�������
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

	//===[ �L�[�{�[�h ]===//
	void SetKeybordState(DirectX::Keyboard::KeyboardStateTracker tracker)	{m_keyboardTracker = tracker; }
	DirectX::Keyboard::KeyboardStateTracker GetKeybordState()				{ return m_keyboardTracker; }

	//===[ �}�E�X ]===//
	void SetMouseState(DirectX::Mouse::ButtonStateTracker tracker) { m_mouseTracker = tracker; }
	DirectX::Mouse::ButtonStateTracker GetMouseState() { return m_mouseTracker; }

public:

	// �}�E�X���������ꂽ�u��
	bool LeftButton_Press();
	// �}�E�X���������ꂽ�u��
	bool LeftButton_Release();
	// �}�E�X���������ꑱ���Ă���
	bool LeftButton_Held();

	// �}�E�X�E�������ꂽ�u��
	bool RightButton_Press();
	// �}�E�X�E�������ꂽ�u��
	bool RightButton_Release();
	// �}�E�X�E�������ꑱ���Ă���
	bool RightButton_Held();

	// �}�E�X�z�C�[���������ꂽ�u��
	bool MiddleButton_Press();
	// �}�E�X�z�C�[���������ꂽ�u��
	bool MiddleButton_Release();
	// �}�E�X�z�C�[���������ꑱ���Ă���
	bool MiddleButton_Held();

	//===[ �}�E�X���W(���[���h���) ]===//
	SimpleMath::Vector3 GetMousePosWolrd();

	//===[ �}�E�X���W(�X�N���[�����) ]===//
	SimpleMath::Vector2 GetMousePosScreen();

};