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

	// エフェクトの作成  (透明度適応)
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成 
	void const* shaderByteCode;
	// ↑結局GPUになんかしら渡さないといけない

	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	// テクスチャのロード 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Fade.png",
		nullptr,
		m_Texture.GetAddressOf()
	);

	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//　初期化　　オブジェクト配置時は当たり判定を大きくする
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

	// 頂点情報（板ポリゴンの頂点） 
	float rage = 1.f;

	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x + m_data.rage.x, m_data.pos.y, m_data.pos.z + m_data.rage.z),	SimpleMath::Vector2(-rage,	-rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x - m_data.rage.x, m_data.pos.y, m_data.pos.z + m_data.rage.z),	SimpleMath::Vector2(rage,	-rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x - m_data.rage.x, m_data.pos.y, m_data.pos.z - m_data.rage.z),	SimpleMath::Vector2(rage,	rage)),
		VertexPositionTexture(SimpleMath::Vector3(m_data.pos.x + m_data.rage.x, m_data.pos.y, m_data.pos.z - m_data.rage.z),	SimpleMath::Vector2(-rage,	rage)),
	};

	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード） 
	ID3D11SamplerState* samplers[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, samplers);

	// 深度バッファに書き込み参照する  Noneはなんかおもろい挙動しだす
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り（反時計回り） ポリゴンの向きを決めている(裏面カリングも可能)(両面カリングも可能)(プレイヤー)
	context->RSSetState(states->CullCounterClockwise());

	// 不透明のみ描画する設定 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(pSD.GetView());
	m_BatchEffect->SetProjection(pSD.GetProjection());
	m_BatchEffect->SetTexture(m_Texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	// 半透明部分を描画 
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
