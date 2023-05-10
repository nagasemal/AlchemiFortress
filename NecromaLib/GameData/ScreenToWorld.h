//--------------------------------------------------------------------------------------
// File: ScreenToWorld.h
//
// スクリーン座標をワールド座標に変換するためのクラス
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

DirectX::SimpleMath::Vector3 CalcScreenToXZN(
	int sX,		// スクリーンのX座標
	int sY,		// スクリーンのY座標
	int screen_W, // 横幅
	int screen_H, // 立幅
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix prj
);

// 呼び出す際はこっち
DirectX::SimpleMath::Vector3 CalcScreenToWorldN(
	int sX,		// スクリーンのX座標
	int sY,		// スクリーンのY座標
	float fZ,	// 射影空間でのZ値(0~1)
	int screen_W, // 横幅
	int screen_H, // 立幅
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix prj
);