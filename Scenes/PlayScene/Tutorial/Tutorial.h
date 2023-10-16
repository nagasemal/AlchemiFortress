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
class DrawLine;
class DrawBox;

class Gauge;
class AlchemicalMachineManager;
class MissionRender;
class AlchemicalMachineObject;
class MachineExplanation;

class Tutorial
{
public:
	enum INSTRUCTION_TYPE : int
	{
		NONE = 0,		// 指定なし
		ATTACKER = 1,		// 攻撃型
		DEFENSER = 2,		// 防御型
		UPPER = 3,		// 範囲内強化型
		RECOVERY = 4,		// 魔力回収型
		MINING = 5,		// 採掘型

		OPERATION = 6,		// 操作方法
		OPERATION_MACHINE = 7,	// 操作方法_マシン
		MACHINE_UI = 8,		// マシンUIの操作について

		GAUGE_HP = 9,		// HPについて
		GAUGE_MP = 10,		// MPについて
		GAUGE_CRYSTAL = 11,		// クリスタルについて

		MISSION = 12,		// ミッションについて
		ALCHEMI = 13,		// 錬金について
		SPAWN = 14,		// 設置について

		NUM
	};

public:
	Tutorial();
	~Tutorial();

	void Initialize(std::vector<int> tutorialNumber);
	void Update(AlchemicalMachineManager* machineManager,Gauge* gauge, MissionRender* missionRender,bool stopFlag = false);
	void Render();
	void Render_Layer2(); // 表示の関係上もう一つ関数が必要であったため
	void Finalize();

	void RelodeTutorial(std::vector<int> tutorialNumber);
	void CreateInterfase();
	
	bool GetTutorialFlag() { return m_explanationFlag || (m_tutorialFlag && m_cameraFlag); }

	// 現在の選択状況からどこにラインを引くか決めます
	void LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge, MissionRender* missionRender,int number);

private:

	static const std::vector<const wchar_t*> FILENAME;

	std::unique_ptr<DrawArrow> m_arrowR,m_arrowL;

	// 説明モードへ移行,解除するボタン
	std::unique_ptr<SelectionBox> m_explanationButton;
	// チュートリアルモードを解除するボタン
	std::unique_ptr<SelectionBox> m_tutorialExitButton;

	std::unique_ptr<DrawLine> m_showLine;	// 何を指しているのか分かるようにする
	std::unique_ptr<DrawBox>  m_showBox;

	std::unique_ptr<UserInterface> m_backFlame;
	std::unique_ptr<UserInterface> m_textTexture;

	std::unique_ptr<AlchemicalMachineObject> m_amMachine;
	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// 説明モードに入っているか否か
	bool m_explanationFlag;

	// チュートリアルモードに入っているか否か
	bool m_tutorialFlag;
	bool m_cameraFlag;

	int m_selectNumber;
	// 0 何もない
	std::vector<int> m_tutorialNumber;

	int m_maxSelectVal;

};