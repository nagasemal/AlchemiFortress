#pragma once

#include "NecromaLib/GameData/Animation.h"

// �O���錾
class AlchemicalMachineManager;
class FieldObjectManager;
class EnemyManager;
class Stage_Condition;
class Number;
class MissionRender;
class DrawTimer;
class Veil;
class UserInterface;

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

	// �X�e�[�W�����ēx�ǂݍ���
	void ReloadWave();

	bool MissionComplete();
	bool MissionmFailure();

	// ����WAVE�ɐi�񂾂��Ƃ�m�点��t���O
	bool NextWaveFlag() { return m_nextWaveFlag && m_waveAnimation.MaxCheck(); }

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

	// ��������(�N���A��)�̕`��
	std::unique_ptr<Veil> m_backVeil;

	// NextWave�̕����`��
	std::unique_ptr<UserInterface> m_nextWaveTexture;

	float m_timer;

	// �X�e�[�W�N���A�����ۂɗ����A�j���[�V�����p�ϐ�
	AnimationData m_clearAnimation;
	// WAVE�N���A�����ۂɗ����A�j���[�V�����p�ϐ�
	AnimationData m_waveAnimation;


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

	// ���݂��Ō�̃X�e�[�W�ł��邩
	bool m_lastWave;

	//�@���݂�Wave���̎擾
	int m_wave;

	// ����Wave�ɐi�񂾂��Ƃ�m�点��t���O
	bool m_nextWaveFlag;

};