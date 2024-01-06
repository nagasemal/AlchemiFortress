#include "pch.h"
#include "MissionManager.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/Mission/ClearVeil.h"

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

MissionManager::MissionManager() :
	m_allClearFlag(),
	m_missionNum(),
	m_missionSituation(),
	m_gameTimer(),
	m_uiTransparentTime(),
	m_failureFlag(),
	m_baseHP(),
	m_lastWave(),
	m_wave(1),
	m_nextWaveFlag(),
	m_closeAnimation(),
	m_closeMission(false),
	m_closeButton(),
	m_waveTimer()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::Initialize()
{

	//�@====================[�@�f�o�C�X�Ɖ�ʃT�C�Y�̊m�ہ@]
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width	= device->GetOutputSize().right;
	int height	= device->GetOutputSize().bottom;

	UI_Data ui_data = ShareJsonData::GetInstance().GetUIData("MissionOffset");

	//�@====================[�@�}�E�X�����ӂɈʒu���邩���擾�@]
	m_collider = std::make_unique<SelectionBox>(ui_data.pos, SimpleMath::Vector2(ui_data.option["COLLIDER_X"], ui_data.option["COLLIDER_Y"]));
	m_collider->Initialize();
	m_collider->SetRect(RECT{ 0,0,1,1 });

	//�@====================[�@�X�e�[�W���̍ēx�ǂݍ��݁@]
	ReloadWave();

	//�@====================[�@���ԕ`��N���X�̐ݒ�@]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionTimer");
	m_timeRender = std::make_unique<DrawTimer>(ui_data.pos, ui_data.rage);

	//�@====================[�@�~�b�V�����N���X�̐ݒ�@]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionText");
	m_missionRender = std::make_unique<MissionRender>(ui_data.pos, ui_data.rage);
	//�@�@|=>�@�ʒu�̍ēo�^
	m_collider->SetSavePos(ui_data.pos);

	//�@====================[�@�~�b�V�����̊J���s�����N���X�̐ݒ�@]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionArrow");
	m_closeButton = std::make_unique<DrawArrow>(ui_data.pos, ui_data.rage, (int)ui_data.option["Open"]);
	m_closeButton->Initialize();

	//�@====================[�@�X�e�[�W�N���A�����o�N���X�̐ݒ�@]
	m_backVeil = std::make_unique<Veil>(3);
	m_backVeil->Create(L"Resources/Textures/Fade.png");
	m_backVeil->LoadShaderFile(L"Veil");
	m_backVeil->SetWindowSize(width, height);
	m_backVeil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_backVeil->SetScale(SimpleMath::Vector2((float)width, (float)height / 5.0f));
	m_backVeil->SetPosition(SimpleMath::Vector2(0.0f, height / 2.5f));

	//�@====================[�@�N���A���̉��o�@]
	m_clearVeil = std::make_unique<ClearVeil>();

	//�@====================[�@�X�e�[�W���s�������̃A�j���[�V�����p�ϐ��@]
	m_clearAnimation.max = 2.0f;
}

