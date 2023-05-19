#include "pch.h"
#include "MousePointer.h"
#include "NecromaLib/Singleton/ShareData.h"
#include <WICTextureLoader.h> 
#include "NecromaLib/Singleton/InputSupport.h"

#define POINTER_RAGE			SimpleMath::Vector3(0.5f, 0.1f, 0.5f)
#define POINTER_RAGE_BIG		SimpleMath::Vector3(1.5f, 0.1f, 1.5f)

MousePointer::MousePointer()
{
}

MousePointer::~MousePointer()
{
}

void MousePointer::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	auto device = pSD.GetDevice();
	auto context = pSD.GetContext();

	// �G�t�F�N�g�̍쐬  (�����x�K��)
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	// ���̓��C�A�E�g���� 
	void const* shaderByteCode;
	// ������GPU�ɂȂ񂩂���n���Ȃ��Ƃ����Ȃ�

	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	// �e�N�X�`���̃��[�h 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Fade.png",
		nullptr,
		m_Texture.GetAddressOf()
	);

	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//�@�������@�@�I�u�W�F�N�g�z�u���͓����蔻���傫������
	m_data.pos = SimpleMath::Vector3::Zero;
	m_data.rage = POINTER_RAGE;

}

void MousePointer::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();

	m_data.pos = pINP.GetMousePosWolrd();
	m_data.pos.y = -1.f;

}

void MousePointer::Draw()
{

	ShareData& pSD = ShareData::GetInstance();

	auto device = pSD.GetDevice();
	auto context = pSD.GetContext();
	auto states = pSD.GetCommonStates();

	// ���_���i�|���S���̒��_�j 
	float rage = 1.f;

	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x + m_data.rage.x, m_data.pos.y, m_data.pos.z + m_data.rage.z),	SimpleMath::Vector2(-rage,	-rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x - m_data.rage.x, m_data.pos.y, m_data.pos.z + m_data.rage.z),	SimpleMath::Vector2(rage,	-rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x - m_data.rage.x, m_data.pos.y, m_data.pos.z - m_data.rage.z),	SimpleMath::Vector2(rage,	rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x + m_data.rage.x, m_data.pos.y, m_data.pos.z - m_data.rage.z),	SimpleMath::Vector2(-rage,	rage)),
	};

	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j 
	ID3D11SamplerState* samplers[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, samplers);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���  None�͂Ȃ񂩂����낢����������
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// �J�����O�͍�����i�����v���j �|���S���̌��������߂Ă���(���ʃJ�����O���\)(���ʃJ�����O���\)(�v���C���[)
	context->RSSetState(states->CullCounterClockwise());

	// �s�����̂ݕ`�悷��ݒ� 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(pSD.GetView());
	m_BatchEffect->SetProjection(pSD.GetProjection());
	m_BatchEffect->SetTexture(m_Texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	// ������������`�� 
	m_Batch->Begin();

	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	m_Batch->End();
}

void MousePointer::Finalize()
{
}

void MousePointer::ObjectDragMode()
{

	m_data.rage = POINTER_RAGE_BIG;

}

void MousePointer::ReleaseLeftButtom()
{

	m_data.rage = POINTER_RAGE;

}
