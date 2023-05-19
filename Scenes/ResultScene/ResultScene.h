#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
//#include "NecromaLib/SceneObject/SceneManager.h"

class ResultScene : public SceneObj
{
public:
	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update() override;

	// �`��
	void Draw() override;

	// UI�n�`��
	void DrawUI() override;

	// �I������
	void Finalize() override;


};