void MissionManager::Update(AlchemicalMachineManager* pAlchemicalManager, EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager)
{
	auto pDeltaT = &DeltaTime::GetInstance();
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//�@====================[�@UI���������@]
	m_uiTransparentTime += pDeltaT->GetNomalDeltaTime();
	//�@�@|=>�@�w��t���[����ɔ�������
	if (m_uiTransparentTime >= pSJD.GetGameParameter().transparent_time)
	{
		TransparentUI(pSJD.GetGameParameter().transparent_val);
	}
	//�@�@|=>�@UI���ӂɃ}�E�X���ڐG�����瓧���x���Z�b�g
	if (m_collider->HitMouse())
	{
		m_uiTransparentTime = 0.0f;
		TransparentUI(1.0f);
	}

	//�@====================[�@�ŏI�E�F�[�u�ł��邩�ۂ����擾�@]
	m_lastWave = pSJD.GetStageData().lastWave;

	//�@�@|=>�@���_��HP���擾����
	m_baseHP = (int)DataManager::GetInstance()->GetNowBaseHP();

	//�@====================[�@UI�̈ʒu������@]
	UI_Data ui_dataText	 = pSJD.GetUIData("MissionText");
	UI_Data ui_dataArrow = pSJD.GetUIData("MissionArrow");
	UI_Data ui_dataTimer = pSJD.GetUIData("MissionTimer");
	SimpleMath::Vector2 missionPos = SimpleMath::Vector2(ui_dataText.pos.x - Easing::EaseInCubic(0, ui_dataText.option["CloseVal"], m_closeAnimation),
														 ui_dataText.pos.y);

	//�@====================[�@UI�̈ړ������@]
	m_missionRender	->SetPos(missionPos);
	m_timeRender	->SetPos(SimpleMath::Vector2(missionPos.x + ui_dataTimer.pos.x, ui_dataTimer.pos.y));
	m_closeButton	->SetSavePos(SimpleMath::Vector2(missionPos.x + (ui_dataArrow.pos.x * 1.5f), ui_dataArrow.pos.y));

	//�@====================[�@�J���̍X�V�@]
	m_closeButton->HitMouse();

	//�@====================[�@�J���̌�����ς��鏈���@]
	if (m_closeButton->SelectionMouse())
	{
		m_closeButton->SetDirection((int)ui_dataArrow.option["Close"]);
		m_closeAnimation += pDeltaT->GetNomalDeltaTime();
	}
	else
	{
		m_closeAnimation -= pDeltaT->GetNomalDeltaTime();
		m_closeButton->SetDirection((int)ui_dataArrow.option["Open"]);
	}


	//�@====================[�@�}�V�����ݒu���ꂽ�ۂ̏����@]
	if (pAlchemicalManager->SpawnMachineNotification()		!= MACHINE_TYPE::NONE) 						MachineMission(pAlchemicalManager);

	//�@====================[�@�}�V�����B�����ꂽ�@]
	if (pAlchemicalManager->GetMachineSelect()->get()->GetManufacturingFlag() != MACHINE_TYPE::NONE)	AlchemiMission(pAlchemicalManager);

	//�@====================[�@�}�V������̂��ꂽ�@]
	if (pAlchemicalManager->DestroyMachineNotification()	!= MACHINE_TYPE::NONE)						DestroyMission(pAlchemicalManager);

	//�@====================[�@�}�V�����C�U���ꂽ�@]
	if (pAlchemicalManager->RepairBoxMachineNotification()	!= MACHINE_TYPE::NONE)						RecoveryMission(pAlchemicalManager);

	//�@====================[�@�}�V����LvUP���ꂽ�@]
	if (pAlchemicalManager->LvUpMachineNotification()		!= MACHINE_TYPE::NONE)						LvUPMission(pAlchemicalManager);

	//�@====================[�@�G�l�~�[���|���ꂽ�@]
	if (pEnemyManager->GetKnokDownEnemyType()				!= ENEMY_TYPE::ENMEY_NONE) 					EnemyMission(pEnemyManager);

	// ���_Lv�̏���
	if (m_missonCondition[MISSION_TYPE::RESOURCE].size() > 0)											BaseLvMission(pFieldManager->GetPlayerBase()->GetBaseLv());

	// ���Ԑ����̏���
	if (m_missonCondition[MISSION_TYPE::TIMER].size() > 0)												TimerMission();

	// ���\�[�X�ɕω����������ۂɒʂ�
	if (pAlchemicalManager->GetPulsMpVal() > 0.0f)		ResourceMission(pAlchemicalManager);

	// �N���X�^�����\�[�X�ɕω����������ۂɒʂ�
	if (pAlchemicalManager->GetPulsCrystalVal() > 0.0f)	ResourceMission(pAlchemicalManager);

	// �N���A�A�܂��͎��s���ɃA�j���[�V��������
	if ((m_missionNum <= m_missionSituation) || MissionUnable())
	{
		// �Ō��Wave�����s�Ȃ�΃V�[���J�ڊJ�n�Ƃ���
		if (m_lastWave || MissionUnable())
		{
			m_backVeil->Update();
			m_clearAnimation += 0.02f;
		}
		else
		{
			m_nextWaveFlag = true;
		}
	}

	//�@====================[�@WAVE�N���A���ɗ�������(�Ō��Wave�̎��͗����Ȃ�)�@] 
	m_clearVeil->Update(m_nextWaveFlag && !m_lastWave);
	//�@�@|=>�@����Wave�Ɉڍs
	if (m_clearVeil->NextWave())
	{
		// Wave�����Z����
		m_wave++;
		// ����Wave��ǂݍ���
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum(), m_wave);
	}
}

