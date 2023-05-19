#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class MachineSelect : public GameObjct2D
{
public:
	MachineSelect();
	~MachineSelect();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

	// モデルを描画するための関数
	void DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,DirectX::Model* model);

private:

};