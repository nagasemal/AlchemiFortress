#include "pch.h"
#include "BaseLv.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"

#define SECOND_COLOR SimpleMath::Color(0.2f,0.2f,0.8f,1.0f);

BaseLv::BaseLv()
{
	auto device = ShareData::GetInstance().GetDevice();

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);


	auto windowSize = ShareData::GetInstance().GetDeviceResources()->GetOutputSize();
	Create(SpriteLoder::GetInstance().GetMagicCircleTexturePath());
	LoadShaderFile(L"BaseLv");
	SetWindowSize(windowSize.right, windowSize.bottom);
	SetColor(SimpleMath::Color(1.0f,1.0f,1.0f,1.0f));
	SetPosition(SimpleMath::Vector2(70.0f, 70.0f));
	SetScale(SimpleMath::Vector2(0.15f, 0.15f));

	m_baseLvRender = std::make_unique<Number>(m_position,m_scale * 5.0f);

	m_expRedio = 0.0f;
}

BaseLv::~BaseLv()
{
}

void BaseLv::Update(FieldObjectManager* pFieldObjectManager)
{

	auto pPlayerBase = pFieldObjectManager->GetPlayerBase();

	// ���_�̌���Lv��o�^����
	m_baseLvRender->SetNumber(pPlayerBase->GetBaseLv());

	m_expRedio = (float)pPlayerBase->GetExp() / (float)pPlayerBase->GetNextLvEXP();

}

void BaseLv::Render()
{

	auto context = ShareData::GetInstance().GetContext();
	// ���_���
	// Position.xy	:�g�k�p�X�P�[��
	// Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
	// Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720)
	// Color.zw		:�摜�T�C�Y
	// Tex.xy		:�E�B���h�E�T�C�Y�i�o�b�t�@�������B������͖��g�p�j
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(MIDDLE_CENTER))
		, SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(m_expRedio,0.8f))
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	cbuff.base_color = m_color;
	cbuff.second_color = SECOND_COLOR;

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	AdvanceRender();

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMagicRule(0).GetAddressOf());
	//context->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetAlchemiButtonTexture().GetAddressOf());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	// ���_Lv�̕`��
	m_baseLvRender->Render();

}
