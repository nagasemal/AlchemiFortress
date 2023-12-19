#include "pch.h"
#include "AM_Defenser.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "NecromaLib/Singleton/DeltaTime.h"

AM_Defenser::AM_Defenser():
	m_isBreak(),
	m_counterFlag()
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
	float machineHP = (float)ShareJsonData::GetInstance().GetMachineData(m_machineID).hp;

	m_hp = (int)machineHP;
	m_maxHp = (int)machineHP;

}

void AM_Defenser::Update()
{
	m_data.pos.y = 0.0f;
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

void AM_Defenser::RenderUI()
{

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

	m_counterFlag = false;

	//�@�������݂���G�l�~�[����
	//	���ʔ͈�to�G�l�~�[
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		// �_�E���L���X�g���s���AGameObject3D�^�ɕϊ�������̏����𓾂�
		bool hitMachine = CircleCollider(GetObject3D(),it->GetObject3D());

		if (hitMachine)
		{
			it->HitMachine(hitMachine);
			it->Bouns();

			if (m_invincibleFlag) continue;

			// �̗͌���
			m_hp -= (int)it->GetPower();
			it->HitBullet(it->GetPos() - m_data.pos, m_lv);
			m_invincibleFlag = true;
			m_counterFlag = true;
		}
	}
}

bool AM_Defenser::CounterAttack()
{
	return m_counterFlag;
}
