#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"
#include "Camera/MoveCamera.h"

// �Q�[���̃I�u�W�F�N�g
#include "Field/FieldObjectManager.h"
#include "Mouse/MousePointer.h"
#include "AlchemicalMachine/AlchemicalMachineManager.h"
#include "Enemy/EnemyManager.h"

// �~�b�V����
#include "Scenes/PlayScene/Stage/MissionManager.h"

// UI�֘A
#include "Scenes/PlayScene/UI/BaseLv.h"
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/PlayScene/Tutorial/Tutorial.h"
#include "Scenes/PlayScene/Tutorial/Explanation.h"

class PlayScene : public SceneObj
{
public:
	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update() override;

	// �`��
	void Draw() override;

	// UI�n�`��
	void DrawUI() override;

	// �I������
	void Finalize() override;

	void EnemyToAMMachine();
	void EnemyToPlayerBase();
	void EnemyToBullet();

private:

	// �t�B�[���h�ɑ��݂���I�u�W�F�N�g���Ǘ�
	std::unique_ptr<FieldObjectManager>			m_fieldManager;

	// �}�E�X�|�C���^�[�ɂ����鏈��
	std::unique_ptr<MousePointer>				m_mousePointer;

	// �}�V�����j�b�g/�}�V���Ɋ֘A����UI�̊Ǘ�
	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	// �J�����ɑ΂��鏈��
	std::unique_ptr<MoveCamera>					m_moveCamera;

	// �G�l�~�[���Ǘ�
	std::unique_ptr<EnemyManager>				m_enemyManager;

	// �~�b�V�������Ǘ�
	std::unique_ptr<MissionManager>				m_missionManager;

	// HP,MP,�N���X�^���̃Q�[�W
	std::unique_ptr<Gauge>						m_resourceGauge;

	std::unique_ptr<BaseLv>						m_baseLv;

	// �`���[�g���A���N���X
	std::unique_ptr<Tutorial>					m_tutorial;

	std::unique_ptr<Explanation>				m_explanation;

	std::unique_ptr<DirectX::Model>				m_skySphere;

	// �{���{�^��
	std::unique_ptr<SelectionBox>				m_doubleSpeedButton; 
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// �{���{��
	int m_doubleSpeedNum;

};