void MissionManager::TimerUpdate()
{
	auto pDeltaT = &DeltaTime::GetInstance();
	m_gameTimer += pDeltaT->GetDeltaTime();
	m_waveTimer += pDeltaT->GetDeltaTime();

	m_timeRender->Update(m_gameTimer);

}

void MissionManager::Render()
{

	ShareData& pSD = ShareData::GetInstance();

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �~�b�V�������e�̕`��
	m_missionRender->Render_MachineMission	(m_missonCondition[MISSION_TYPE::SPAWN]);
	m_missionRender->Render_AlchemiMission	(m_missonCondition[MISSION_TYPE::ALCHEMI]);
	m_missionRender->Render_DestroyMission	(m_missonCondition[MISSION_TYPE::DESTROY]);
	m_missionRender->Render_RepairMission	(m_missonCondition[MISSION_TYPE::REPAIR]);
	m_missionRender->Render_LvUpMission		(m_missonCondition[MISSION_TYPE::LVUP]);
	m_missionRender->Render_EnemyMission	(m_missonCondition[MISSION_TYPE::ENEMY_KILL]);
	m_missionRender->Render_BaseLvMission	(m_missonCondition[MISSION_TYPE::BASELV]);
	m_missionRender->Render_TimerMission	(m_missonCondition[MISSION_TYPE::TIMER]);
	m_missionRender->Render_ResourceMission (m_missonCondition[MISSION_TYPE::RESOURCE]);

	m_missionRender->LineReset();

	// �o�ߎ��Ԃ̕`��
	m_timeRender->TimerDraw();

	if ((m_missionNum <= m_missionSituation) || MissionUnable())
	{
		m_backVeil->Render();

		SpriteLoder& pSL = SpriteLoder::GetInstance();

		int filed = (int)MissionUnable();

		RECT rect = SpriteCutter(320, 48, filed, 0);

		pSD.GetSpriteBatch()->Draw(pSL.GetStageClearTextTexture().Get(),
			SimpleMath::Vector2(1280.0f / 2.0f, 720.0f / 2.0f - (m_clearAnimation * 2.8f)), &rect,
			SimpleMath::Color(1.0f, 1.0f, 1.0f, m_clearAnimation),
			0.0f, SimpleMath::Vector2(320.0f / 2.0f, 48.0f / 2.0f), 2.0f);

	}

	pSD.GetSpriteBatch()->End();

	m_closeButton		->Draw();

	m_clearVeil			->Render();
}

void MissionManager::ReloadWave()
{

	auto pSJD = &ShareJsonData::GetInstance();

	// �~�b�V�����̒B���x
	m_missionSituation = 0;

	// �E�F�[�u���̎��Ԃ����Z�b�g����
	m_waveTimer = 0;

	// �擾���������R�s�[����
	for (int i = 0; i < MISSION_TYPE::MISSION_NUM; i++)
	{
		m_missonCondition[i] = pSJD->GetStageData().condition[i];

		if (m_missonCondition[i][0].value <= 0)		m_missionSituation++;
	}

	// ���ꂼ��̓��e�̍��v�l�𓾂�
	m_missionNum = 
		(int)m_missonCondition[MISSION_TYPE::SPAWN].size()		+
		(int)m_missonCondition[MISSION_TYPE::ALCHEMI].size()	+
		(int)m_missonCondition[MISSION_TYPE::REPAIR].size()		+
		(int)m_missonCondition[MISSION_TYPE::LVUP].size()		+
		(int)m_missonCondition[MISSION_TYPE::DESTROY].size()	+
		(int)m_missonCondition[MISSION_TYPE::ENEMY_KILL].size()	+
		(int)m_missonCondition[MISSION_TYPE::RESOURCE].size()	+
		(int)m_missonCondition[MISSION_TYPE::BASELV].size()		+
		(int)m_missonCondition[MISSION_TYPE::TIMER].size();

	// �~�b�V������S�ăN���A�����t���O�����ɖ߂�
	m_allClearFlag = false;

	// �E�F�[�u���i�񂾂��߁A�v����false�ɂ���
	m_nextWaveFlag = false;
}

