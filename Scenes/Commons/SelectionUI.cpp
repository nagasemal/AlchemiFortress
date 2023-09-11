#include "pch.h"
#include "SelectionUI.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/SpriteLoder.h"

SelectionUI::SelectionUI() :
	m_selectFlag(),
	m_hitMouseFlag(),
	m_luminousFlag(),
	m_keySelectFlag(),
	m_activeFlag(true),
	m_rect{0,0,64,64}
{
}

SelectionUI::~SelectionUI()
{
}

void SelectionUI::Initialize()
{
}

//void SelectionUI::Update()
//{
//
//}

void SelectionUI::Finalize()
{
}

bool SelectionUI::HitMouse()
{
	// active状態でなければ処理を飛ばす
	if (!m_activeFlag)return false;

	InputSupport& pIS = InputSupport::GetInstance();
	return m_hitMouseFlag = HitObject_RageSet(pIS.GetMousePosScreen(), static_cast<float>(m_rect.right), static_cast<float>(m_rect.bottom), m_data.rage);
}

bool SelectionUI::ClickMouse()
{
	InputSupport& pIS = InputSupport::GetInstance();
	bool leftFlag = pIS.GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED;
	//m_keySelectFlag = pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Enter);

	return (m_hitMouseFlag && leftFlag) || m_keySelectFlag;
}

bool SelectionUI::SelectionMouse()
{
	if (ClickMouse() && !m_selectFlag)
	{
		m_selectFlag = true;
		return m_selectFlag;
	}

	if (ClickMouse() && m_selectFlag)
	{
		m_selectFlag = false;
	}

	return m_selectFlag;
}

bool SelectionUI::HoldMouse()
{
	InputSupport& pIS = InputSupport::GetInstance();
	bool leftFlag = pIS.GetMouseState().leftButton;

	return m_hitMouseFlag && leftFlag;
}

bool SelectionUI::ExitMouse()
{
	InputSupport& pIS = InputSupport::GetInstance();
	bool exitFlag = false;

	if (m_hitMouseFlag)
	{
		exitFlag = !HitObject_RageSet(pIS.GetMousePosScreen(), 64, 64, m_data.rage);
	}

	return exitFlag;
}


