#include "pch.h"
#include "AM_Defenser.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

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

	// Json����ǂݎ�����}�V���̃f�[�^��K������
	float machineHP = ShareJsonData::GetInstance().GetMachineData(m_machineID).hp;

	m_hp = (int)machineHP;
	m_maxHp = (int)machineHP;

}

void AM_Defenser::Update()
{
	// Json����ǂݎ�����}�V���̃f�[�^��K������
	float effectRage = ShareJsonData::GetInstance().GetMachineData(m_machineID).effect_rage;

	m_data.pos.y = 0.0f;

	m_magicCircle.p = m_data.pos;

	// ���ʔ͈͂����肷��
	m_magicCircle.r = effectRage;
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
	m_dismantlingBox->DrawUI(texture);

}

void AM_Defenser::LvUp()
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
			m_hp -= (int)it->GetPower();

		}
	}
}
