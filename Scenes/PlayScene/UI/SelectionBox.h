//--------------------------------------------------------------------------------------
// File		: SelectionBox.h
//
// �I�u�W�F�N�g�I�������Ɍ����I���\UI
// ��{�@�\ �G���@�N���b�N�@�z�[���h�@�{�^�����@���ꂽ�u�ԁ@�擾
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class SelectionBox : public GameObjct2D
{
public:
	SelectionBox(DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage);
	~SelectionBox();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT rect = { 0,0,64,64 },
		DirectX::SimpleMath::Color pulsTextureCol = {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color nomalColor	= {1.0f,1.0f,1.0f,1.0f},
		DirectX::SimpleMath::Color onColor		= {0.9f,0.9f,0.9f,1.0f},
		DirectX::SimpleMath::Color pressdColor	= {0.7f,0.7f,0.7f,1.0f});

	// �}�E�X���G�ꂽ����
	bool HitMouse();
	// ���N���b�N���ꂽ����
	bool ClickMouse();
	// ���N���b�N����đI������Ă����Ԃ̏���
	bool SelectionMouse();
	// ���N���b�N�������ꑱ���Ă��鏈��
	bool HoldMouse();
	// �}�E�X�����ꂽ����
	bool ExitMouse();

	// �}�E�X���������Ă��锻���ς����܂�
	void SetHitMouseFlag(bool flag)								{ m_hitMouseFlag = flag; }
	// �I����ԂɂȂ��Ă��锻���ς����܂�
	void SetSelectFlag(bool flag)								{ m_selectFlag = flag; }
	// �F�����R�ɕς����܂��B(�{�b�N�X�������������ۂȂǂɎg�p)
	void SetLuminousColor(DirectX::SimpleMath::Color color)		{ m_boxColor = color; }
	void SetLuminousFlag(bool flag)								{ m_luminousFlag = flag; };


private:

	bool m_hitMouseFlag;
	bool m_selectFlag;

	DirectX::SimpleMath::Color m_boxColor;
	bool m_luminousFlag;
};
