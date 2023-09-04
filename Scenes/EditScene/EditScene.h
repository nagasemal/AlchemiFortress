

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/SelectScene/MissionRender.h"

#include <list>

class MissonRender;
class DrawArrow;
class Number;

class EditScene : public SceneObj
{
public:
	// コンストラクタ
	EditScene();

	// デストラクタ
	~EditScene();

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

	void AddMission(std::vector<std::unique_ptr<SelectionBox>> &ui, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 rage);

	void WritingFile();

private:

	int m_stageNum;
	Stage_Data m_stageData;

	std::unique_ptr<MissionRender> m_missionRender;

	// ミッション内容を描画
	std::unique_ptr<DrawArrow> m_machineMissions_puls [MACHINE_TYPE::NUM];
	std::unique_ptr<DrawArrow> m_machineMissions_minus[MACHINE_TYPE::NUM];

	std::vector<std::unique_ptr<SelectionBox>> m_enemyMissions;
	std::vector<std::unique_ptr<SelectionBox>> m_timeMissions;

	// ステージ番号を管理
	std::unique_ptr<Number> m_ui_StageNumber;
	std::unique_ptr<DrawArrow> m_stageNum_puls;
	std::unique_ptr<DrawArrow> m_stageNum_minus;

	// リソース群
	std::unique_ptr<SelectionBox> m_resource_Machine[MACHINE_TYPE::NUM];
	std::unique_ptr<SelectionBox> m_resource_Lv;
	std::unique_ptr<SelectionBox> m_resource_MP;
	std::unique_ptr<SelectionBox> m_resource_Crystal;

	// 読み込みボタン
	std::unique_ptr<SelectionBox> m_writingButton;

	// 決定ボタン
	std::unique_ptr<SelectionBox> m_decisionButton;

};