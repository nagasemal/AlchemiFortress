#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "SelectionBox.h"

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
	void DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,DirectX::Model* model,DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj, DirectX::Model* secondModel = nullptr);

public:

	bool GetHitMouseFlag() { return m_hitMouseFlag; }

	void SetPosition(DirectX::SimpleMath::Vector2 pos) { m_data.pos = pos; }

	void SetMachineType(AlchemicalMachineObject::MACHINE_TYPE type) { m_selectMachineType = type;}
	AlchemicalMachineObject::MACHINE_TYPE GetMachineType()			{ return m_selectMachineType;}

private:

	bool m_hitMouseFlag;

	AlchemicalMachineObject::MACHINE_TYPE m_selectMachineType;

	std::unique_ptr<SelectionBox> m_selectionBox[3];

};