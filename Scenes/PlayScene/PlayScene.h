#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"
#include "Camera/MoveCamera.h"

// �Q�[���̃I�u�W�F�N�g
#include "AlchemicalMachine/PlayerBase.h"
#include "Field/Field.h"
#include "Mouse/MousePointer.h"
#include "AlchemicalMachine/AlchemicalMachineManager.h"
#include "Enemy/EnemyManager.h"

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

private:

	std::unique_ptr<PlayerBase>					m_playerBase;

	std::unique_ptr<Field>						m_field;

	std::unique_ptr<MousePointer>				m_mousePointer;

	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	std::unique_ptr<MoveCamera>					m_moveCamera;

	std::unique_ptr<EnemyManager>				m_enemyManager;

};