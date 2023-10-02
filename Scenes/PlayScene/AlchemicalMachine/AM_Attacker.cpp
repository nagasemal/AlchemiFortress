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

	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i] = std::make_unique<SelectionBox>(SimpleMath::Vector2(80.f + ((float)i * 65.f), 560.f), SimpleMath::Vector2(1.0f, 1.0f));
	}

	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

	// 1Lv���̃o���b�g�̃X�e�[�^�X���󂯎��
	m_bulletStatus = RecalculationStatus(m_element,1);

	// �ۑ�
	m_saveBulletStatus = m_bulletStatus;
}

void AM_Attacker::Update()
{
	// Json����ǂݎ�����}�V���̃f�[�^��K������
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// �����O�ɖ߂�
	m_powerUPFlag = false;
	m_bulletStatus = m_saveBulletStatus;

	m_magicCircle.p = m_data.pos;

	// ���ʔ͈͂����肷��
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage + (float)(m_lv / 2.0f);

}

void AM_Attacker::SelectUpdate()
{

	// �I����Ԃ��m�[�}���̎��Ȃ�΁A�����I�����[�h
	if (m_element == NOMAL)
	{
		m_selectBox[0]->HitMouse();
		m_selectBox[1]->HitMouse();
		m_selectBox[2]->HitMouse();
		m_selectBox[3]->HitMouse();

		//�@�@�O���t�@�C������ǂݍ��߂�悤�ɂ�����
		//�@Nomal���Ȃ����߂�1����X�^�[�g
		for (int i = 1; i < ELEMENT::Num; i++)
		{
			if (m_selectBox[i - 1]->ClickMouse())
			{
				// ���X�V
				m_element = (ELEMENT)i;
				m_color = ShareJsonData::GetInstance().GetElementColor(m_element);
				m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

			}
		}
	}
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
			m_bulletStatus.str		 = m_saveBulletStatus.str	  * 1.5f;
			m_bulletStatus.life		 = m_saveBulletStatus.life	  * 1.5f;
			m_bulletStatus.speed	 = m_saveBulletStatus.speed	  * 1.5f;
			m_bulletStatus.span		 = m_saveBulletStatus.span	  * 0.8f;
			m_bulletStatus.lossMp	 = (int)((float)m_saveBulletStatus.lossMp  * 0.35f);
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
		// ���@�ƃG�l�~�[�̓����蔻��
		if (CircleCollider(it->GetCircle(), GetCircle()) && !m_invincibleFlag)
		{
			// �_���[�W���󂯂�
			m_hp -= (int)it->GetPower();

			m_invincibleFlag = true;
		}

		if (CircleCollider(it->GetCircle(), m_magicCircle))
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

void AM_Attacker::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	if (m_element != NOMAL) return;

	for (int i = 0; i < 4; i++)
	{
		RECT rect_element = SpriteCutter(64, 64, 2 + i, 0);

		m_selectBox[i]->DrawUI(texture, pSL.GetElementTexture(), rect_element);
	}
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
