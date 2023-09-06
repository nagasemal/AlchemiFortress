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
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/PlayScene/Tutorial/Tutorial.h"

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

	std::unique_ptr<FieldObjectManager>			m_fieldManager;

	std::unique_ptr<MousePointer>				m_mousePointer;

	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	std::unique_ptr<MoveCamera>					m_moveCamera;

	std::unique_ptr<EnemyManager>				m_enemyManager;

	std::unique_ptr<MissionManager>				m_missionManager;

	// �e�X�g�p
	std::unique_ptr<Gauge>						m_gauge;

	std::unique_ptr<DirectX::Model>				m_skySphere;

	std::unique_ptr<Tutorial>					m_tutorial;

	// �X�e�[�W�ԍ�
	int m_stageNumber;

};