//--------------------------------------------------------------------------------------
// File		: MachineSelectManager.h
//
// �A���P�~�J���}�V���֘A��UI�Ɋւ��N���X�����
// 
// Use		: AlchemicalMachineManager
// Date		: 2023.5.18
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelect.h"
#include "NecromaLib/GameData/Camera.h"

#include "NecromaLib/GameData/CommonStruct.h"

class FieldObjectManager;
class DisplayMagicCircle;
class UIKeyControl;

class MachineSelectManager
{
public:
	MachineSelectManager();
	~MachineSelectManager();

	void TextuerLoader();

	void Initialize();

	// fieldObjecytManager = UI�ɕω���^����I�u�W�F�N�g�̌��o(�N���X�^��)
	void Update(FieldObjectManager* fieldObjectManager);

	void Render();

	/// <summary>
	/// ���f����`�悷��
	/// </summary>
	/// <param name="model">�x�[�X�ƂȂ郂�f��</param>
	/// <param name="index">�C���f�b�N�X�ԍ�(�}�V���^�C�vID)</param>
	/// <param name="secondModel">���̃��f��</param>
	void ModelRender(DirectX::Model* model,int index,DirectX::Model* secondModel = nullptr);

	void MagicCircleRender();

	void Finalize();

	void SetSelectNoneFlag(bool flag)								{ m_selectNoneFlag = flag; }

public:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextuer()	{ return m_boxTextuer;}

	// �I�𒆂̃}�V���̃^�C�v��Ԃ�
	MACHINE_TYPE GetSelectMachineType()	{ return m_selectMachineType; }

	// �I���{�b�N�X��������Ă��邩��Type�ɉ����ĕԂ�
	bool GetHitMouseToSelectBox(int index)							{ return m_machineSelect[index]->GetHitMouseFlag();}

	// �I���{�b�N�X��Type�ɉ����ĕԂ�
	MachineSelect* GetMachineSelect(int index)						{ return m_machineSelect[index].get();}

	bool GetManufacturingFlag()										{ return m_manufacturingFlag;}

	// ��̗v�f�ł�true�Ȃ�
	bool GetHitMouseToSelectBoxEven()								{ return m_selectBoxAll;}

private:

	// ���\�[�X�Q�����炷
	void ReduceResource(MACHINE_TYPE type);

private:

	// ���f�������锠�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_boxTextuer;

	std::unique_ptr<MachineSelect> m_machineSelect[MACHINE_TYPE::NUM];

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

	MACHINE_TYPE m_selectMachineType;

	bool m_selectBoxAll;
	bool m_manufacturingFlag;

	// �t�B�[���h���None��I�������ۂɁA�S�Ă�MachineSelect�̑I��Box�̐F��ύX����
	bool m_selectNoneFlag;

	std::unique_ptr<Camera> m_camera;

	DirectX::SimpleMath::Matrix m_dispView;
	DirectX::SimpleMath::Matrix m_dispProj;

	//��ʂɑ傫���o�������閂�@�w
	std::unique_ptr<DisplayMagicCircle>			m_displayMagicCircle;

};