bool MissionManager::MissionComplete()
{
	return (m_missionNum <= m_missionSituation) && m_clearAnimation.MaxCheck();
}

bool MissionManager::MissionUnable()
{
	return m_failureFlag || m_baseHP <= 0;
}

bool MissionManager::MissionFailure()
{
	return MissionUnable() && m_clearAnimation.MaxCheck();
}

int MissionManager::GetStartTimer()
{
	return m_missonCondition[MISSION_TYPE::TIMER][0].progress;
}

void MissionManager::TransparentUI(float transparentVal)
{

	m_missionRender->SetAlpha(transparentVal);
	m_closeButton->SetColor(SimpleMath::Color(m_closeButton->GetColorRGB(), transparentVal));
	m_timeRender->SetColor(SimpleMath::Color(m_timeRender->GetColorRGB(), transparentVal));

}

void MissionManager::MachineMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�ݒu�֘A
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::SPAWN].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_missonCondition[MISSION_TYPE::SPAWN][i].condition) ==
			alchemicalManager->SpawnMachineNotification() &&
			m_missonCondition[MISSION_TYPE::SPAWN][i].progress < m_missonCondition[MISSION_TYPE::SPAWN][i].value)
		{
			m_missonCondition[MISSION_TYPE::SPAWN][i].progress++;

			// �U������
			if (m_missonCondition[MISSION_TYPE::SPAWN][i].progress >= m_missonCondition[MISSION_TYPE::SPAWN][i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::AlchemiMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���:�B���֘A
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::ALCHEMI].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_missonCondition[MISSION_TYPE::ALCHEMI][i].condition)
			== alchemicalManager->GetMachineSelect()->get()->GetSelectMachineType()
			&& m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress < m_missonCondition[MISSION_TYPE::ALCHEMI][i].value)
		{

			m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress++;

			// �U������
			if (m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress >= m_missonCondition[MISSION_TYPE::ALCHEMI][i].value)
			{
				m_missionSituation++;
			}
		}
	}

}

void MissionManager::DestroyMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���F�j�����
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::DESTROY].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_missonCondition[MISSION_TYPE::DESTROY][i].condition) ==
			alchemicalManager->DestroyMachineNotification() &&
			m_missonCondition[MISSION_TYPE::DESTROY][i].progress < m_missonCondition[MISSION_TYPE::DESTROY][i].value)
		{
			m_missonCondition[MISSION_TYPE::DESTROY][i].progress++;

			// �U������
			if (m_missonCondition[MISSION_TYPE::DESTROY][i].progress >= m_missonCondition[MISSION_TYPE::DESTROY][i].value)
			{
				m_missionSituation++;
			}
		}
	}

}

