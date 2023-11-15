#include "pch.h"
#include "MissionManager.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/Commons/DrawArrow.h"

#include "Scenes/SelectScene/MissionRender.h"

#include "Scenes/TitleScene/TitleLogo/Veil.h"

#include "Scenes/Commons/DrawTimer.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define MISSION_RENDERPOS SimpleMath::Vector2(100.0f,260.0f)
#define MISSION_CLOSEBUTTON SimpleMath::Vector2(260.0f,10.0f)

MissionManager::MissionManager() :
	m_machineCondition(),
	m_enemyCondition(),
	m_timeCondition(),
	m_allClearFlag(),
	m_missionNum(),
	m_missionSituation(),
	m_timer(),
	m_failureFlag(),
	m_baseHP(),
	m_lastWave(),
	m_wave(1),
	m_waveAnimation(),
	m_nextWaveFlag(),
	m_closeAnimation(),
	m_closeMission(false),
	m_closeButton()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::Initialize()
{

	// �f�o�C�X�Ɖ�ʃT�C�Y�̊m��
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width	= device->GetOutputSize().right;
	int height	= device->GetOutputSize().bottom;

	// �X�e�[�W���̍ēx�ǂݍ���
	ReloadWave();

	m_timeRender = std::make_unique<DrawTimer>(SimpleMath::Vector2{ 140.0f,160.0f }, SimpleMath::Vector2{ 0.6f,0.6f });

	m_missionRender = std::make_unique<MissionRender>(MISSION_RENDERPOS, SimpleMath::Vector2{ 1.0f,1.0f });

	// �N���A�����o�̐���
	m_backVeil = std::make_unique<Veil>(3);
	m_backVeil->Create(L"Resources/Textures/Fade.png");
	m_backVeil->LoadShaderFile(L"Veil");
	m_backVeil->SetWindowSize(width, height);
	m_backVeil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_backVeil->SetScale(SimpleMath::Vector2((float)width, (float)height / 5.0f));
	m_backVeil->SetPosition(SimpleMath::Vector2(0.0f, height / 2.5f));

	// Wave�N���A���I�o�̐���
	m_nextWaveTexture = std::make_unique<UserInterface>();
	m_nextWaveTexture->Create(device,
							  L"Resources/Textures/NextWave.png",
							  SimpleMath::Vector2(width / 1.3f,height / 1.1f),
							  SimpleMath::Vector2(0.5f,0.5f),
							  UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWaveTexture->SetWindowSize(width,height);
	m_nextWaveTexture->SetColor(SimpleMath::Color(0.0f, 0.6f, 1.0f, 1.0f));


	// �X�e�[�W���s�������̃A�j���[�V�����p�ϐ�
	m_clearAnimation.max = 2.0f;

	m_closeButton = std::make_unique<DrawArrow>(MISSION_RENDERPOS + MISSION_CLOSEBUTTON,SimpleMath::Vector2(1.0f,1.0),2);
	m_closeButton->Initialize();
}

void MissionManager::Update(AlchemicalMachineManager* pAlchemicalManager, EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager)
{
	auto pDeltaT = &DeltaTime::GetInstance();
	m_lastWave = ShareJsonData::GetInstance().GetStageData().lastWave;

	// ���_��HP���擾����
	m_baseHP = (int)DataManager::GetInstance()->GetNowBaseHP();

	// �ʒu�����肷��
	SimpleMath::Vector2 missionPos = SimpleMath::Vector2(MISSION_RENDERPOS.x - Easing::EaseInCubic(0, MISSION_CLOSEBUTTON.x, m_closeAnimation), MISSION_RENDERPOS.y);

	m_missionRender->SetPos(missionPos);
	m_closeButton->SetSavePos(missionPos + (MISSION_CLOSEBUTTON * 1.2f));
	m_closeButton->HitMouse();

	if (m_closeButton->SelectionMouse())
	{
		m_closeAnimation -= pDeltaT->GetNomalDeltaTime();
		m_closeButton->SetDirection(4);
	}
	else
	{
		m_closeButton->SetDirection(2);
		m_closeAnimation += pDeltaT->GetNomalDeltaTime();
	}


	// None�ȊO�Ȃ�Βʂ� �}�V�����ݒu���ꂽ�ۂ̏���
	if (pAlchemicalManager->SpawnMachineNotification() != MACHINE_TYPE::NONE) 							MachineMission(pAlchemicalManager);

	// �B�����ɒʂ�
	if (pAlchemicalManager->GetMachineSelect()->get()->GetManufacturingFlag() != MACHINE_TYPE::NONE)	AlchemiMission(pAlchemicalManager);

	// �}�V������̂��ꂽ�Ƃ��ɒʂ�
	if (pAlchemicalManager->DestroyMachineNotification() != MACHINE_TYPE::NONE)							DestroyMission(pAlchemicalManager);

	// �}�V�����C�U���ꂽ�Ƃ��ɒʂ�
	if (pAlchemicalManager->RepairBoxMachineNotification() != MACHINE_TYPE::NONE)						RecoveryMission(pAlchemicalManager);

	// �}�V����LvUp���ꂽ�Ƃ��ɒʂ�
	if (pAlchemicalManager->LvUpMachineNotification() != MACHINE_TYPE::NONE)							LvUPMission(pAlchemicalManager);

	// Enemy���|���ꂽ�ۂɒʂ�
	if (pEnemyManager->GetKnokDownEnemyType() != ENEMY_TYPE::ENMEY_NONE) 								EnemyMission(pEnemyManager);

	// ���_Lv�̏���
	if (m_baseLvCondition.size() > 0)																	BaseLvMission(pFieldManager->GetPlayerBase()->GetBaseLv());

	// ���Ԑ����̏���
	if (m_timeCondition.size() > 0)																		TimerMission();

	// ���\�[�X�ɕω����������ۂɒʂ�
	if (pAlchemicalManager->GetPulsMpVal() > 0.0f)		ResourceMission(pAlchemicalManager);

	// �N���X�^�����\�[�X�ɕω����������ۂɒʂ�
	if (pAlchemicalManager->GetPulsCrystalVal() > 0.0f)	ResourceMission(pAlchemicalManager);

	// ���_��HP��0�ɂȂ������Ƃ�m�点��t���O
	m_failureFlag = m_baseHP <= 0;

	// �N���A�A�܂��͎��s���ɃA�j���[�V��������
	if ((m_missionNum <= m_missionSituation) || m_failureFlag)
	{
		// �Ō��Wave�����s�Ȃ�΃V�[���J�ڊJ�n�Ƃ���
		if (m_lastWave || m_failureFlag)
		{
			m_backVeil->Update();
			m_clearAnimation += 0.02f;
		}
		else
		{
			m_nextWaveFlag = true;
		}
	}

	// WAVE�N���A���ɗ�������(�Ō��Wave�̎��͗����Ȃ�)
	if ( m_nextWaveFlag && !m_lastWave )
	{

		m_waveAnimation += pDeltaT->GetDeltaTime() * 0.5f;

		if (m_waveAnimation.MaxCheck())
		{
			// wave�����Z����
			m_wave++;
			// ����Wave��ǂݍ���
			ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum(), m_wave);
		}

	}
	else
	{
		m_waveAnimation -= pDeltaT->GetDeltaTime() * 0.8f;
	}

	m_nextWaveTexture->SetRenderRatio(Easing::EaseInOutQuad(0.0f,1.0f,m_waveAnimation));

}

