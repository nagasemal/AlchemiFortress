#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "SelectionBox.h"

class MachineSelect : public GameObjct2D
{
public:
	MachineSelect();
	~MachineSelect();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	// ���f����`�悷�邽�߂̊֐�
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