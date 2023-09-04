//--------------------------------------------------------------------------------------
// File: UIKeyControl.h
//
// キーボード上でUIを操作する処理
// 
// Date: 2023.8.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include <vector>

class SelectionUI;

class UIKeyControl
{
public:

	struct UI_Data
	{
		SelectionUI* ui;
		int low; // 行
		int col; // 列
	};

public:
	UIKeyControl();
	~UIKeyControl();

	void Update();

	void AddUI(SelectionUI* ui,int low,int col);

	void DeleteUI(SelectionUI* ui);

private:

	bool ArrowKeyPush();

private:

	std::vector<UI_Data> m_selectionUIs;

	int m_nowCol; // 現在の行
	int m_nowLow; // 現在の列

	int m_maxCol; // 現在の行の最大値

	std::vector<int> m_maxLows;

	DirectX::SimpleMath::Vector2 m_prevMousePos; // 前のマウスのポジションの取得
	bool m_keyContorlFlag;

};