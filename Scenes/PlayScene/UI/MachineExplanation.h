#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"

class MachineExplanation : public GameObjct2D
{
public:
	MachineExplanation();
	~MachineExplanation();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

	// モデルを描画するための関数
	void DisplayObject(DirectX::Model* model);

	void ResetMoveTime() { m_moveTime = 0; }

private:

	float m_moveTime;

	DirectX::SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// テキストボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};