#include "pch.h"
#include "DisplayMagicCircle.h"
#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

#define ROTATION_SPEED 0.3f
#define MAX_ALPHA 0.35f

const std::vector<D3D11_INPUT_ELEMENT_DESC> DisplayMagicCircle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

DisplayMagicCircle::DisplayMagicCircle()
	: m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_res(nullptr)
	, m_scale(DirectX::SimpleMath::Vector2::One)
	, m_position(DirectX::SimpleMath::Vector2::Zero)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
	, m_rotationMatrix()
	, m_rotateTime()
	, m_transparentTime()
	, m_transparentDeltaTime()
{
}

DisplayMagicCircle::~DisplayMagicCircle()
{
}

/// <summary>
/// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
/// </summary>
/// <param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void DisplayMagicCircle::LoadTexture(const wchar_t* path)
{
	auto device = ShareData::GetInstance().GetDevice();

	DirectX::CreateWICTextureFromFile(device, path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

void DisplayMagicCircle::Create(const wchar_t* path)
{

	auto device = ShareData::GetInstance().GetDevice();
	auto context = ShareData::GetInstance().GetContext();
	auto resouce = ShareData::GetInstance().GetDeviceResources();
	RECT windowSize = resouce->GetOutputSize();

	m_position = {static_cast<float>(windowSize.right / 2),static_cast<float>(windowSize.bottom / 2)};
	m_baseScale = m_scale = {1.2f,1.2f};

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�摜�̓ǂݍ���
	LoadTexture(path);

	SetWindowSize(windowSize.right, windowSize.bottom);

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	m_states = std::make_unique<DirectX::CommonStates>(device);

}


void DisplayMagicCircle::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	m_scale = scale;
}
void DisplayMagicCircle::SetPosition(DirectX::SimpleMath::Vector2 position)
{
	m_position = position;
}

void DisplayMagicCircle::TransparentUpdate(bool spawnFlag)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	if(spawnFlag)	m_transparentDeltaTime += deltaTime * 1.5f;
	else            m_transparentDeltaTime -= deltaTime * 1.5f;

	m_transparentTime = Easing::EaseInCirc(0.0f, MAX_ALPHA, m_transparentDeltaTime);

	if (m_transparentDeltaTime >= 1.0f) m_transparentDeltaTime = 1.0f;
	if (m_transparentDeltaTime <= 0.0f) m_transparentDeltaTime = 0.0f;

	if (m_transparentTime >= MAX_ALPHA) m_transparentTime = MAX_ALPHA;
	if (m_transparentTime <= 0.0f) m_transparentTime = 0.0f;

}

// Shader�쐬�����������������֐�
void DisplayMagicCircle::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_PS.cso");

	//�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);
}

void DisplayMagicCircle::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_rotateTime += deltaTime * ROTATION_SPEED;

	m_rotationMatrix = DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotateTime);

	m_color.A(m_transparentTime);
}

// �`��֐�
void DisplayMagicCircle::Render()
{
	//auto device = ShareData::GetInstance().GetDevice();
	auto context = ShareData::GetInstance().GetContext();
	// ���_���
	// Position.xy	:�g�k�p�X�P�[��
	// Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
	// Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720)
	// Color.zw		:�摜�T�C�Y
	// Tex.xy		:x ���g�p�@y ������������
	DirectX::VertexPositionColorTexture vertex[1] = {
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(ANCHOR::MIDDLE_CENTER))
		, DirectX::SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, DirectX::SimpleMath::Vector2(1,m_transparentTime))
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	cbuff.color = m_color;
	cbuff.rotationMatrix = m_rotationMatrix.Transpose();

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);


	//�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, SpriteLoder::GetInstance().GetMagicCircleTexture(0).GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

void DisplayMagicCircle::SpritebatchRender()
{
	auto& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, m_textureWidth, m_textureHeight };

	DirectX::SimpleMath::Vector2 box_Pos = { static_cast<float>(m_windowWidth) / 2,static_cast<float>(m_windowHeight) / 2 };

	// �I��BOX
	pSB->Draw(SpriteLoder::GetInstance().GetMagicCircleTexture(0).Get(), box_Pos, &srcRect, m_color,m_rotateTime, DirectX::XMFLOAT2((float)m_textureWidth / 2, (float)m_textureHeight / 2), 1.5f);

	pSB->End();
}

void DisplayMagicCircle::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}