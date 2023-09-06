#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/TitleScene/Camera/TitleCamera.h"
#include "Scenes/TitleScene/TitleLogo/TitleLogo.h"

class MagicCircle;

class TitleScene : public SceneObj
{
public:
	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

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

	std::unique_ptr<DrawMachine>		m_titleCall;
	std::unique_ptr<TitleCamera>		m_titleCamera;
	std::unique_ptr<TitleLogo>			m_titleLogo;

	std::unique_ptr<MagicCircle>		m_magicCircle;

	std::unique_ptr<DirectX::Model>		m_skySphere;

};