#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelect.h"
#include "NecromaLib/GameData/Camera.h"

class MachineSelectManager
{
public:
	MachineSelectManager();
	~MachineSelectManager();

	void TextuerLoader();

	void Initialize();

	void Update();

	void Render();

	void ModelRender(DirectX::Model* model,int index);

	void Finalize();

public:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextuer()	{ return m_boxTextuer;}

	// �I�𒆂̃}�V���̃^�C�v��Ԃ�
	AlchemicalMachineObject::MACHINE_TYPE GetSelectMachineType()	{ return m_selectMachineType; }

	bool GetHitMouseToSelectBox(int index)							{ return m_machineSelect[index]->GetHitMouseFlag();}

	// ��̗v�f�ł�true�Ȃ�
	bool GetHitMouseToSelectBoxEven()								{ return m_selectBoxAll; }

private:

	// ���f�������锠�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_boxTextuer;

	std::unique_ptr<MachineSelect> m_machineSelect[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	AlchemicalMachineObject::MACHINE_TYPE m_selectMachineType;

	bool m_selectBoxAll;

	std::unique_ptr<Camera> m_camera;

};