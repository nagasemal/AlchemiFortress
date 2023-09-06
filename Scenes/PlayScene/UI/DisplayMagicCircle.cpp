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
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
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

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(path);

	SetWindowSize(windowSize.right, windowSize.bottom);

	// プリミティブバッチの作成
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

// Shader作成部分だけ分離した関数
void DisplayMagicCircle::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/DisplayMagicCircle_PS.cso");

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

void DisplayMagicCircle::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_rotateTime += deltaTime * ROTATION_SPEED;

	m_rotationMatrix = DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotateTime);

	m_color.A(m_transparentTime);
}

// 描画関数
void DisplayMagicCircle::Render()
{
	//auto device = ShareData::GetInstance().GetDevice();
	auto context = ShareData::GetInstance().GetContext();
	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:x 未使用　y 半透明化処理
	DirectX::VertexPositionColorTexture vertex[1] = {
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(ANCHOR::MIDDLE_CENTER))
		, DirectX::SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, DirectX::SimpleMath::Vector2(1,m_transparentTime))
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	cbuff.color = m_color;
	cbuff.rotationMatrix = m_rotationMatrix.Transpose();

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
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, SpriteLoder::GetInstance().GetMagicCircleTexture(0).GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//シェーダの登録を解除しておく
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

	// 画像のサイズ
	RECT srcRect = { 0, 0, m_textureWidth, m_textureHeight };

	DirectX::SimpleMath::Vector2 box_Pos = { static_cast<float>(m_windowWidth) / 2,static_cast<float>(m_windowHeight) / 2 };

	// 選択BOX
	pSB->Draw(SpriteLoder::GetInstance().GetMagicCircleTexture(0).Get(), box_Pos, &srcRect, m_color,m_rotateTime, DirectX::XMFLOAT2((float)m_textureWidth / 2, (float)m_textureHeight / 2), 1.5f);

	pSB->End();
}

void DisplayMagicCircle::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}