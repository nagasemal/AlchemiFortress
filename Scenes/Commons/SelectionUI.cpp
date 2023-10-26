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
	//// active��ԂłȂ���Ώ������΂�
	//if (!m_activeFlag)return false;

	InputSupport& pINP = InputSupport::GetInstance();

	m_hitMouseFlag = HitObject_RageSet(pINP.GetMousePosScreen(), static_cast<float>(m_rect.right), static_cast<float>(m_rect.bottom), m_data.rage);

	if (m_hitMouseFlag) pINP.HitUI();

	// �G��Ă���Ԃ̓��[���h��ԍ��W�ɉe�����y�ڂ��Ȃ��悤�ɂ���
	// layer�`�F�b�N��ԂȂ�Βǉ��Ń`�F�b�N���s��
	if (m_hitMouseFlag && layerCheck)
	{
		// ���g�̕����傫����΃��C���[�����X�V���� �����łȂ���Ώ�����ʂ��Ȃ�
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
		// �A�N�e�B�u��
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