void MissionManager::TimerUpdate()
{
	auto pDeltaT = &DeltaTime::GetInstance();
	m_timer += pDeltaT->GetDeltaTime();
	m_timeRender->Update(m_timer);

}

void MissionManager::Render()
{

	ShareData& pSD = ShareData::GetInstance();

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �~�b�V�������e�̕`��
	m_missionRender->Render_MachineMission	(m_machineCondition);
	m_missionRender->Render_AlchemiMission	(m_alchemiCondition);
	m_missionRender->Render_DestroyMission	(m_destroyCondition);
	m_missionRender->Render_RepairMission	(m_recoveryCondition);
	m_missionRender->Render_LvUpMission		(m_lvUpCondition);
	m_missionRender->Render_EnemyMission	(m_enemyCondition);
	m_missionRender->Render_BaseLvMission	(m_baseLvCondition);
	m_missionRender->Render_TimerMission	(m_timeCondition);
	m_missionRender->Render_ResourceMission (m_resourceCondition);
	m_missionRender->LineReset();

	m_closeButton->Draw();

	// �o�ߎ��Ԃ̕`��
	m_timeRender->TimerDraw();

	if ((m_missionNum <= m_missionSituation) || m_failureFlag)
	{
		m_backVeil->Render();

		SpriteLoder& pSL = SpriteLoder::GetInstance();

		int filed = (int)m_failureFlag;

		RECT rect = SpriteCutter(320, 48, filed, 0);

		pSD.GetSpriteBatch()->Draw(pSL.GetStageClearTextTexture().Get(),
			SimpleMath::Vector2(1280.0f / 2.0f, 720.0f / 2.0f - (m_clearAnimation * 2.8f)), &rect,
			SimpleMath::Color(1.0f, 1.0f, 1.0f, m_clearAnimation),
			0.0f, SimpleMath::Vector2(320.0f / 2.0f, 48.0f / 2.0f), 2.0f);

	}

	pSD.GetSpriteBatch()->End();

	m_nextWaveTexture->Render();

}

