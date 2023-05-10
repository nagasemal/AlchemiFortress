#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
//#include "NecromaLib/SceneObject/SceneManager.h"

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

	// 終了処理
	void Finalize() override;


};