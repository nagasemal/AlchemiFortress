#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
//#include "NecromaLib/SceneObject/SceneManager.h"

class SelectionBox;
class UIKeyControl;

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

private:

	std::unique_ptr<SelectionBox> m_selectionBox_Next;
	std::unique_ptr<SelectionBox> m_selectionBox_Retry;
	std::unique_ptr<SelectionBox> m_selectionBox_Back;

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

};