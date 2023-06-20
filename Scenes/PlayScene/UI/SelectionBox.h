//--------------------------------------------------------------------------------------
// File: SelectionBox.h
//
//　オブジェクト選択時等に現れる選択可能UI
// 
// Date: 2023.6.12
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class SelectionBox : public GameObjct2D
{
public:
	SelectionBox(DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage);
	~SelectionBox();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
				RECT rect = {0,0,64,64});

	// マウスが触れた処理
	bool HitMouse();
	// 左クリックされた処理
	bool ClickMouse();
	// マウスが離れた処理
	bool ExitMouse();

private:

	bool m_hitMouseFlag;

};
