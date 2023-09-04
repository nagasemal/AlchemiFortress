#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
//#include "NecromaLib/SceneObject/SceneManager.h"

class SelectionBox;
class UIKeyControl;

class ResultScene : public SceneObj
{
public:
	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

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

	std::unique_ptr<SelectionBox> m_selectionBox_Next;
	std::unique_ptr<SelectionBox> m_selectionBox_Retry;
	std::unique_ptr<SelectionBox> m_selectionBox_Back;

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

};