#pragma once

#include <vector>

class AlchemicalMachineManager;
class EnemyManager;
class Stage_Condition;
class Number;

class MissionManager
{
public:
	MissionManager();
	~MissionManager();

	void Initialize();
	void Update(AlchemicalMachineManager* alchemicalManager,EnemyManager* enemyManager);

	void Render();

	bool MissionComplete();
	bool MissionmFailure();

private:

	void MachineMission(AlchemicalMachineManager* alchemicalManager);
	void EnemyMission(EnemyManager* enemyManager);
	void TimerMission();

private:

	// マシン関連の条件
	std::vector<Stage_Condition> m_machineCondition;

	// エネミー関連の条件
	std::vector<Stage_Condition> m_enemyCondition;

	// 時間関連の条件
	std::vector<Stage_Condition> m_timeCondition;

	// タイマー計測
	std::unique_ptr<Number> m_timeRender;

	float m_timer;

	// 全ての条件を満たしたことを示すフラグ
	bool m_allClearFlag;

	// 満たすことができなくなったことを示すフラグ(失敗フラグ)
	bool m_failureFlag;

	// 全てミッションの合計数
	int m_missionNum;

	// 現在のミッションの進行度
	int m_missionSituation;

};