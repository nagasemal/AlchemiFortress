#include "pch.h"
#include "ModelShader.h"

#include "NecromaLib/GameData/BinaryFile.h"
#include "ShareData.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"

ModelShader* ModelShader::instance = nullptr;

ModelShader::ModelShader():
	m_constBuffer()
{

}

void ModelShader::Create()
{
	if (instance == nullptr)
	{
		instance = new ModelShader;
	}
}

void ModelShader::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void ModelShader::CreateModelShader()
{

	ShareData& pSD = ShareData::GetInstance();

	// �V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[
	BinaryFile PSData_Shadow = BinaryFile::LoadFile(L"Resources/Shader/ModelShadow_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_Shadow.GetData(), PSData_Shadow.GetSize(), NULL, m_modelShadowShader.ReleaseAndGetAddressOf());
	
	// �������`��p�̃s�N�Z���V�F�[�_�[
	BinaryFile PSData_Transparent = BinaryFile::LoadFile(L"Resources/Shader/ModelTransparent_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_Transparent.GetData(), PSData_Transparent.GetSize(), NULL, m_modelTransparentShader.ReleaseAndGetAddressOf());

	// �ʏ탂�f���`��p�̃s�N�Z���V�F�[�_�[
	BinaryFile PSData_MyModel = BinaryFile::LoadFile(L"Resources/Shader/MyModelShader_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_MyModel.GetData(), PSData_MyModel.GetSize(), NULL, m_modelMyShader_PS.ReleaseAndGetAddressOf());
	
	// ���f����p�����G�t�F�N�g�p�̃V�F�[�_�[
	BinaryFile PSData_EffectModel = BinaryFile::LoadFile(L"Resources/Shader/MagicEffectShader_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_EffectModel.GetData(), PSData_EffectModel.GetSize(), NULL, m_modelEffect_PS.ReleaseAndGetAddressOf());

	// ���_���󂯓n���p�̃o�[�e�b�N�X�V�F�[�_�[
	BinaryFile VSData_MyModel = BinaryFile::LoadFile(L"Resources/Shader/MyModelShader_VS.cso");
	pSD.GetDevice()->CreateVertexShader(VSData_MyModel.GetData(), VSData_MyModel.GetSize(), NULL, m_modelMyShader_VS.ReleaseAndGetAddressOf());

	//�C���v�b�g���C�A�E�g�̍쐬
	pSD.GetDevice()->CreateInputLayout(VertexPositionNormalTangentColorTexture::InputElements,
		VertexPositionNormalTangentColorTexture::InputElementCount,
		VSData_MyModel.GetData(), VSData_MyModel.GetSize(),
		m_inputLayout.GetAddressOf());

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	ZeroMemory(&m_constBuffer, sizeof(m_constBuffer));
	m_constBuffer.Usage = D3D11_USAGE_DEFAULT;
	m_constBuffer.ByteWidth = sizeof(ConstBuffer);
	m_constBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_constBuffer.CPUAccessFlags = 0;
	pSD.GetDevice()->CreateBuffer(&m_constBuffer, nullptr, &m_cbuffer);

	// ----- �[�x�X�e���V���̍쐬 ----- /

	D3D11_DEPTH_STENCIL_DESC desc = {};

	// �^������鑤(�V���G�b�g���`�悳��鑤)
	{
		//desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
		//desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
		//desc.DepthFunc = D3D11_COMPARISON_ALWAYS;					// �[�x�l�ȉ��Ȃ�X�V����

		//desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
		//desc.StencilReadMask = 0xff;		// 0xff
		//desc.StencilWriteMask = 0xff;	// 0xff

		//// �\��
		//desc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;			// ��������ΐ���
		//desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;		// OK�@�X�e���V���f�[�^���Q�ƒl�Ƃ���
		//desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// NG�@�������Ȃ�
		//desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�

		desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;			// �[�x�l�ȉ��Ȃ�X�V����

		desc.StencilEnable		= TRUE;								// �X�e���V���e�X�g���s��
		desc.StencilReadMask	= 0xff;
		desc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;		// OK�@�X�e���V���f�[�^���Q�ƒl�Ƃ���
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// NG�@�������Ȃ�
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�

		desc.BackFace = desc.FrontFace;	// ���ʂ�����
		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Base.ReleaseAndGetAddressOf());
	}
	// 	�ʏ�`��
	{
		desc.DepthFunc		= D3D11_COMPARISON_EQUAL;			// �ȉ��Ȃ�ΐ�������
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		// �������܂Ȃ�

		desc.StencilEnable = TRUE;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff
		// �\��
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;			// ��������ΐ���
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// OK�@�������Ȃ�
		desc.BackFace = desc.FrontFace;	// ���ʂ�����

		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Nomal.ReleaseAndGetAddressOf());
	}
	// �e�`��
	{
		desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;					// ��ɍX�V
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// �������܂Ȃ�

		desc.StencilEnable = FALSE;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �����Ȃ�Βʂ�

		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	// OK�@�������Ȃ�

		desc.BackFace = desc.FrontFace;	// ���ʂ�����

		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Shadow.ReleaseAndGetAddressOf());
	}

}

void ModelShader::CreateEffectModel()
{

	// ���f���擾
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	// ��`�^�̃��f��
	m_magicTrauabgukarPyram = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/MagicTrauabgukarPyram.cmo", *fx);

}

void ModelShader::ModelDrawShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	ShareData& pSD = ShareData::GetInstance();

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.Time = time;
	cbuff.PaintColor = color;
	cbuff.eyes = SimpleMath::Vector4(pSD.GetCamera()->GetTargetPosition());
	cbuff.LimLightColor = SimpleMath::Color(1.0f, 0.95f, 0.6f, 0.25f);

	SimpleMath::Vector3 mousePos = InputSupport::GetInstance().GetMousePosWolrd();

	cbuff.mousePos = SimpleMath::Vector4(mousePos.x, mousePos.y, mousePos.z,0.0f);

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	pSD.GetContext()->UpdateSubresource(m_cbuffer.Get(), 0, NULL, &cbuff, 0, 0);
	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cbuffer.Get() };
	pSD.GetContext()->PSSetConstantBuffers(2, 1, cb);

	//�C���v�b�g���C�A�E�g�̓o�^
	pSD.GetContext()->IASetInputLayout(m_inputLayout.Get());

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// �������菈��
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

	// �J�����O�͍�����
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// ���@�[�e�b�N�X�V�F�[�_�[�ɓK��
	pSD.GetContext()->VSSetShader(GetModelMyShader_VS().Get(), nullptr, 0);
	// �s�N�Z���V�F�[�_�[�ɓK��
	pSD.GetContext()->PSSetShader(GetModelMyShader_PS().Get(), nullptr, 0);

	pSD.GetContext()->PSSetShaderResources(0, 1, texture.GetAddressOf());

}

