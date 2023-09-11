#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/TitleScene/Camera/TitleCamera.h"
#include "Scenes/TitleScene/TitleLogo/TitleLogo.h"
#include "Scenes/TitleScene/TitleLogo/Veil.h"

class MagicCircle;
class PopLine;
class ShaderButton;

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

	enum ButtonType
	{
		NewGame  = 0,
		Restert	 = 1,
		Option	 = 2,
		Exit	 = 3,
		Num
	};

	std::unique_ptr<DrawMachine>		m_titleCall;
	std::unique_ptr<TitleCamera>		m_titleCamera;
	std::unique_ptr<TitleLogo>			m_titleLogo;

	// �{�^��
	std::unique_ptr<PopLine>			m_selectionButton[ButtonType::Num];

	std::unique_ptr<MagicCircle>		m_magicCircle;

	std::unique_ptr<DirectX::Model>		m_skySphere;

	std::unique_ptr<Veil>				m_veil;

};