#pragma once

#include "NecromaLib/GameData/Animation.h"
#include "NecromaLib/GameData/CommonStruct.h"

// �O���錾
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

	// �X�e�[�W�����ēx�ǂݍ���
	void ReloadWave();

	bool MissionComplete();
	bool MissionmFailure();

	// ����WAVE�ɐi�񂾂��Ƃ�m�点��t���O
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

	// �~�b�V��������
	std::vector<Stage_Condition> m_missonCondition[MISSION_TYPE::MISSION_NUM];

	//// �}�V���B���̏���
	//std::vector<Stage_Condition> m_alchemiCondition;
	//
	//// �}�V���j��̏���
	//std::vector<Stage_Condition> m_destroyCondition;
	//
	//// �}�V���C�U�̏���
	//std::vector<Stage_Condition> m_recoveryCondition;
	//
	//// �}�V�������̏���
	//std::vector<Stage_Condition> m_lvUpCondition;
	//
	//// �擾���\�[�X�̏���
	//std::vector<Stage_Condition> m_resourceCondition;
	//
	//// �G�l�~�[�֘A�̏���
	//std::vector<Stage_Condition> m_enemyCondition;
	//
	//// ���_LV�֘A�̏���
	//std::vector<Stage_Condition> m_baseLvCondition;
	// 
	//// ���Ԋ֘A�̏���
	//std::vector<Stage_Condition> m_timeCondition;

	// �^�C�}�[�v��
	std::unique_ptr<DrawTimer> m_timeRender;

	// �~�b�V�����`��
	std::unique_ptr<MissionRender> m_missionRender;

	std::unique_ptr<DrawArrow> m_closeButton;

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

	// �~�b�V�����̕\�������
	bool m_closeMission;
	// ��ލۂ̃A�j���[�V�����p�ϐ�
	AnimationData m_closeAnimation;

};