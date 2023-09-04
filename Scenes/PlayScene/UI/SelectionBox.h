//--------------------------------------------------------------------------------------
// File		: SelectionBox.h
//
// オブジェクト選択時等に現れる選択可能UI
// 基本機能 触れる　クリック　ホールド　ボタン式　離れた瞬間　取得
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/Commons/SelectionUI.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class SelectionBox : public SelectionUI
{
public:
	SelectionBox(DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage);
	~SelectionBox();

	// 描画
	void Draw()					override;

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT rect = { 0,0,64,64 },
		DirectX::SimpleMath::Color pulsTextureCol = { 1.0f,1.0f,1.0f,1.0f });

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT rect = { 0,0,64,64 },
		DirectX::SimpleMath::Color pulsTextureCol = {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color nomalColor	= {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color onColor		= {0.9f,0.9f,0.9f,1.0f},
		DirectX::SimpleMath::Color pressdColor	= {0.7f,0.7f,0.7f,1.0f});
};
