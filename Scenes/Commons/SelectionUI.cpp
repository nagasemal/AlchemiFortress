#include "pch.h"
#include "SelectionUI.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"

SelectionUI::SelectionUI() :
	m_selectFlag(),
	m_hitMouseFlag(),
	m_luminousFlag(),
	m_keySelectFlag(),
	m_layer(),
	m_activeFlag(true),
	m_rect{ 0,0,64,64 }
{
}

SelectionUI::~SelectionUI()
{
}

void SelectionUI::Initialize()
{
}

void SelectionUI::Finalize()
{
}

bool SelectionUI::HitMouse(bool layerCheck)
{
	//// active状態でなければ処理を飛ばす
	//if (!m_activeFlag)return false;

	InputSupport& pINP = InputSupport::GetInstance();

	m_hitMouseFlag = HitObject_RageSet(pINP.GetMousePosScreen(), static_cast<float>(m_rect.right), static_cast<float>(m_rect.bottom), m_data.rage);

	if (m_hitMouseFlag) pINP.HitUI();

	// 触れている間はワールド空間座標に影響を及ぼさないようにする
	// layerチェック状態ならば追加でチェックを行う
	if (m_hitMouseFlag && layerCheck)
	{
		// 自身の方が大きければレイヤー数を更新する そうでなければ処理を通さない
		if (m_layer >= pINP.GetLayer())	pINP.SetLayer(m_layer);
		else return m_hitMouseFlag = false;

	}

	return m_hitMouseFlag;
}

bool SelectionUI::ClickMouse()
{
	InputSupport& pINP = InputSupport::GetInstance();

	if ((m_hitMouseFlag && pINP.LeftButton_Press()) || m_keySelectFlag)
	{
		// アクティブ時
		if (m_activeFlag)
		{
			SoundData::GetInstance().PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE_BUTTOMPUSH);
			return true;
		}
		else
		{
			SoundData::GetInstance().PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE_POPTEXT);
			return false;
		}
	}

	return false;
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
	InputSupport& pINP = InputSupport::GetInstance();
	bool leftFlag = pINP.GetMouseState().leftButton;

	return m_hitMouseFlag && leftFlag;
}

bool SelectionUI::ExitMouse()
{
	InputSupport& pINP = InputSupport::GetInstance();
	bool exitFlag = false;

	if (m_hitMouseFlag)
	{
		exitFlag = !HitObject_RageSet(pINP.GetMousePosScreen(), 64, 64, m_data.rage);
	}

	return exitFlag;
}


