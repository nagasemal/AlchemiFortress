#include "pch.h"
#include "AM_Upper.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#define LVUP_MAGNIFICATION_HP 1.25f
#define STANDARD_HP 30

AM_Upper::AM_Upper()
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	m_machineID = MACHINE_TYPE::UPEER;
	m_objectName = "Upper";

	m_magicCircle.r = 8.0f;

	ShareData& pSD = ShareData::GetInstance();
	// �x������
	pSD.GetDevice();


	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(80.f + ((float)i * 65.f), 560.f), DirectX::SimpleMath::Vector2(64.f, 64.f));
	}

	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);

}

void AM_Upper::Update()
{
	m_magicCircle.p = m_data.pos;

}

void AM_Upper::SelectUpdate()
{
	// �I����Ԃ��m�[�}���̎��Ȃ�΁A�����I�����[�h
	if (m_element == NOMAL)
	{
		m_selectBox[0]->HitMouse();
		m_selectBox[1]->HitMouse();
		m_selectBox[2]->HitMouse();
		m_selectBox[3]->HitMouse();

		//�@�O���t�@�C������ǂݍ��߂�悤�ɂ�����
		//�@�Α����擾
		if (m_selectBox[0]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0, 0, 1);

			m_element = FLAME;

		}
		//�@�������擾
		if (m_selectBox[1]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 0, 1, 1);

			m_element = AQUA;
		}
		//�@�������擾
		if (m_selectBox[2]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 1, 0, 1);

			m_element = WIND;

		}
		//�@�y�����擾
		if (m_selectBox[3]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0.5f, 0, 1);

			m_element = EARTH;
		}
	}
}

void AM_Upper::Draw()
{
}

void AM_Upper::Finalize()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	m_selectBox[i]->Finalize();
	//}
}

void AM_Upper::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	RECT rect_lv = SpriteCutter(64, 64, m_lv, 0);
	m_selectLvUpBox->DrawUI(texture, pSL.GetNumberTexture(), rect_lv);


	if (m_element != NOMAL) return;

	for (int i = 0; i < 4; i++)
	{
		SpriteLoder& pSL = SpriteLoder::GetInstance();

		RECT rect = SpriteCutter(64, 64, 2 + i, 0);

		m_selectBox[i]->DrawUI(texture, pSL.GetElementTexture(), rect);
	}
}

void AM_Upper::LvUp()
{
	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;

	// HP����
	m_maxHp = (int)(STANDARD_HP * LVUP_MAGNIFICATION_HP);
	// HP��
	m_hp = m_maxHp;
}