void MissionManager::ReloadWave()
{

	auto pSJD = &ShareJsonData::GetInstance();

	// �擾���������R�s�[����
	m_machineCondition	= pSJD->GetStageData().condition_Machine;
	m_alchemiCondition	= pSJD->GetStageData().condition_Alchemi;
	m_destroyCondition	= pSJD->GetStageData().condition_Destroy;
	m_recoveryCondition = pSJD->GetStageData().condition_Recovery;
	m_lvUpCondition		= pSJD->GetStageData().condition_LvUP;
	m_enemyCondition	= pSJD->GetStageData().condition_Enemy;
	m_baseLvCondition	= pSJD->GetStageData().condition_BaseLv;
	m_timeCondition		= pSJD->GetStageData().condition_Time;
	m_resourceCondition = pSJD->GetStageData().condition_Resource;

	// ���ꂼ��̓��e�̍��v�l�𓾂�
	m_missionNum = (int)m_machineCondition.size() +
		(int)m_alchemiCondition.size()	+
		(int)m_enemyCondition.size()	+
		(int)m_baseLvCondition.size()	+
		(int)m_timeCondition.size()		+
		(int)m_destroyCondition.size()	+
		(int)m_recoveryCondition.size() +
		(int)m_lvUpCondition.size()		+
		(int)m_resourceCondition.size();


	// �~�b�V�����̒B���x
	m_missionSituation = 0;

	// �N���A�o���Ȃ����̂̓~�b�V���������������Ƃɂ���
	if (m_alchemiCondition	[0].value <= 0)		m_missionSituation++;
	if (m_baseLvCondition	[0].value <= 0)		m_missionSituation++;
	if (m_machineCondition	[0].value <= 0)		m_missionSituation++;
	if (m_enemyCondition	[0].value <= 0)		m_missionSituation++;
	if (m_timeCondition		[0].value <= 0)		m_missionSituation++;
	if (m_destroyCondition	[0].value <= 0)		m_missionSituation++;
	if (m_recoveryCondition	[0].value <= 0)		m_missionSituation++;
	if (m_lvUpCondition		[0].value <= 0)		m_missionSituation++;
	if (m_resourceCondition	[0].value <= 0)		m_missionSituation++;


	// �~�b�V������S�ăN���A�����t���O�����ɖ߂�
	m_allClearFlag = false;

	// 
	m_nextWaveFlag = false;
}

bool MissionManager::MissionComplete()
{
	return (m_missionNum <= m_missionSituation) && m_clearAnimation.MaxCheck();
}

bool MissionManager::MissionmFailure()
{
	return m_failureFlag && m_clearAnimation.MaxCheck();
}

int MissionManager::GetStartTimer()
{
	return m_timeCondition[0].progress;
}

void MissionManager::MachineMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�ݒu�֘A
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

void MissionManager::AlchemiMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���:�B���֘A
	for (int i = 0; i < m_alchemiCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_alchemiCondition[i].condition)
			== alchemicalManager->GetMachineSelect()->get()->GetSelectMachineType()
			&& m_alchemiCondition[i].progress < m_alchemiCondition[i].value)
		{

			m_alchemiCondition[i].progress++;

			// �U������
			if (m_alchemiCondition[i].progress >= m_alchemiCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}


}

void MissionManager::DestroyMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���F�j�����
	for (int i = 0; i < m_destroyCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_destroyCondition[i].condition) ==
			alchemicalManager->DestroyMachineNotification() &&
			m_destroyCondition[i].progress < m_destroyCondition[i].value)
		{
			m_destroyCondition[i].progress++;

			// �U������
			if (m_destroyCondition[i].progress >= m_destroyCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}

}

void MissionManager::RecoveryMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�C�U����
	for (int i = 0; i < m_recoveryCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_recoveryCondition[i].condition) ==
			alchemicalManager->RepairBoxMachineNotification() &&
			m_recoveryCondition[i].progress < m_recoveryCondition[i].value)
		{
			m_recoveryCondition[i].progress++;

			// �U������
			if (m_recoveryCondition[i].progress >= m_recoveryCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::LvUPMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�C�U����
	for (int i = 0; i < m_lvUpCondition.size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_lvUpCondition[i].condition) ==
			alchemicalManager->LvUpMachineNotification() &&
			m_lvUpCondition[i].progress < m_lvUpCondition[i].value)
		{
			m_lvUpCondition[i].progress++;

			// �U������
			if (m_lvUpCondition[i].progress >= m_lvUpCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::ResourceMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���F���\�[�X����
	for (int i = 0; i < m_resourceCondition.size(); i++)
	{
		if (m_resourceCondition[i].progress >= m_resourceCondition[i].value) continue;

		if (m_resourceCondition[i].condition == "MP")		m_resourceCondition[i].progress += alchemicalManager->GetPulsMpVal();

		if (m_resourceCondition[i].condition == "Crystal")	m_resourceCondition[i].progress += alchemicalManager->GetPulsCrystalVal();

		if (m_resourceCondition[i].progress >= m_resourceCondition[i].value)
		{
			m_missionSituation++;
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

		// �S�Ή��̏ꍇ
		if (m_enemyCondition[i].condition == "All")
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

void MissionManager::BaseLvMission(int baseLv)
{
	// ���_Lv���ڕW�l�ɒB����܂ōX�V
	if (m_baseLvCondition[0].progress < m_baseLvCondition[0].value)
	{
		m_baseLvCondition[0].progress = baseLv;

		if (m_baseLvCondition[0].progress >= m_baseLvCondition[0].value)
		{
			m_missionSituation++;
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
