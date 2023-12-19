#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "NecromaLib/GameData/SpriteCutter.h"

AM_Attacker::AM_Attacker():
	m_timer(),
	m_targetPos(),
	m_saveBulletStatus(),
	m_bulletStatus()
{
}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

	// Json����ǂݎ�����}�V���̃f�[�^��K������
	m_maxHp = m_hp = ShareJsonData::GetInstance().GetMachineData(m_machineID).hp;

	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

	// 1Lv���̃o���b�g�̃X�e�[�^�X���󂯎��
	m_bulletStatus = RecalculationStatus(m_element,1);

	// �ۑ�
	m_saveBulletStatus = m_bulletStatus;
}

void AM_Attacker::Update()
{
	// �����O�ɖ߂�
	m_powerUPFlag = false;
	m_bulletStatus = m_saveBulletStatus;

}

void AM_Attacker::SelectUpdate()
{
}

void AM_Attacker::Draw()
{
}

void AM_Attacker::Finalize()
{
}

void AM_Attacker::AllAlchemicalMachine(AlchemicalMachineObject* object)
{
		if (CircleCollider(object->GetMagicCircle(), GetCircle()))
		{
			
			m_powerUPFlag = true;

			//����
			m_bulletStatus.str		 = m_saveBulletStatus.str	  * 1.15f;
			m_bulletStatus.life		 = m_saveBulletStatus.life	  * 1.15f;
			m_bulletStatus.speed	 = m_saveBulletStatus.speed	  * 1.15f;
			m_bulletStatus.span		 = m_saveBulletStatus.span	  * 0.95f;
			m_bulletStatus.lossMp	 = (int)((float)m_saveBulletStatus.lossMp  / 2.0f);

		}
}

bool AM_Attacker::BulletRequest(std::list<EnemyObject>* enemys)
{

	DataManager* pDataM = DataManager::GetInstance();
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;

	//�@�������݂���G�l�~�[����
	//	���ʔ͈�to�G�l�~�[
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{

		// �_�E���L���X�g���s���AGameObject3D�^�ɕϊ�������̏����𓾂�
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		// ���@�ƃG�l�~�[�̓����蔻��
		if (hitMachine && !m_invincibleFlag)
		{
			// �_���[�W���󂯂�
			m_hp -= (int)it->GetPower();

			m_invincibleFlag = true;
		}

		if (CircleCollider(it->GetCircle(), m_magicCircle) && it->GetColliderActive())
		{
			// �X�p�����ɐ���
			if (m_timer >= m_bulletStatus.span && pDataM->GetNowMP() > 0)
			{
				SoundData& pSound = SoundData::GetInstance();

				pSound.PlaySE(ConvertToElement(m_element));

				m_timer = 0.0f;
				m_targetPos = it->GetPos();

				// ����MP��Lv�Ɉˑ�(�����قǏ����)
				pDataM->SetNowMP(pDataM->GetNowMP() - (int)(m_bulletStatus.lossMp * m_lv));

				return true;
			}
		}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	Bullet::BulletData data = {};
	data.damage	 = (float)m_bulletStatus.str;
	data.life	 = (float)m_bulletStatus.life;
	data.speed	 = (float)m_bulletStatus.speed;

	return Bullet(data, m_color, m_data.pos,m_targetPos);
}

void AM_Attacker::RenderUI()
{
}

void AM_Attacker::LvUp()
{
	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// Json����ǂݎ�����}�V���̃f�[�^��K������
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;
	// ���݃��x���ōČv�Z
	m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

	// HP����
	m_maxHp = (int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv));
	// HP��
	m_hp = m_maxHp;

}
