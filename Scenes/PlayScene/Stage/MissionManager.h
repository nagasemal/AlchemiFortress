#pragma once

#include "NecromaLib/GameData/Animation.h"

class AlchemicalMachineManager;
class FieldObjectManager;
class EnemyManager;
class Stage_Condition;
class Number;
class MissionRender;
class DrawTimer;

class MissionManager
{
public:
	MissionManager();
	~MissionManager();

	void Initialize();
	void Update(AlchemicalMachineManager* pAlchemicalManager,EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager);

	std::unique_ptr<MissionRender>* GetMissionRender() { return &m_missionRender;}

	void Render();

	bool MissionComplete();
	bool MissionmFailure();

	int GetStartTimer();

private:

	void MachineMission(AlchemicalMachineManager* alchemicalManager);
	void AlchemiMission(AlchemicalMachineManager* alchemicalManager);
	void EnemyMission(EnemyManager* enemyManager);
	void BaseLvMission(int baseLv);
	void TimerMission();

private:

	// マシン関連の条件
	std::vector<Stage_Condition> m_machineCondition;

	// 錬金関連の条件
	std::vector<Stage_Condition> m_alchemiCondition;

	// エネミー関連の条件
	std::vector<Stage_Condition> m_enemyCondition;

	// 錬金関連の条件
	std::vector<Stage_Condition> m_baseLvCondition;

	// 時間関連の条件
	std::vector<Stage_Condition> m_timeCondition;

	// タイマー計測
	std::unique_ptr<DrawTimer> m_timeRender;

	// ミッション描画
	std::unique_ptr<MissionRender> m_missionRender;

	float m_timer;

	AnimationData m_clearAnimation;

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

};