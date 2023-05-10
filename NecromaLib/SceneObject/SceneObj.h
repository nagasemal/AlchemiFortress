//--------------------------------------------------------------------------------------
// File: SceneObj.h
//
// 
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/Singleton/ShareData.h"

// �V�[���̗�
enum class GAME_SCENE : int
{
	NONE,

	TITLE,
	PLAY,
	RESULT
};

class SceneObj
{
protected:

public:

	virtual ~SceneObj() = default;

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual GAME_SCENE Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

private:

};