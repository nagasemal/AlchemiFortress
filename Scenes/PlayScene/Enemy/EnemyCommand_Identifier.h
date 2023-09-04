#pragma once

#include "Scenes/PlayScene/Enemy/EnemyList/ICommand_Enemy.h"

// エネミーの行動クラス識別
ICommand_Enemy* ChangeEnemyMoveCommand(const std::string moveName);

class EnemyCommand_Identifier
{
public:
	EnemyCommand_Identifier();
	~EnemyCommand_Identifier();

private:

};