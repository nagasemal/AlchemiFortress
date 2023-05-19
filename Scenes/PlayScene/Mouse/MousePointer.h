//--------------------------------------------------------------------------------------
// File: MousePointer.h
//
// プレイシーン上で扱う。現在のマウス位置を板ポリゴンで表示
//
//
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "PrimitiveBatch.h"

class MousePointer : public GameObjct3D
{
public:
	MousePointer();
	~MousePointer();

	// 初期化
	void Initialize()	override;

	// 更新
	void Update()		override;

	// 描画
	void Draw()			override;

	// 終了処理
	void Finalize()		override;

public:

	// オブジェクトを動かす際に動く処理
	void ObjectDragMode();

	// オブジェクトを離した際に動く処理
	void ReleaseLeftButtom();

// アクセサ
public:

private:

	// 入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;

	// テクスチャハンドル 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Texture;

	// エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect>			m_BatchEffect;

	// プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

};