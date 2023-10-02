#include "pch.h"
#include "MousePointer.h"
#include "NecromaLib/Singleton/ShareData.h"
#include <WICTextureLoader.h> 
#include "NecromaLib/Singleton/InputSupport.h"

#define POINTER_RAGE			SimpleMath::Vector3(0.5f, 0.5f, 0.5f)
#define POINTER_RAGE_BIG		SimpleMath::Vector3(2.5f, 2.5f, 2.5f)

MousePointer::MousePointer()
{
}

MousePointer::~MousePointer()
{
}

void MousePointer::Initialize()
{
	//�@�������@�@�I�u�W�F�N�g�z�u���͓����蔻���傫������
	m_data.pos = SimpleMath::Vector3::Zero;
	m_data.rage = POINTER_RAGE;

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_mouseCursorModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/MouseCursor.cmo", *fx);

}

void MousePointer::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();

	m_data.pos = pINP.GetMousePosWolrd();
	m_data.pos.y = -1.0f;
	m_data.rage = POINTER_RAGE;

}

void MousePointer::Draw()
{
	ModelDraw(m_mouseCursorModel.get());
}

void MousePointer::ModelDraw(DirectX::Model* model)
{

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(POINTER_RAGE);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos);

	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			//�摜�p�T���v���[�̓o�^
			ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
			pSD.GetContext()->PSSetSamplers(0, 1, sampler);
			//�������`��w��
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// �������菈��
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthNone(), 0);

			// �J�����O�͍�����
			pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

			// �s�N�Z���V�F�[�_�[�ɓK��
			pSD.GetContext()->PSSetShader(pSD.GetModelTransparentShader().Get(), nullptr, 0);
		});

}

void MousePointer::Finalize()
{
}

void MousePointer::HitMachine(SimpleMath::Vector3 pos)
{
	m_data.pos = pos;
	m_data.rage = POINTER_RAGE_BIG;
}

void MousePointer::ObjectDragMode()
{

	m_data.rage = POINTER_RAGE_BIG;

}

void MousePointer::ReleaseLeftButtom()
{

	m_data.rage = POINTER_RAGE;

}
