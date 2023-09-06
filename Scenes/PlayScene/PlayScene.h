#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"
#include "Camera/MoveCamera.h"

// ゲームのオブジェクト
#include "Field/FieldObjectManager.h"
#include "Mouse/MousePointer.h"
#include "AlchemicalMachine/AlchemicalMachineManager.h"
#include "Enemy/EnemyManager.h"

// ミッション
#include "Scenes/PlayScene/Stage/MissionManager.h"

// UI関連
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/PlayScene/Tutorial/Tutorial.h"

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

	// テスト用
	std::unique_ptr<Gauge>						m_gauge;

	std::unique_ptr<DirectX::Model>				m_skySphere;

	std::unique_ptr<Tutorial>					m_tutorial;

	// ステージ番号
	int m_stageNumber;

};