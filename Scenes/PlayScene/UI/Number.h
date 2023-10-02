//--------------------------------------------------------------------------------------
// File		: Number.h
//
// スプライト画像による数字の描画に使用
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.17
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class Number
{
public:
	Number();
	Number(SimpleMath::Vector2 position,SimpleMath::Vector2 rage);
	~Number();

	// 表示する数字の設定
	void SetNumber(int num);
	void SetPosition(SimpleMath::Vector2 position) { m_position = position; }
	void SetRage(SimpleMath::Vector2 rage) { m_rage = rage; }

	void SetColor(SimpleMath::Color color) { m_color = color; }

	// 数字画像描画
	void Render();

	// Missions
	void Render_SelectScene(int first,int next);

private:

	// 数字を描画する
	void Number_Render(int num,SimpleMath::Vector2 pos);

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	SimpleMath::Color m_color;

	int m_num;

};