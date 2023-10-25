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

	if (m_hitUIFlag)
	{
		// å©Ç¶Ç»Ç¢à íuÇ‹Ç≈éùÇ¡ÇƒÇ¢Ç≠
		m_WorldScreenMouse = SimpleMath::Vector3(1000000,0.0f,0.0f);
	}
	else
	{

		// ïœä∑ÇµÇƒìnÇ∑
		m_WorldScreenMouse = CalcScreenToXZN(m_mouseTracker.GetLastState().x,
			m_mouseTracker.GetLastState().y,
			pDR->GetOutputSize().right,
			pDR->GetOutputSize().bottom,
			pSD.GetView(), pSD.GetProjection());
	}

	// êGÇÍÇÁÇÍÇÈèÛë‘Ç…ñﬂÇ∑
	m_hitUIFlag = false;
	// å≥Ç…ñﬂÇ∑
	m_nowLayer = 0;

}

void InputSupport::SetLayer(int layer)
{
	m_nowLayer = layer;
}

SimpleMath::Vector3 InputSupport::GetMousePosWolrd()
{
	return m_WorldScreenMouse;
}

SimpleMath::Vector2 InputSupport::GetMousePosScreen()
{
	return {(float)m_mouseTracker.GetLastState().x, (float)m_mouseTracker.GetLastState().y };
}
