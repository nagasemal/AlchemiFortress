#include <pch.h>
#include "ShareData.h"
#include "ShareJsonData.h"

#include "NecromaLib/GameData/BinaryFile.h"

ShareData* ShareData::instance = nullptr;

ShareData::ShareData():
	m_debugFont{nullptr},
	m_commonStates(),
	m_deviceResources(),
	m_spriteBatch(),
	m_camera(),
	m_effectFactory()
{
}

void ShareData::CreateStencilData()
{

	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/ModelShadow_PS.cso");
	// �s�N�Z���V�F�[�_�쐬
	GetDevice()->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_modelShadowShader.ReleaseAndGetAddressOf());

	// ----- �[�x�X�e���V���̍쐬 ----- /

	D3D11_DEPTH_STENCIL_DESC desc = {};


	// �^������鑤(�V���G�b�g���`�悳��鑤)
	{
		desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

		desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
		desc.StencilReadMask = 0x01;		// 0xff
		desc.StencilWriteMask = 0x01;	// 0xff

		// �\��
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;			// ��������ΐ���
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// OK�@�X�e���V���f�[�^���Q�ƒl�Ƃ���
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// NG�@�������Ȃ�
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�

		desc.BackFace = desc.FrontFace;	// ���ʂ�����
		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Base.ReleaseAndGetAddressOf());
	}
	// 	�ʏ�`��
	{
		desc.DepthFunc = D3D11_COMPARISON_NEVER;			// �ȉ��Ȃ�ΐ�������

		desc.StencilEnable = TRUE;
		desc.StencilReadMask = 0x00;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff
		// �\��
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	// �K���ʂ�

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// OK�@�������Ȃ�
		desc.BackFace = desc.FrontFace;	// ���ʂ�����

		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Nomal.ReleaseAndGetAddressOf());
	}
	// �e�`��
	{
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;					// ��ɍX�V
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// �������܂Ȃ�

		desc.StencilEnable = FALSE;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �����Ȃ�Βʂ�

		desc.StencilReadMask = 1;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK�@�������Ȃ�
		desc.BackFace = desc.FrontFace;	// ���ʂ�����

		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Shadow.ReleaseAndGetAddressOf());
	}
}

void ShareData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareData;
	}
}

void ShareData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}