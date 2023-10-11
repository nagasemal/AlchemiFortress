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
#include "Scenes/PlayScene/UI/BaseLv.h"
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/PlayScene/Tutorial/Tutorial.h"
#include "Scenes/PlayScene/Tutorial/Explanation.h"

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

	// フィールドに存在するオブジェクトを管理
	std::unique_ptr<FieldObjectManager>			m_fieldManager;

	// マウスポインターにかかる処理
	std::unique_ptr<MousePointer>				m_mousePointer;

	// マシンユニット/マシンに関連するUIの管理
	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	// カメラに対する処理
	std::unique_ptr<MoveCamera>					m_moveCamera;

	// エネミーを管理
	std::unique_ptr<EnemyManager>				m_enemyManager;

	// ミッションを管理
	std::unique_ptr<MissionManager>				m_missionManager;

	// HP,MP,クリスタルのゲージ
	std::unique_ptr<Gauge>						m_resourceGauge;

	std::unique_ptr<BaseLv>						m_baseLv;

	// チュートリアルクラス
	std::unique_ptr<Tutorial>					m_tutorial;

	std::unique_ptr<Explanation>				m_explanation;

	std::unique_ptr<DirectX::Model>				m_skySphere;

	// 倍速ボタン
	std::unique_ptr<SelectionBox>				m_doubleSpeedButton; 
	// ステージ番号
	int m_stageNumber;
	// 倍速倍率
	int m_doubleSpeedNum;

};