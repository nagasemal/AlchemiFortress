#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

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
	void DisplayObject(DirectX::Model* model,DirectX::Model* secondModel = nullptr,AlchemicalMachineObject* object = nullptr);

	void ResetMoveTime() { m_moveTime = 0; m_hitFlag = false;}

public:

	// マウスが説明ウィンドウに触れている
	bool OnMouse();

private:

	float m_moveTime;

	DirectX::SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// テキストボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	bool m_hitFlag;

};