//--------------------------------------------------------------------------------------
// File: DrawArrow.h
//
//　矢印のUI表示
//  
// 
// Date: 2023.8.25
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class DrawArrow : public GameObjct2D
{
public:
	DrawArrow();
	~DrawArrow();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

private:

};