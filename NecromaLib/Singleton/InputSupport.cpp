#include "pch.h"
#include "InputSupport.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "ShareData.h"

InputSupport* InputSupport::instance = nullptr;

InputSupport::InputSupport()
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

DirectX::SimpleMath::Vector3 InputSupport::GetMousePosWolrd()
{
	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// •ÏŠ·‚µ‚Ä“n‚·
	return CalcScreenToXZN(	m_mouseTracker.GetLastState().x,
							m_mouseTracker.GetLastState().y,
							pDR->GetOutputSize().right,
							pDR->GetOutputSize().bottom,
							pSD.GetView(), pSD.GetProjection());
}
