//--------------------------------------------------------------------------------------
// File: Tutorial.h
//
//　チュートリアルステージである場合、
//	対応したチュートリアル画像を出す
// 
// Date: 2023.9.4
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class SelectionBox;
class DrawArrow;

class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

private:


};