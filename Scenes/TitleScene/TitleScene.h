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
	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update() override;

	// 描画
	void Draw() override;

	// UI系描画
	void DrawUI() override;

	// 終了処理
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

	// ボタン
	std::unique_ptr<PopLine>			m_selectionButton[ButtonType::Num];

	std::unique_ptr<MagicCircle>		m_magicCircle;

	std::unique_ptr<DirectX::Model>		m_skySphere;

	std::unique_ptr<Veil>				m_veil;

};