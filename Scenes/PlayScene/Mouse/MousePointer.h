//--------------------------------------------------------------------------------------
// File: MousePointer.h
//
// �v���C�V�[����ň����B���݂̃}�E�X�ʒu��|���S���ŕ\��
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

	// ������
	void Initialize()	override;

	// �X�V
	void Update()		override;

	// �`��
	void Draw()			override;

	void ModelDraw(DirectX::Model* model);

	// �I������
	void Finalize()		override;

public:

	void HitMachine(SimpleMath::Vector3 pos);

	// �I�u�W�F�N�g�𓮂����ۂɓ�������
	void ObjectDragMode();

	// �I�u�W�F�N�g�𗣂����ۂɓ�������
	void ReleaseLeftButtom();

// �A�N�Z�T
public:

private:

	//// ���̓��C�A�E�g 
	//Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;

	//// �e�N�X�`���n���h�� 
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Texture;

	//// �G�t�F�N�g 
	//std::unique_ptr<DirectX::AlphaTestEffect>			m_BatchEffect;

	std::unique_ptr<DirectX::Model>				m_mouseCursorModel;

};