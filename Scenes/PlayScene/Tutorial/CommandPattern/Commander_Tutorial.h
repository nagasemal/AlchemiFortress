#pragma once

//#include "ICommand_Tutorial.h"
#include <vector>

class ICommand_Tutorial;

class TutorialCommander
{

public:
	enum TutorialType :int
	{
		NONE,						// チュートリアル処理がない
		ATTACKER_UI,				// マシンUIへの誘導を行う
		DEFENSER_UI,				// ディフェンサーマシンUIへの誘導を行う
		UPPER_UI,					// アッパーマシンUIへの誘導を行う
		RECOVERY_UI,				// リカバリーマシンUIへの誘導を行う
		MINING_UI,					// マイニングマシンUIへの誘導を行う

		ALCHEMI_UI,					// 錬金ボタンUIへの誘導を行う

		HP_UI,						// HPゲージUIへの誘導を行う
		MP_UI,						// MPゲージUIへの誘導を行う
		CRYSTAL_UI,					// 結晶ゲージUIへの誘導を行う

		REPEA_UI,					// 修繕UIへの誘導を行う
		DESTOROY_UI,				// 破壊UIへの誘導を行う
		LVUP_UI,					// LVUPUIへの誘導を行う

		LR_BUTTON_LOCK,				// マシンUIの移動を制限する

		NONE_MACHINE_SELECT,		// ノーンマシンへの視線誘導を行う
		PROPHERAL_ATTACKER_MACHINE,	// アタッカーマシン周辺のマシンに視線誘導させる
		ENEMY_ATTENTION,			// エネミー出現位置に視線誘導を行う
		BREAK_MACHINE,				// マシンを破壊する
		REPAIR_MACHINE,				// マシンを修繕する

		TEXT_ARROW,					// チュートリアルのテキストを流す

		ALCHEMI_LOCK				// 錬金ボタンをロック/解除する

	};

public:

	std::vector<ICommand_Tutorial*> GetCommand() { return m_commands; }

	// コマンドを追加する
	void AddCommand(ICommand_Tutorial* command, TutorialManager* object)
	{
		command->SetTutorialManagerPtr(object);
		m_commands.push_back(command);
	}

public:

	TutorialCommander() { m_commandNumber = 0; }
	~TutorialCommander() 
	{
	};

	// 全て同時に回す
	void Execute_ALL()
	{

		for (auto& command : m_commands)
		{
			command->Execute();
		}

	};	

	// 配列順に回し、完了したものは回さない
	void Execute_One()
	{
		int counter = 0;

		// 処理が完了したら次のコマンドへ移行する
		for (auto& command : m_commands)
		{
			counter += command->GetCompletion();

			// 終了処理を行う
			if (command->GetCompletion())
			{
				command->Finalize();
			}

		}

		if (counter >= m_commands.size()) return;

		m_commands[counter]->Initialize();
		m_commands[counter]->Execute();

	};

	void Reset()
	{
		for (auto& command : m_commands)
		{
			delete command;
		}
		m_commands.clear();
		m_commands.shrink_to_fit();
	};

private:

	std::vector<ICommand_Tutorial*> m_commands;

	int m_commandNumber;
};