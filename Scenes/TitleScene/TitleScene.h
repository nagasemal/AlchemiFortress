#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/TitleScene/Camera/TitleCamera.h"
#include "Scenes/TitleScene/TitleLogo/TitleLogo.h"

class MagicCircle;

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

	std::unique_ptr<DrawMachine>		m_titleCall;
	std::unique_ptr<TitleCamera>		m_titleCamera;
	std::unique_ptr<TitleLogo>			m_titleLogo;

	std::unique_ptr<MagicCircle>		m_magicCircle;

	std::unique_ptr<DirectX::Model>		m_skySphere;

};