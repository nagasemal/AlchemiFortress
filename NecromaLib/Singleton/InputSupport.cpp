#include "pch.h"
#include "InputSupport.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "ShareData.h"

InputSupport* InputSupport::instance = nullptr;

InputSupport::InputSupport() :
	m_WorldScreenMouse(),
	m_nowLayer(),
	m_hitUIFlag()
{
}

void InputSupport::Create()
{
	if (instance == nullptr)
	{
		instance = new InputSupport;
	}
}

void InputSupport::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void InputSupport::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// UIに触れている
	if (m_hitUIFlag)
	{
		// 見えない位置まで持っていく
		m_WorldScreenMouse = SimpleMath::Vector3(1000000,0.0f,0.0f);
	}
	else
	{

		// ワールド空間座標に変換
		m_WorldScreenMouse = CalcScreenToXZN(m_mouseTracker.GetLastState().x,
			m_mouseTracker.GetLastState().y,
			pDR->GetOutputSize().right,
			pDR->GetOutputSize().bottom,
			pSD.GetView(), pSD.GetProjection());
	}

	// 触れられる状態に戻す
	m_hitUIFlag = false;
	// レイヤー番号を元に戻す
	m_nowLayer = 0;

}

void InputSupport::SetLayer(int layer)
{
	m_nowLayer = layer;
}

bool InputSupport::LeftButton_Press()
{
	return m_mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED;
}

bool InputSupport::LeftButton_Release()
{
	return m_mouseTracker.leftButton == Mouse::ButtonStateTracker::RELEASED;
}

bool InputSupport::LeftButton_Held()
{
	return m_mouseTracker.leftButton == Mouse::ButtonStateTracker::HELD;
}

bool InputSupport::RightButton_Press()
{
	return m_mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED;
}

bool InputSupport::RightButton_Release()
{
	return m_mouseTracker.leftButton == Mouse::ButtonStateTracker::RELEASED;
}

bool InputSupport::RightButton_Held()
{
	return m_mouseTracker.leftButton == Mouse::ButtonStateTracker::HELD;
}

bool InputSupport::MiddleButton_Press()
{
	return m_mouseTracker.middleButton == Mouse::ButtonStateTracker::PRESSED;
}

bool InputSupport::MiddleButton_Release()
{
	return m_mouseTracker.middleButton == Mouse::ButtonStateTracker::RELEASED;
}

bool InputSupport::MiddleButton_Held()
{
	return m_mouseTracker.middleButton == Mouse::ButtonStateTracker::HELD;
}

SimpleMath::Vector3 InputSupport::GetMousePosWolrd()
{
	return m_WorldScreenMouse;
}

SimpleMath::Vector2 InputSupport::GetMousePosScreen()
{
	return {(float)m_mouseTracker.GetLastState().x, (float)m_mouseTracker.GetLastState().y };
}