void ModelShader::ModelEffectShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	ShareData& pSD = ShareData::GetInstance();

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.Time = time;
	cbuff.PaintColor = color;
	cbuff.eyes = SimpleMath::Vector4(pSD.GetCamera()->GetTargetPosition());
	cbuff.LimLightColor = SimpleMath::Color(1.0f, 0.95f, 0.6f, 0.25f);

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	pSD.GetContext()->UpdateSubresource(m_cbuffer.Get(), 0, NULL, &cbuff, 0, 0);
	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cbuffer.Get() };
	pSD.GetContext()->PSSetConstantBuffers(2, 1, cb);

	//�C���v�b�g���C�A�E�g�̓o�^
	pSD.GetContext()->IASetInputLayout(m_inputLayout.Get());

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// �������菈��
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthRead(), 0);

	// �J�����O�͍�����
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// ���@�[�e�b�N�X�V�F�[�_�[�ɓK��
	pSD.GetContext()->VSSetShader(GetModelMyShader_VS().Get(), nullptr, 0);
	// �s�N�Z���V�F�[�_�[�ɓK��
	pSD.GetContext()->PSSetShader(m_modelEffect_PS.Get(), nullptr, 0);

	pSD.GetContext()->PSSetShaderResources(0, 1, texture.GetAddressOf());

}

void ModelShader::SilhouetteShader()
{
	ShareData& pSD = ShareData::GetInstance();

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// �������菈��
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�X�e���V���X�e�[�g�̐ݒ�(���̂̔w��ɂ��Ă��`�悷�鏈��)
	pSD.GetContext()->OMSetDepthStencilState(GetStencilShadow().Get(), 1);

	// �J�����O�͍�����
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// �V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[��K��
	pSD.GetContext()->PSSetShader(GetModelShadowShader().Get(), nullptr, 0);

}

void ModelShader::ToransparentShader()
{
	ShareData& pSD = ShareData::GetInstance();

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);
	//�������`��w��
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// �������菈��
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthNone(), 1);

	// �J�����O�͍�����
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// �s�N�Z���V�F�[�_�[�ɓK��
	pSD.GetContext()->PSSetShader(GetModelTransparentShader().Get(), nullptr, 0);

}
