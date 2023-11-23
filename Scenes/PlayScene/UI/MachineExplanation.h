//--------------------------------------------------------------------------------------
// File: MachineExplanation.h
//
//�@�X�v���C�g�o�b�`�@�g�p
//�@�A���P�~�J���}�V���̑I�����A��ʑS�̂ɏo�������閂�@�w�̏���
// 
// Use		: AlchemicalMachineManager
// Date		: 2023.7.17
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineGauge.h"

class MachineExplanation : public GameObjct2D
{
public:
	MachineExplanation();
	~MachineExplanation();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �}�V���f�[�^�[�p�̃A�b�v�f�[�g����
	void Update_MachineData(AlchemicalMachineObject* object);

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	// ���f����`�悷�邽�߂̊֐�
	void DisplayObject(DirectX::Model* model,DirectX::Model* secondModel = nullptr,AlchemicalMachineObject* object = nullptr);

	/// <summary>
	/// �}�V�����N���b�N���A���j���[���J�����ۂɌ����{�^���̃|�C���^�[��Ԃ��܂�
	/// </summary>
	/// <param name="buttonType">0 = �C�U ,1 = �j�� , 2 = ���x���A�b�v</param>
	/// <returns></returns>
	SelectionBox* GetMenuButton(int buttonType);

	// ��̂��ꂽ�u�Ԃ�Ԃ��t���O
	bool GetDismantlingFlag();

	// �C�U���ꂽ�u�Ԃ�Ԃ��t���O
	bool GetRepairFlag();

	// LvUP���ꂽ�u�Ԃ�Ԃ��t���O
	bool GetLvUpFlag();

	void ResetMoveTime() { m_moveTime = 0; m_hitFlag = false;}

public:

	// �}�E�X�������E�B���h�E�ɐG��Ă���
	bool OnMouse();

private:

	float m_moveTime;

	SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// �e�L�X�g�{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// LvUp�p�̑I���{�b�N�X
	std::unique_ptr<SelectionBox> m_selectLvUpBox;
	// �C���p�̑I���{�b�N�X
	std::unique_ptr<SelectionBox> m_repairBox;
	// ��̗p�̑I���{�b�N�X
	std::unique_ptr<SelectionBox> m_dismantlingBox;

	// �}�V����HP�Q�[�W
	//std::unique_ptr<MachineGauge> m_machineGauge;

	bool m_hitFlag;

};