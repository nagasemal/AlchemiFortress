#pragma once

#include "Scenes/PlayScene/Enemy/EnemyList/ICommand_Enemy.h"

// �G�l�~�[�̍s���N���X����
ICommand_Enemy* ChangeEnemyMoveCommand(const std::string moveName);

class EnemyCommand_Identifier
{
public:
	EnemyCommand_Identifier();
	~EnemyCommand_Identifier();

private:

};