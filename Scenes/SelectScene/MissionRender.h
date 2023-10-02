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
struct Stage_Data;

class MissionRender
{
public:

	MissionRender(SimpleMath::Vector2 pos,SimpleMath::Vector2 rage);
	~MissionRender();

	void Render();
	void Render(Stage_Data data);

	// マシン系のミッション内容を描画
	void Render_MachineMission(std::vector<Stage_Condition> stageData);

	// エネミー系ののミッション内容を描画
	void Render_EnemyMission(std::vector<Stage_Condition> stageData);

	// 錬金系のミッション内容を描画
	void Render_AlchemiMission(std::vector<Stage_Condition> stageData);

	// 拠点Lvのミッション内容を描画
	void Render_BaseLvMission(std::vector<Stage_Condition> stageData);

	// タイマー系のミッション内容を描画
	void Render_TimerMission(std::vector<Stage_Condition> stageData);

	// 行の変数を0にする
	void LineReset();

	// ラインのカウンターを返す
	const int GetLineCount() const  { return m_lineCount; }

	const SimpleMath::Vector2 GetPos()  ;
	const SimpleMath::Vector2 GetRage() ;

private:

	void LabelDraw(SimpleMath::Vector2 pos);

private:

	std::unique_ptr<Number> m_number;

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	int m_lineCount;
	int m_missionNum;

};