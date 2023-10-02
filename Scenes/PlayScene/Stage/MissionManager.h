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

	// �}�V���֘A�̏���
	std::vector<Stage_Condition> m_machineCondition;

	// �B���֘A�̏���
	std::vector<Stage_Condition> m_alchemiCondition;

	// �G�l�~�[�֘A�̏���
	std::vector<Stage_Condition> m_enemyCondition;

	// �B���֘A�̏���
	std::vector<Stage_Condition> m_baseLvCondition;

	// ���Ԋ֘A�̏���
	std::vector<Stage_Condition> m_timeCondition;

	// �^�C�}�[�v��
	std::unique_ptr<DrawTimer> m_timeRender;

	// �~�b�V�����`��
	std::unique_ptr<MissionRender> m_missionRender;

	float m_timer;

	AnimationData m_clearAnimation;

	// �S�Ă̏����𖞂��������Ƃ������t���O
	bool m_allClearFlag;

	// ���������Ƃ��ł��Ȃ��Ȃ������Ƃ������t���O(���s�t���O)
	bool m_failureFlag;

	// �S�ă~�b�V�����̍��v��
	int m_missionNum;

	// ���݂̃~�b�V�����̐i�s�x
	int m_missionSituation;

	// ���݂̋��_��HP
	int m_baseHP;

};