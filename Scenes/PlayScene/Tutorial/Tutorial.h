//--------------------------------------------------------------------------------------
// File: Tutorial.h
//
//　チュートリアルステージである場合、
//	対応したチュートリアル画像を出す
// 
// Date: 2023.9.4
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class SelectionBox;
class DrawArrow;
class UserInterface;
class PopLine;

class Gauge;
class AlchemicalMachineManager;

class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	void Update(AlchemicalMachineManager* machineManager,Gauge* gauge);
	void Render();
	void Render_Layer2(); // 表示の関係上もう一つ関数が必要であったため
	void Finalize();

	void CreateInterfase();
	
	bool GetTutorialFlag() { return m_tutorialFlag; }

	// 現在の選択状況からどこにラインを引くか決めます
	void LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge);

private:

	static const std::vector<const wchar_t*> FILENAME;

	std::unique_ptr<DrawArrow> m_arrowR,m_arrowL;
	std::unique_ptr<SelectionBox> m_cancelButton;
	std::unique_ptr<SelectionBox> m_doubleSpeedButton; // 倍速ボタン

	std::unique_ptr<PopLine> m_showLine;	// 何を指しているのか分かるようにする

	std::unique_ptr<UserInterface> m_backFlame;
	std::unique_ptr<UserInterface> m_textTexture;

	bool m_tutorialFlag;
	int m_selectNumber;
	int m_doubleSpeedNum; // 倍速倍率

};