

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/SelectScene/MissionRender.h"

#include "Scenes/Commons/DrawVariableNum.h"

#include <list>

class MissonRender;
class DrawArrow;
class Number;
class DrawSlider;
class TitleLogo;
class AM_Attacker;
class MoveCamera;

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

	void AddMission(std::vector<std::unique_ptr<SelectionBox>> &ui, SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);

	void WritingFile();

private:

private:

	// 書き込む生成したデータ
	Stage_Data m_writingData;

	// ミッション内容を描画する
	std::unique_ptr<MissionRender> m_missionRender;

	// マシンの生成関係のユーザー可変数値
	std::unique_ptr<DrawVariableNum> m_userVariable[MISSION_TYPE::MISSION_NUM];

	// リソース群
	std::unique_ptr<DrawVariableNum> m_resource_Machine[MACHINE_TYPE::NUM];
	std::unique_ptr<DrawSlider>		 m_resource_MP;
	std::unique_ptr<DrawSlider>		 m_resource_Crystal;

	// 読み込みボタン
	std::unique_ptr<SelectionBox> m_writingButton;

	// 決定ボタン
	std::unique_ptr<SelectionBox> m_decisionButton;

	std::unique_ptr<MoveCamera> m_camera;

};