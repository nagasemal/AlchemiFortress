#pragma once

#include "NecromaLib/GameData/Animation.h"
#include "NecromaLib/GameData/CommonStruct.h"

// 前方宣言
class AlchemicalMachineManager;
class FieldObjectManager;
class EnemyManager;
struct Stage_Condition;
class Number;
class MissionRender;
class DrawTimer;
class Veil;
class UserInterface;
class DrawArrow;

class MissionManager
{
public:
	MissionManager();
	~MissionManager();

	void Initialize();
	void Update(AlchemicalMachineManager* pAlchemicalManager, EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager);

	void TimerUpdate();

	std::unique_ptr<MissionRender>* GetMissionRender() { return &m_missionRender; }

	void Render();

	// ステージ情報を再度読み込む
	void ReloadWave();

	bool MissionComplete();
	bool MissionmFailure();

	// 次のWAVEに進んだことを知らせるフラグ
	bool NextWaveFlag() { return m_nextWaveFlag && m_waveAnimation.MaxCheck(); }

	int GetStartTimer();

	int GetWave() { return m_wave; }

private:

	void MachineMission(AlchemicalMachineManager* alchemicalManager);
	void AlchemiMission(AlchemicalMachineManager* alchemicalManager);
	void DestroyMission(AlchemicalMachineManager* alchemicalManager);
	void RecoveryMission(AlchemicalMachineManager* alchemicalManager);
	void LvUPMission(AlchemicalMachineManager* alchemicalManager);

	void ResourceMission(AlchemicalMachineManager* alchemicalManager);

	void EnemyMission(EnemyManager* enemyManager);
	void BaseLvMission(int baseLv);
	void TimerMission();

private:

	// ミッション条件
	std::vector<Stage_Condition> m_missonCondition[MISSION_TYPE::MISSION_NUM];

	//// マシン錬金の条件
	//std::vector<Stage_Condition> m_alchemiCondition;
	//
	//// マシン破壊の条件
	//std::vector<Stage_Condition> m_destroyCondition;
	//
	//// マシン修繕の条件
	//std::vector<Stage_Condition> m_recoveryCondition;
	//
	//// マシン強化の条件
	//std::vector<Stage_Condition> m_lvUpCondition;
	//
	//// 取得リソースの条件
	//std::vector<Stage_Condition> m_resourceCondition;
	//
	//// エネミー関連の条件
	//std::vector<Stage_Condition> m_enemyCondition;
	//
	//// 拠点LV関連の条件
	//std::vector<Stage_Condition> m_baseLvCondition;
	// 
	//// 時間関連の条件
	//std::vector<Stage_Condition> m_timeCondition;

	// タイマー計測
	std::unique_ptr<DrawTimer> m_timeRender;

	// ミッション描画
	std::unique_ptr<MissionRender> m_missionRender;

	std::unique_ptr<DrawArrow> m_closeButton;

	// 半透明幕(クリア時)の描画
	std::unique_ptr<Veil> m_backVeil;

	// NextWaveの文字描画
	std::unique_ptr<UserInterface> m_nextWaveTexture;

	float m_timer;

	// ステージクリアした際に流すアニメーション用変数
	AnimationData m_clearAnimation;
	// WAVEクリアした際に流すアニメーション用変数
	AnimationData m_waveAnimation;


	// 全ての条件を満たしたことを示すフラグ
	bool m_allClearFlag;

	// 満たすことができなくなったことを示すフラグ(失敗フラグ)
	bool m_failureFlag;

	// 全てミッションの合計数
	int m_missionNum;

	// 現在のミッションの進行度
	int m_missionSituation;

	// 現在の拠点のHP
	int m_baseHP;

	// 現在が最後のステージであるか
	bool m_lastWave;

	//　現在のWave数の取得
	int m_wave;

	// 次のWaveに進んだことを知らせるフラグ
	bool m_nextWaveFlag;

	// ミッションの表示を畳む
	bool m_closeMission;
	// 畳む際のアニメーション用変数
	AnimationData m_closeAnimation;

};