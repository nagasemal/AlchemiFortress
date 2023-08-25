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
	Number(DirectX::SimpleMath::Vector2 position,DirectX::SimpleMath::Vector2 rage);
	~Number();

	// 表示する数字の設定
	void SetNumber(int num);
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
	void SetRage(DirectX::SimpleMath::Vector2 rage) { m_rage = rage; }


	// 数字画像描画
	void Render();

	// Missions
	void Render_SelectScene(int first,int next);

private:

	void Number_Render(int num,DirectX::SimpleMath::Vector2 pos);

	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_rage;

	int m_num;

};