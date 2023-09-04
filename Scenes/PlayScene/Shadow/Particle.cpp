#include "pch.h"
#include "Particle.h"
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include <algorithm>
#include <random>

// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR"	,	0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,  sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Particle::Particle(EFFECT_TYPE type) :
	m_particleNum(1),
	m_particleSpawnTime(0),
	m_colFlag(),
	m_timer(),
	m_billboard(),
	m_proj(),
	m_view(),
	m_effectType(type)
{

}

Particle::~Particle()
{
}

void Particle::LoadTexture(const wchar_t* path)
{
	auto device = ShareData::GetInstance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

void Particle::CreateBillboard()
{

	auto camera = ShareData::GetInstance().GetCamera();

	m_billboard =
		DirectX::SimpleMath::Matrix::CreateBillboard(DirectX::SimpleMath::Vector3::Zero,
			camera->GetEyePosition() - camera->GetTargetPosition(), camera->GetUpVector());

	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = camera->GetEyePosition();
	m_cameraTarget = camera->GetTargetPosition();
	m_billboard = rot * m_billboard;
}

void Particle::Initialize()
{
	auto device = ShareData::GetInstance().GetDevice();

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�摜�̓ǂݍ���
	LoadTexture(L"Resources/Textures/Particle.png");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique <DirectX::PrimitiveBatch <DirectX::VertexPositionColorTexture>> (ShareData::GetInstance().GetContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

}

void Particle::Update(DirectX::SimpleMath::Vector3 pos)
{
	m_timer += DeltaTime::GetInstance().GetDeltaTime();

	CreateBillboard();

	// ���Ԃ������琶�����s��
	if (m_timer >= m_particleSpawnTime)
	{
		for (int i = 0; i < m_particleNum; i++)
		{
			m_particleUtility.push_back(CreateEffectParam(m_effectType,pos));
		}

		m_timer = 0.0f;

	}
}

bool Particle::OnShot(DirectX::SimpleMath::Vector3 pos, bool flag, DirectX::SimpleMath::Color color)
{
	// flase�Ȃ�ʂ��Ȃ�
	if (!flag) return false;

	CreateBillboard();

	// ��������������
	for (int i = 0; i < m_particleNum; i++)
	{
		m_particleUtility.push_back(CreateEffectParam(m_effectType,pos, color));
	}

	return false;

}

void Particle::UpdateParticle()
{
	//Particle�̗��̍X�V�������s��
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// �q�N���X����false�ŏ���
		if (!(ite)->Update())
		{
			ite = m_particleUtility.erase(ite);
			if (ite == m_particleUtility.end()) break;
		}
	}
}

void Particle::Render()
{
	auto context = ShareData::GetInstance().GetContext();
	auto view = ShareData::GetInstance().GetView();
	auto proj = ShareData::GetInstance().GetProjection();

	// ���_���(�|���S���̂S���_�̍��W���j
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs,ParticleUtility  rhs)
		{
			//�J�������ʂ̋����Ń\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});

	//�o�^����Ă��钸�_�����Z�b�g
	m_vertices.clear();

	for (auto& li : m_particleUtility)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
			continue;
		}

		DirectX::VertexPositionColorTexture vPCT;

		vPCT.position = DirectX::XMFLOAT3(li.GetPosition());
		vPCT.color = DirectX::XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = DirectX::XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//�\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())
	{
		return;
	}

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

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
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//// �|���S����`��
	m_batch->Begin();
	////m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//�J�������ʂ̋����Ń\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
}

void Particle::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_PS.cso");

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

ParticleUtility Particle::CreateEffectParam(EFFECT_TYPE type, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Color color)
{

	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_real_distribution<> dist(0, DirectX::XM_2PI);
	float range = 0.5f;
	float rand = static_cast<float>(dist(engine));

	ParticleUtility pU(
		1.0f,																					// ��������
		DirectX::SimpleMath::Vector3::Zero,														// ����W
		DirectX::SimpleMath::Vector3::Zero,														// ���x
		DirectX::SimpleMath::Vector3::Zero,														// �����x
		DirectX::SimpleMath::Vector3(0.7f, 0.7f, 0.7f), DirectX::SimpleMath::Vector3::Zero,		// �����X�P�[���A�ŏI�X�P�[��
		DirectX::SimpleMath::Color(color), DirectX::SimpleMath::Color(0.f, 0.f, 0.f, 0.f)		// �����J���[�A�ŏI�J���[
	);

	DirectX::SimpleMath::Vector3 vectol = pU.GetPosition() - pos;


	switch (type)
	{
	case Particle::HIT_BULLET:

		// �g�U����G�t�F�N�g

		pU.SetLife(0.8f);

		pU.SetPosition(DirectX::SimpleMath::Vector3(
			pos.x + range * cosf(rand),
			pos.y + range * sinf(rand),
			pos.z + range * cosf(rand)));

		pU.SetVelocity(DirectX::SimpleMath::Vector3(0.0f,0.8f,0.0f));

		pU.SetEndColor({color.R(),color.G(),color.B(),0.0f});

		m_particleNum = 5;

		break;
	case Particle::SPAWN_ENEMY:
		pU.SetPosition(DirectX::SimpleMath::Vector3(pos.x + range * cosf(rand),
			pos.y,
			pos.z + range * sinf(rand)));

		pU.SetVelocity(DirectX::SimpleMath::Vector3(0.0f, 0.8f, 0.0f));

		m_particleNum = 10;
		break;
	case Particle::DELETE_ENEMY:

		pU.SetPosition(DirectX::SimpleMath::Vector3(pos.x + range * cosf(rand),
													pos.y,
													pos.z + range * sinf(rand)));

		pU.SetVelocity(DirectX::SimpleMath::Vector3(0.0f, 0.8f, 0.0f));
		pU.SetAccele(DirectX::SimpleMath::Vector3(0.0f, -0.025f, 0.0f));

		m_particleNum = 10;
		break;
	case Particle::MACHINE_LVUP:

		break;
	default:
		break;
	}

	return pU;
}