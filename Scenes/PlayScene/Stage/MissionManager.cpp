#include "pch.h"
#include "MissionManager.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/SelectScene/MissionRender.h"

#include "Scenes/TitleScene/TitleLogo/Veil.h"

#include "Scenes/Commons/DrawTimer.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

MissionManager::MissionManager():
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
	m_nextWaveFlag()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::Initialize()
{

	// デバイスと画面サイズの確保
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width	= device->GetOutputSize().right;
	int height	= device->GetOutputSize().bottom;

	// ステージ情報の再度読み込み
	ReloadWave();

	m_timeRender = std::make_unique<DrawTimer>(SimpleMath::Vector2{ 140.0f,160.0f }, SimpleMath::Vector2{ 1.0f,1.0f });
	m_missionRender = std::make_unique<MissionRender>(SimpleMath::Vector2{ 100.0f,260.0f }, SimpleMath::Vector2{ 1.0f,1.0f });

	// クリア時演出の生成
	m_backVeil = std::make_unique<Veil>(3);
	m_backVeil->Create(L"Resources/Textures/Fade.png");
	m_backVeil->LoadShaderFile(L"Veil");
	m_backVeil->SetWindowSize(width, height);
	m_backVeil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_backVeil->SetScale(SimpleMath::Vector2((float)width, (float)height / 5.0f));
	m_backVeil->SetPosition(SimpleMath::Vector2(0.0f, height / 2.5f));

	// Waveクリア時選出の生成
	m_nextWaveTexture = std::make_unique<UserInterface>();
	m_nextWaveTexture->Create(device,
							  L"Resources/Textures/NextWave.png",
							  SimpleMath::Vector2(width / 1.3f,height / 1.1f),
							  SimpleMath::Vector2(0.5f,0.5f),
							  UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWaveTexture->SetWindowSize(width,height);
	m_nextWaveTexture->SetColor(SimpleMath::Color(0.0f, 0.6f, 1.0f, 1.0f));


	// ステージ失敗成功時のアニメーション用変数
	m_clearAnimation.max = 2.0f;

}

void MissionManager::Update(AlchemicalMachineManager* pAlchemicalManager, EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager)
{
	auto pDeltaT = &DeltaTime::GetInstance();
	m_lastWave = ShareJsonData::GetInstance().GetStageData().lastWave;

	//m_nextWaveFlag = false;

	m_baseHP = (int)DataManager::GetInstance()->GetNowBaseHP();

	// None以外ならば通す マシンが設置された際の処理
	if (pAlchemicalManager->SpawnMachineNotification() != MACHINE_TYPE::NONE) 							MachineMission(pAlchemicalManager);

	// 錬金時に通す
	if (pAlchemicalManager->GetMachineSelect()->get()->GetManufacturingFlag() != MACHINE_TYPE::NONE)	AlchemiMission(pAlchemicalManager);
	// Enemyが倒された際に通す
	if (pEnemyManager->GetKnokDownEnemyType() != ENEMY_TYPE::ENMEY_NONE) 								EnemyMission(pEnemyManager);

	// 拠点Lvの処理
	if (m_baseLvCondition.size() > 0)																	BaseLvMission(pFieldManager->GetPlayerBase()->GetBaseLv());

	// 時間制限の処理
	if (m_timeCondition.size() > 0)																		TimerMission();

	// 拠点のHPが0になったことを知らせるフラグ
	m_failureFlag = m_baseHP <= 0;

	// クリア、または失敗時にアニメーションを回す
	if ((m_missionNum <= m_missionSituation) || m_failureFlag)
	{
		// 最後のWaveか失敗ならばシーン遷移開始とする
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

	// WAVEクリア時に流す処理(最後のWaveの時は流さない)
	if ( m_nextWaveFlag && !m_lastWave )
	{

		m_waveAnimation += pDeltaT->GetDeltaTime() * 0.5f;

		if (m_waveAnimation.MaxCheck())
		{
			// waveを加算する
			m_wave++;
			// 次のWaveを読み込む
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

	// ミッション内容の描画
	m_missionRender->Render_MachineMission	(m_machineCondition);
	m_missionRender->Render_AlchemiMission	(m_alchemiCondition);
	m_missionRender->Render_EnemyMission	(m_enemyCondition);
	m_missionRender->Render_BaseLvMission	(m_baseLvCondition);
	m_missionRender->Render_TimerMission	(m_timeCondition);
	m_missionRender->LineReset();

	// 経過時間の描画
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

	// 取得した情報をコピーする
	m_machineCondition = pSJD->GetStageData().condition_Machine;
	m_alchemiCondition = pSJD->GetStageData().condition_Alchemi;
	m_enemyCondition = pSJD->GetStageData().condition_Enemy;
	m_baseLvCondition = pSJD->GetStageData().condition_BaseLv;
	m_timeCondition = pSJD->GetStageData().condition_Time;

	// それぞれの内容の合計値を得る
	m_missionNum = (int)m_machineCondition.size() +
		(int)m_alchemiCondition.size() +
		(int)m_enemyCondition.size() +
		(int)m_baseLvCondition.size() +
		(int)m_timeCondition.size();


	// ミッションの達成度
	m_missionSituation = 0;

	// クリア出来ないものはミッション完了したことにする
	if (m_alchemiCondition[0].value <= 0)	m_missionSituation++;
	if (m_baseLvCondition[0].value <= 0)	m_missionSituation++;
	if (m_machineCondition[0].value <= 0)	m_missionSituation++;
	if (m_enemyCondition[0].value <= 0)		m_missionSituation++;
	if (m_timeCondition[0].value <= 0)		m_missionSituation++;

	// ミッションを全てクリアしたフラグを元に戻す
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
	// 対応する条件をTrueにする：設置関連
	for (int i = 0; i < m_machineCondition.size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (Json::ChangeMachine(m_machineCondition[i].condition) ==
			alchemicalManager->SpawnMachineNotification() &&
			m_machineCondition[i].progress < m_machineCondition[i].value)
		{
			m_machineCondition[i].progress++;

			// 攻略完了
			if (m_machineCondition[i].progress >= m_machineCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::AlchemiMission(AlchemicalMachineManager* alchemicalManager)
{

	// 対応する条件をTrueにする:錬金関連
	for (int i = 0; i < m_alchemiCondition.size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (Json::ChangeMachine(m_alchemiCondition[i].condition)
			== alchemicalManager->GetMachineSelect()->get()->GetSelectMachineType()
			&& m_alchemiCondition[i].progress < m_alchemiCondition[i].value)
		{

			m_alchemiCondition[i].progress++;

			// 攻略完了
			if (m_alchemiCondition[i].progress >= m_alchemiCondition[i].value)
			{
				m_missionSituation++;
			}
		}
	}


}

void MissionManager::EnemyMission(EnemyManager* enemyManager)
{

	// 対応する条件をTrueにする
	for (int i = 0; i < m_enemyCondition.size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (Json::ChangeEnemy(m_enemyCondition[i].condition) ==
			enemyManager->GetKnokDownEnemyType() &&
			m_enemyCondition[i].progress < m_enemyCondition[i].value)
		{
			// 同一フレーム内に複数対敵がやられたとしても対応可能にする
			m_enemyCondition[i].progress += enemyManager->GetKnokDownEnemyFlag();

			// 攻略完了
			if (m_enemyCondition[i].progress >= m_enemyCondition[i].value)
			{
				m_missionSituation++;
			}
		}

		// 全対応の場合
		if (m_enemyCondition[i].condition == "All")
		{
			// 同一フレーム内に複数対敵がやられたとしても対応可能にする
			m_enemyCondition[i].progress += enemyManager->GetKnokDownEnemyFlag();

			// 攻略完了
			if (m_enemyCondition[i].progress >= m_enemyCondition[i].value)
			{
				m_missionSituation++;
			}
		}

	}

}

void MissionManager::BaseLvMission(int baseLv)
{
	// 拠点Lvが目標値に達するまで更新
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

	// 生き残れば勝利系タイマー
	if (m_timeCondition[0].progress < m_timeCondition[0].value)
	{
		// 毎秒進行度を上げる
		m_timeCondition[0].progress = (int)m_timer;

		if (m_timeCondition[0].progress >= m_timeCondition[0].value)
		{

			if (m_timeCondition[0].condition == "Standerd") m_missionSituation++;
			if (m_timeCondition[0].condition == "Limit") m_failureFlag = true;
			
		}
	}
}
