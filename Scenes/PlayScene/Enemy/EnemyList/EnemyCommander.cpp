#include "pch.h"
#include "EnemyCommander.h"

EnemyCommander::EnemyCommander():
	m_commandNumber(),
	m_commands()
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::AddCommand(ICommand_Enemy* command)
{
	m_commands.push_back(command);
}

void EnemyCommander::Execute_ALL()
{

	for (auto& command : m_commands)
	{
		command->Execute();
	}

}

void EnemyCommander::Execute_One()
{
	int counter = 0;

	for (auto& command : m_commands)
	{
		counter += command->GetCompletion();
	}

	if (m_commands.size() <= counter)
	{
		// ‘S‚Ä‚ÌŠ®—¹Ï‚Ýƒtƒ‰ƒO‚ð‰ðœ‚·‚é
		for (auto& command : m_commands)
		{
			command->SetCompletion(false);
			command->Reset();
		}

		counter = 0;
	}

	m_commands[counter]->Execute();

}

void EnemyCommander::Reset()
{
	m_commands.clear();
}
