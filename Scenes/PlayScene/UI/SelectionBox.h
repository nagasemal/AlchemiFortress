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
		RECT rect = { 0,0,64,64 },
		DirectX::SimpleMath::Color pulsTextureCol = {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color nomalColor	= {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color onColor		= {0.9f,0.9f,0.9f,1.0f},
		DirectX::SimpleMath::Color pressdColor	= {0.7f,0.7f,0.7f,1.0f});

	// マウスが触れた処理
	bool HitMouse();
	// 左クリックされた処理
	bool ClickMouse();
	// 左クリックされて選択されている状態の処理
	bool SelectionMouse();
	// 左クリックが押され続けている処理
	bool HoldMouse();
	// マウスが離れた処理
	bool ExitMouse();

	// マウスが当たっている判定を変えられます
	void SetHitMouseFlag(bool flag)								{ m_hitMouseFlag = flag; }
	// 選択状態になっている判定を変えられます
	void SetSelectFlag(bool flag)								{ m_selectFlag = flag; }
	// 色を自由に変えられます。(ボックスを強調したい際などに使用)
	void SetLuminousColor(DirectX::SimpleMath::Color color)		{ m_boxColor = color; }
	void SetLuminousFlag(bool flag)								{ m_luminousFlag = flag; };


private:

	bool m_hitMouseFlag;
	bool m_selectFlag;

	DirectX::SimpleMath::Color m_boxColor;
	bool m_luminousFlag;
};
