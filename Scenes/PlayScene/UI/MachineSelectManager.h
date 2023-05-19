#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelect.h"

class MachineSelectManager
{
public:
	MachineSelectManager();
	~MachineSelectManager();

	void TextuerLoader();

	void Initialize();

	void Update();

	void Render();

	void ModelRender(DirectX::Model model[]);

	void Finalize();

public:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextuer() { return m_boxTextuer;}

	AlchemicalMachineObject::MACHINE_TYPE GetSelectMachineType() { return m_selectMachineType; }

private:

	// モデルを入れる箱のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_boxTextuer;

	std::unique_ptr<MachineSelect> m_machineSelect[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	AlchemicalMachineObject::MACHINE_TYPE m_selectMachineType;
};