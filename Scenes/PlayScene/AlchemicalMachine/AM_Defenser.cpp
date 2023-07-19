#include "pch.h"
#include "AM_Defenser.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#define LVUP_MAGNIFICATION_HP 1.45f
#define STANDARD_HP 100

// �C���ɂ����閂��
#define REPAIR_HP 50 * m_lv

AM_Defenser::AM_Defenser():
	m_isBreak()
{
}

AM_Defenser::~AM_Defenser()
{
}

void AM_Defenser::Initialize()
{
	m_machineID = MACHINE_TYPE::DEFENSER;
	m_objectName = "Defenser";

	m_hp = STANDARD_HP;
	m_maxHp = STANDARD_HP;

}

void AM_Defenser::Update()
{

	m_data.pos.y = -1.0f;

	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = 2.25f;
}

void AM_Defenser::SelectUpdate()
{

}

void AM_Defenser::Draw()
{
}

void AM_Defenser::Finalize()
{
}

void AM_Defenser::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	RECT rect_lv = SpriteCutter(64, 64, m_lv, 0);
	m_selectLvUpBox->DrawUI(texture, pSL.GetNumberTexture(), rect_lv);

	m_repairBox->DrawUI(texture);

}

void AM_Defenser::LvUp()
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

void AM_Defenser::EnemyHit(std::list<EnemyObject>* enemys)
{
	//�@�������݂���G�l�~�[����
	//	���ʔ͈�to�G�l�~�[
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{

		bool hitMachine = false;

		if (hitMachine = (CircleCollider(it->GetCircle(), m_magicCircle)))
		{
			it->HitMachine(hitMachine);
			it->Bouns();

			// �̗͌���
			m_hp -= it->GetPower();

		}
	}
}
