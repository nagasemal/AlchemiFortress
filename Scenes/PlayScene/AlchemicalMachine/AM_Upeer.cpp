#include "pch.h"
#include "AM_Upper.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

AM_Upper::AM_Upper()
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	// Json����ǂݎ�����}�V���̃f�[�^��K������
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_machineID = MACHINE_TYPE::UPPER;
	m_objectName = "Upper";

	// Json����ǂݎ�����}�V���̃f�[�^��K������
	m_maxHp = m_hp = pSJD.GetMachineData(m_machineID).hp;

	// ���ʔ͈͂�Lv�ɉ����Č��߂�
	m_magicCircle.r = pSJD.GetMachineData(m_machineID).effect_rage * (pSJD.GetMachineData(m_machineID).multiplier_effect * m_lv);

	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i] = std::make_unique<SelectionBox>(SimpleMath::Vector2(80.f + ((float)i * 65.f), 560.f), SimpleMath::Vector2(1.f, 1.f));
	}

	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

}

void AM_Upper::Update()
{
	// Json����ǂݎ�����}�V���̃f�[�^��K������
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_magicCircle.p = m_data.pos;
	// ���ʔ͈͂����肷��
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage + (float)(m_lv / 2.0f);

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
		//�@Nomal���Ȃ����߂�1����X�^�[�g
		for (int i = 1; i < ELEMENT::Num; i++)
		{
			if (m_selectBox[i - 1]->ClickMouse())
			{
				// ���X�V
				m_element = (ELEMENT)i;
				m_color = ShareJsonData::GetInstance().GetElementColor(m_element);
			}
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

	if (m_element != NOMAL) return;

	for (int i = 0; i < 4; i++)
	{
		RECT rect_element = SpriteCutter(64, 64, 2 + i, 0);

		m_selectBox[i]->DrawUI(texture, pSL.GetElementTexture(), rect_element);
	}
}

void AM_Upper::HitEnemy(std::list<EnemyObject>* enemy)
{
	//�@�������݂���G�l�~�[����
//	���ʔ͈�to�G�l�~�[
	for (std::list<EnemyObject>::iterator it = enemy->begin(); it != enemy->end(); it++)
	{

		if (CircleCollider(it->GetCircle(), GetCircle()))
		{
			if (!m_invincibleFlag)
			{
				// �̗͌���
				m_hp -= (int)it->GetPower();
				m_invincibleFlag = true;
			}
		}
	}
}

void AM_Upper::LvUp()
{
	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// Json����ǂݎ�����}�V���̃f�[�^��K������
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;

	// HP����
	m_maxHp = (int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv));
	// HP��
	m_hp = m_maxHp;

}
