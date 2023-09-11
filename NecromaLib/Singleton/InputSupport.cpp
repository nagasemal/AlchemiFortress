#include "pch.h"
#include "InputSupport.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "ShareData.h"

InputSupport* InputSupport::instance = nullptr;

InputSupport::InputSupport():
	m_WorldScreenMouse()
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

	// •ÏŠ·‚µ‚Ä“n‚·
	m_WorldScreenMouse = CalcScreenToXZN(m_mouseTracker.GetLastState().x,
										 m_mouseTracker.GetLastState().y,
										 pDR->GetOutputSize().right,
										 pDR->GetOutputSize().bottom,
										 pSD.GetView(), pSD.GetProjection());
}

SimpleMath::Vector3 InputSupport::GetMousePosWolrd()
{
	return m_WorldScreenMouse;
}

SimpleMath::Vector2 InputSupport::GetMousePosScreen()
{
	return {(float)m_mouseTracker.GetLastState().x, (float)m_mouseTracker.GetLastState().y };
}
