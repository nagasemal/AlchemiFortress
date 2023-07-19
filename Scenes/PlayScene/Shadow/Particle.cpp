#include "pch.h"
#include "Particle.h"
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include <algorithm>
#include <random>

// インプットレイアウト
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

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(L"Resources/Textures/Particle.png");

	// プリミティブバッチの作成
	m_batch = std::make_unique <DirectX::PrimitiveBatch <DirectX::VertexPositionColorTexture>> (ShareData::GetInstance().GetContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

}

void Particle::Update(DirectX::SimpleMath::Vector3 pos)
{
	m_timer += DeltaTime::GetInstance().GetDeltaTime();

	CreateBillboard();

	// 時間が来たら生成を行う
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
	// flaseなら通さない
	if (!flag) return false;

	CreateBillboard();

	// 個数分生成をする
	for (int i = 0; i < m_particleNum; i++)
	{
		m_particleUtility.push_back(CreateEffectParam(m_effectType,pos, color));
	}

	return false;

}

void Particle::UpdateParticle()
{
	//Particleの粒の更新処理を行う
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// 子クラスからfalseで消す
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

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs,ParticleUtility  rhs)
		{
			//カメラ正面の距離でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});

	//登録されている頂点をリセット
	m_vertices.clear();

	for (auto& li : m_particleUtility)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}

		DirectX::VertexPositionColorTexture vPCT;

		vPCT.position = DirectX::XMFLOAT3(li.GetPosition());
		vPCT.color = DirectX::XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = DirectX::XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//// 板ポリゴンを描画
	m_batch->Begin();
	////m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//カメラ正面の距離でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
}

void Particle::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_PS.cso");

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
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
		1.0f,																					// 生存時間
		DirectX::SimpleMath::Vector3::Zero,														// 基準座標
		DirectX::SimpleMath::Vector3::Zero,														// 速度
		DirectX::SimpleMath::Vector3::Zero,														// 加速度
		DirectX::SimpleMath::Vector3(0.7f, 0.7f, 0.7f), DirectX::SimpleMath::Vector3::Zero,		// 初期スケール、最終スケール
		DirectX::SimpleMath::Color(color), DirectX::SimpleMath::Color(0.f, 0.f, 0.f, 0.f)		// 初期カラー、最終カラー
	);

	DirectX::SimpleMath::Vector3 vectol = pU.GetPosition() - pos;


	switch (type)
	{
	case Particle::HIT_BULLET:

		// 拡散するエフェクト

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
