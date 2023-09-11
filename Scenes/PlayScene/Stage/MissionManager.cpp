#include "pch.h"
#include "MissionManager.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/SelectScene/MissionRender.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

MissionManager::MissionManager():
	m_machineCondition(),
	m_enemyCondition(),
	m_timeCondition(),
	m_allClearFlag(),
	m_missionNum(),
	m_missionSituation(),
	m_timer(),
	m_failureFlag()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::Initialize()
{

	auto pSJD = &ShareJsonData::GetInstance();

	// �擾���������R�s�[����
	m_machineCondition = pSJD->GetStageData().condition_Machine;
	m_enemyCondition   = pSJD->GetStageData().condition_Enemy;
	m_timeCondition    = pSJD->GetStageData().condition_Time;

	// �X�^���_�[�h�Ȃ��True�ɂ���
	bool timeMission = m_timeCondition[0].condition == "Standerd";

	m_missionNum = (int)m_machineCondition.size() +
				   (int)m_enemyCondition.size() + 
				   (int)timeMission;

	m_missionSituation = 0;

	m_allClearFlag = false;

	m_timeRender = std::make_unique<Number>(SimpleMath::Vector2{220.0f,220.0f}, SimpleMath::Vector2{1.0f,1.0f});

	m_missionRender = std::make_unique<MissionRender>(SimpleMath::Vector2{ 100.0f,300.0f }, SimpleMath::Vector2{ 1.0f,1.0f });

}

void MissionManager::Update(AlchemicalMachineManager* alchemicalManager, EnemyManager* enemyManager)
{
	auto pDeltaT = &DeltaTime::GetInstance();

	m_timer += pDeltaT->GetDeltaTime();

	// None�ȊO�Ȃ�Βʂ� �}�V�����ݒu���ꂽ�ۂ̏���
	if (alchemicalManager->SpawnMachineNotification() != MACHINE_TYPE::NONE)
	{
		MachineMission(alchemicalManager);
	}

	// Enemy���|���ꂽ�ۂɒʂ�
	if (enemyManager->GetKnokDownEnemyType() != ENEMY_TYPE::ENMEY_NONE)
	{
		EnemyMission(enemyManager);
	}

	// ���Ԑ����̏���
	if (m_timeCondition.size() > 0)
	{
		TimerMission();
	}
}

void MissionManager::Render()
{

	ShareData& pSD = ShareData::GetInstance();

	// ���Ԍv��
	m_timeRender->SetNumber((int)m_timer);
	m_timeRender->Render();


	// �~�b�V�������e�̕`��
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	m_missionRender->Render_MachineMission(m_machineCondition);
	m_missionRender->Render_EnemyMission(m_enemyCondition);
	m_missionRender->Render_TimerMission(m_timeCondition);
	m_missionRender->LineReset();
	pSD.GetSpriteBatch()->End();


}

bool MissionManager::MissionComplete()
{
	return m_missionNum <= m_missionSituation;
}

bool MissionManager::MissionmFailure()
{
	return m_failureFlag;
}

int MissionManager::GetStartTimer()
{
	return m_timeCondition[0].progress;
}

void MissionManager::MachineMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���
	for (int i = 0; i < m_machineCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_machineCondition[i].condition) ==
			alchemicalManager->SpawnMachineNotification() &&
			m_machineCondition[i].progress < m_machineCondition[i].value)
		{
			m_machineCondition[i].progress++;

			// �U������
			if (m_machineCondition[i].progress >= m_machineCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::EnemyMission(EnemyManager* enemyManager)
{

	// �Ή����������True�ɂ���
	for (int i = 0; i < m_enemyCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeEnemy(m_enemyCondition[i].condition) ==
			enemyManager->GetKnokDownEnemyType() &&
			m_enemyCondition[i].progress < m_enemyCondition[i].value)
		{
			// ����t���[�����ɕ����ΓG�����ꂽ�Ƃ��Ă��Ή��\�ɂ���
			m_enemyCondition[i].progress += enemyManager->GetKnokDownEnemyFlag();

			// �U������
			if (m_enemyCondition[i].progress >= m_enemyCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}

}

void MissionManager::TimerMission()
{

	// �����c��Ώ����n�^�C�}�[
	if (m_timeCondition[0].progress < m_timeCondition[0].value)
	{
		// ���b�i�s�x���グ��
		m_timeCondition[0].progress = (int)m_timer;

		if (m_timeCondition[0].progress >= m_timeCondition[0].value)
		{

			if (m_timeCondition[0].condition == "Standerd") m_missionSituation++;
			if (m_timeCondition[0].condition == "Limit") m_failureFlag = true;
			
		}
	}
}
