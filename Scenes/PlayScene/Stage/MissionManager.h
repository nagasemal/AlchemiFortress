#pragma once

#include <vector>

class AlchemicalMachineManager;
class EnemyManager;
class Stage_Condition;
class Number;
class MissionRender;

class MissionManager
{
public:
	MissionManager();
	~MissionManager();

	void Initialize();
	void Update(AlchemicalMachineManager* alchemicalManager,EnemyManager* enemyManager);

	std::unique_ptr<MissionRender>* GetMissionRender() { return &m_missionRender;}

	void Render();

	bool MissionComplete();
	bool MissionmFailure();

	int GetStartTimer();

private:

	void MachineMission(AlchemicalMachineManager* alchemicalManager);
	void EnemyMission(EnemyManager* enemyManager);
	void TimerMission();

private:

	// �}�V���֘A�̏���
	std::vector<Stage_Condition> m_machineCondition;

	// �G�l�~�[�֘A�̏���
	std::vector<Stage_Condition> m_enemyCondition;

	// ���Ԋ֘A�̏���
	std::vector<Stage_Condition> m_timeCondition;

	// �^�C�}�[�v��
	std::unique_ptr<Number> m_timeRender;

	// �~�b�V�����`��
	std::unique_ptr<MissionRender> m_missionRender;

	float m_timer;

	// �S�Ă̏����𖞂��������Ƃ������t���O
	bool m_allClearFlag;

	// ���������Ƃ��ł��Ȃ��Ȃ������Ƃ������t���O(���s�t���O)
	bool m_failureFlag;

	// �S�ă~�b�V�����̍��v��
	int m_missionNum;

	// ���݂̃~�b�V�����̐i�s�x
	int m_missionSituation;

};