#include "pch.h"
#include "UIKeyControl.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "Scenes/Commons/SelectionUI.h"

// �L�[�{�[�h�őI���\�ɂȂ�܂ł̎���
#define SELECT_TIME 20.0f

UIKeyControl::UIKeyControl():
	m_maxCol(),
	m_maxLows{0},
	m_nowCol(),
	m_nowLow(),
	m_prevMousePos(),
	m_keyContorlFlag(false)
{
}

UIKeyControl::~UIKeyControl()
{
}

void UIKeyControl::Update()
{
	InputSupport& pIS = InputSupport::GetInstance();
	m_nowCol -= pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Up);
	m_nowCol += pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Down);

	m_nowLow -= pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Left);
	m_nowLow += pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Right);

	m_nowCol = std::min(std::max(m_nowCol, 0), m_maxCol);
	m_nowLow = std::min(std::max(m_nowLow, 0), m_maxLows[m_nowCol]);

	// �}�E�X�𓮂����Ă��Ȃ���΃L�[�{�[�h�����ɂ���
	if (m_prevMousePos != pIS.GetMousePosScreen()) m_keyContorlFlag = false;

	// �O�t���[���̃}�E�X�ʒu��ۑ�����
	m_prevMousePos = pIS.GetMousePosScreen();

	// �\���L�[�������ꂽ��L�[�{�[�h���샂�[�h�ƂȂ�
	ArrowKeyPush();

	if (!m_keyContorlFlag) return;

	// �s�Ɨ�ɑΉ�����UI�̑I�����/������Ԃ�ς��܂�
	for (auto& selectionUI : m_selectionUIs)
	{
		
		// �G��Ȃ���ԂȂ�Ώ������΂�
		if (!selectionUI.ui->GetActiveFlag()) continue;

		selectionUI.ui->Update();

		if (selectionUI.col == m_nowCol && selectionUI.low == m_nowLow)
		{
			selectionUI.ui->SetHitMouseFlag(true);
			selectionUI.ui->SetSelectFlag(true);

			if (pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Enter))
			{
				selectionUI.ui->SetKeySelectFlag(true);
			}

		}
	}
}

void UIKeyControl::AddUI(SelectionUI* ui,int low, int col)
{
	UI_Data ui_data;

	ui_data.low = low;
	ui_data.col = col;
	ui_data.ui = ui;

	// �s��Max�l���X�V���� ����X�V����
	if (m_maxCol < col)
	{
		m_maxCol = col;
		m_maxLows.push_back(low);
	}
	if (m_maxLows[col] < low) m_maxLows[col] = low;

	m_selectionUIs.push_back(ui_data);
}

void UIKeyControl::DeleteUI(SelectionUI* ui)
{

}

bool UIKeyControl::ArrowKeyPush()
{
	if (!m_keyContorlFlag)
	{
		InputSupport& pIS = InputSupport::GetInstance();
		m_keyContorlFlag = pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Up)
			|| pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Down)
			|| pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Left)
			|| pIS.GetKeybordState().IsKeyPressed(DirectX::Keyboard::Right);
	}
	return false;
}
