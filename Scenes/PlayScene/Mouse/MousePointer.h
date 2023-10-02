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
#include "NecromaLib/GameData/Animation.h"

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

	void ModelDraw(DirectX::Model* model);

	// 終了処理
	void Finalize()		override;

public:

	void HitMachine(SimpleMath::Vector3 pos);

	// オブジェクトを動かす際に動く処理
	void ObjectDragMode();

	// オブジェクトを離した際に動く処理
	void ReleaseLeftButtom();

// アクセサ
public:

private:

	//// 入力レイアウト 
	//Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;

	//// テクスチャハンドル 
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Texture;

	//// エフェクト 
	//std::unique_ptr<DirectX::AlphaTestEffect>			m_BatchEffect;

	std::unique_ptr<DirectX::Model>				m_mouseCursorModel;

};