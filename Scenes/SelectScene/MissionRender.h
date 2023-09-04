//--------------------------------------------------------------------------------------
// File: MissionRender.h
//
//　ミッション内容を表示するクラス
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/UI/Number.h"
class Stage_Condition;
class SpriteLoder;
class Stage_Data;

class MissionRender
{
public:

	MissionRender(DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage);
	~MissionRender();

	void Render();
	void Render(Stage_Data data);

	// マシン系のミッション内容を描画
	void Render_MachineMission(std::vector<Stage_Condition> stageData);

	// エネミー系ののミッション内容を描画
	void Render_EnemyMission(std::vector<Stage_Condition> stageData);

	// タイマー系のミッション内容を描画
	void Render_TimerMission(std::vector<Stage_Condition> stageData);

	// 行の変数を0にする
	void LineReset();

	// ラインのカウンターを返す
	int GetLineCount() { return m_lineCount; }

private:

	void LabelDraw(DirectX::SimpleMath::Vector2 pos);

private:

	std::unique_ptr<Number> m_number;

	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_rage;

	int m_lineCount;

};