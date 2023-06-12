#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"
#include "Camera/MoveCamera.h"

// ゲームのオブジェクト
#include "AlchemicalMachine/PlayerBase.h"
#include "Field/Field.h"
#include "Mouse/MousePointer.h"
#include "AlchemicalMachine/AlchemicalMachineManager.h"
#include "Enemy/EnemyManager.h"

class PlayScene : public SceneObj
{
public:
	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update() override;

	// 描画
	void Draw() override;

	// UI系描画
	void DrawUI() override;

	// 終了処理
	void Finalize() override;

private:

	std::unique_ptr<PlayerBase>					m_playerBase;

	std::unique_ptr<Field>						m_field;

	std::unique_ptr<MousePointer>				m_mousePointer;

	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	std::unique_ptr<MoveCamera>					m_moveCamera;

	std::unique_ptr<EnemyManager>				m_enemyManager;

};