#include "pch.h"
#include "EnemyCommand_Identifier.h"

#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_StandardMove.h"
#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_MeanderingMove.h"
#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_HoppingMove.h"

EnemyCommand_Identifier::EnemyCommand_Identifier()
{
}

EnemyCommand_Identifier::~EnemyCommand_Identifier()
{
}

ICommand_Enemy* ChangeEnemyMoveCommand(const std::string moveName)
{
	if (moveName == "Standard")		return new Enemy_StanderMove();
	if (moveName == "Meandering")	return new Enemy_MeanderingMove();
	if (moveName == "Hopping")		return new Enemy_HoppingMove();

	return nullptr;
}