void MissionManager::RecoveryMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�C�U����
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::REPAIR].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_missonCondition[MISSION_TYPE::REPAIR][i].condition) ==
			alchemicalManager->RepairBoxMachineNotification() &&
			m_missonCondition[MISSION_TYPE::REPAIR][i].progress < m_missonCondition[MISSION_TYPE::REPAIR][i].value)
		{
			m_missonCondition[MISSION_TYPE::REPAIR][i].progress++;

			// �U������
			if (m_missonCondition[MISSION_TYPE::REPAIR][i].progress >= m_missonCondition[MISSION_TYPE::REPAIR][i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::LvUPMission(AlchemicalMachineManager* alchemicalManager)
{
	// �Ή����������True�ɂ���F�C�U����
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::LVUP].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeMachine(m_missonCondition[MISSION_TYPE::LVUP][i].condition) ==
			alchemicalManager->LvUpMachineNotification() &&
			m_missonCondition[MISSION_TYPE::LVUP][i].progress < m_missonCondition[MISSION_TYPE::LVUP][i].value)
		{
			m_missonCondition[MISSION_TYPE::LVUP][i].progress++;

			// �U������
			if (m_missonCondition[MISSION_TYPE::LVUP][i].progress >= m_missonCondition[MISSION_TYPE::LVUP][i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::ResourceMission(AlchemicalMachineManager* alchemicalManager)
{

	// �Ή����������True�ɂ���F���\�[�X����
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::RESOURCE].size(); i++)
	{
		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].progress >= m_missonCondition[MISSION_TYPE::RESOURCE][i].value) continue;

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].condition == "MP")		m_missonCondition[MISSION_TYPE::RESOURCE][i].progress += alchemicalManager->GetPulsMpVal();

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].condition == "Crystal")	m_missonCondition[MISSION_TYPE::RESOURCE][i].progress += alchemicalManager->GetPulsCrystalVal();

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].progress >= m_missonCondition[MISSION_TYPE::RESOURCE][i].value)
		{
			m_missionSituation++;
		}

	}
}

void MissionManager::EnemyMission(EnemyManager* enemyManager)
{

	// �Ή����������True�ɂ���
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::ENEMY_KILL].size(); i++)
	{
		// �~�b�V�����̓��e�Ɠ����Ȃ�Ώ�����ʂ� ���Ƀ~�b�V�������ς�ł���ꍇ�͔�΂�
		if (Json::ChangeEnemy(m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].condition) ==
			enemyManager->GetKnokDownEnemyType() &&
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress < m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
		{
			// ����t���[�����ɕ����ΓG�����ꂽ�Ƃ��Ă��Ή��\�ɂ���
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress += enemyManager->GetKnokDownEnemyFlag();

			// �U������
			if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress >= m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
			{
				m_missionSituation++;
			}
		}

		// �S�Ή��̏ꍇ
		if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].condition == "All")
		{
			// ����t���[�����ɕ����ΓG�����ꂽ�Ƃ��Ă��Ή��\�ɂ���
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress += enemyManager->GetKnokDownEnemyFlag();

			// �U������
			if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress >= m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
			{
				m_missionSituation++;
			}
		}

	}

}

void MissionManager::BaseLvMission(int baseLv)
{
	// ���_Lv���ڕW�l�ɒB����܂ōX�V
	if (m_missonCondition[MISSION_TYPE::BASELV][0].progress < m_missonCondition[MISSION_TYPE::BASELV][0].value)
	{
		m_missonCondition[MISSION_TYPE::BASELV][0].progress = baseLv;

		if (m_missonCondition[MISSION_TYPE::BASELV][0].progress >= m_missonCondition[MISSION_TYPE::BASELV][0].value)
		{
			m_missionSituation++;
		}
	}

}

void MissionManager::TimerMission()
{

	// �����c��Ώ����n�^�C�}�[
	if (m_missonCondition[MISSION_TYPE::TIMER][0].progress < m_missonCondition[MISSION_TYPE::TIMER][0].value)
	{
		// ���b�i�s�x���グ��
		m_missonCondition[MISSION_TYPE::TIMER][0].progress = (int)m_waveTimer;

		if (m_missonCondition[MISSION_TYPE::TIMER][0].progress >= m_missonCondition[MISSION_TYPE::TIMER][0].value)
		{

			if (m_missonCondition[MISSION_TYPE::TIMER][0].condition == "Standerd") m_missionSituation++;
			if (m_missonCondition[MISSION_TYPE::TIMER][0].condition == "Limit") m_failureFlag = true;
			
		}
	}
}

bool MissionManager::NextWaveFlag()
{
	return m_nextWaveFlag && m_clearVeil->NextWave();
}