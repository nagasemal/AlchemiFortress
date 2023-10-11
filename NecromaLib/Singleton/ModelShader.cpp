#include "pch.h"
#include "ModelShader.h"

#include "NecromaLib/GameData/BinaryFile.h"
#include "ShareData.h"

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

	BinaryFile PSData_Shadow = BinaryFile::LoadFile(L"Resources/Shader/ModelShadow_PS.cso");
	// ピクセルシェーダ作成
	pSD.GetDevice()->CreatePixelShader(PSData_Shadow.GetData(), PSData_Shadow.GetSize(), NULL, m_modelShadowShader.ReleaseAndGetAddressOf());

	BinaryFile PSData_Transparent = BinaryFile::LoadFile(L"Resources/Shader/ModelTransparent_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_Transparent.GetData(), PSData_Transparent.GetSize(), NULL, m_modelTransparentShader.ReleaseAndGetAddressOf());

	BinaryFile PSData_MyModel = BinaryFile::LoadFile(L"Resources/Shader/MyModelShader_PS.cso");
	pSD.GetDevice()->CreatePixelShader(PSData_MyModel.GetData(), PSData_MyModel.GetSize(), NULL, m_modelMyShader_PS.ReleaseAndGetAddressOf());
	BinaryFile VSData_MyModel = BinaryFile::LoadFile(L"Resources/Shader/MyModelShader_VS.cso");
	pSD.GetDevice()->CreateVertexShader(VSData_MyModel.GetData(), VSData_MyModel.GetSize(), NULL, m_modelMyShader_VS.ReleaseAndGetAddressOf());

	//インプットレイアウトの作成
	pSD.GetDevice()->CreateInputLayout(VertexPositionNormalTangentColorTexture::InputElements,
		VertexPositionNormalTangentColorTexture::InputElementCount,
		VSData_MyModel.GetData(), VSData_MyModel.GetSize(),
		m_inputLayout.GetAddressOf());

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	ZeroMemory(&m_constBuffer, sizeof(m_constBuffer));
	m_constBuffer.Usage = D3D11_USAGE_DEFAULT;
	m_constBuffer.ByteWidth = sizeof(ConstBuffer);
	m_constBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_constBuffer.CPUAccessFlags = 0;
	pSD.GetDevice()->CreateBuffer(&m_constBuffer, nullptr, &m_cbuffer);

	// ----- 深度ステンシルの作成 ----- /

	D3D11_DEPTH_STENCIL_DESC desc = {};

	// 型抜かれる側(シルエットが描画される側)
	{
		desc.DepthEnable = TRUE;									// 深度テストを行う
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する

		desc.StencilEnable = TRUE;									// ステンシルテストを行う
		desc.StencilReadMask = 0x01;		// 0xff
		desc.StencilWriteMask = 0x01;	// 0xff

		// 表面
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;			// 等しければ成功
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// OK　ステンシルデータを参照値とする
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// NG　何もしない
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// NG　何もしない

		desc.BackFace = desc.FrontFace;	// 裏面も同じ
		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Base.ReleaseAndGetAddressOf());
	}
	// 	通常描画
	{
		desc.DepthFunc = D3D11_COMPARISON_NEVER;			// 以下ならば成功する

		desc.StencilEnable = TRUE;
		desc.StencilReadMask = 0x00;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff
		// 表面
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	// 必ず通す

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// OK　何もしない
		desc.BackFace = desc.FrontFace;	// 裏面も同じ

		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Nomal.ReleaseAndGetAddressOf());
	}
	// 影描画
	{
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;					// 常に更新
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// 書き込まない

		desc.StencilEnable = FALSE;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// 同じならば通す

		desc.StencilReadMask = 0x00;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	// OK　何もしない

		desc.BackFace = desc.FrontFace;	// 裏面も同じ

		pSD.GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Shadow.ReleaseAndGetAddressOf());
	}

}

void ModelShader::ModelDrawShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	ShareData& pSD = ShareData::GetInstance();

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.Time = time;
	cbuff.PaintColor = color;

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	pSD.GetContext()->UpdateSubresource(m_cbuffer.Get(), 0, NULL, &cbuff, 0, 0);
	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cbuffer.Get() };
	pSD.GetContext()->PSSetConstantBuffers(2, 1, cb);

	//インプットレイアウトの登録
	pSD.GetContext()->IASetInputLayout(m_inputLayout.Get());

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// 透明判定処理
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度ステンシルステートの設定
	pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

	// カリングは左周り
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// ヴァーテックスシェーダーに適応
	pSD.GetContext()->VSSetShader(GetModelMyShader_VS().Get(), nullptr, 0);
	// ピクセルシェーダーに適応
	pSD.GetContext()->PSSetShader(GetModelMyShader_PS().Get(), nullptr, 0);

	pSD.GetContext()->PSSetShaderResources(0, 1, texture.GetAddressOf());

}

void ModelShader::SilhouetteShader()
{
	ShareData& pSD = ShareData::GetInstance();

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// 透明判定処理
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度ステンシルステートの設定(物体の背後にいても描画する処理)
	pSD.GetContext()->OMSetDepthStencilState(GetStencilShadow().Get(), 1);

	// カリングは左周り
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// シルエット描画用のピクセルシェーダーを適応
	pSD.GetContext()->PSSetShader(GetModelShadowShader().Get(), nullptr, 0);

}

void ModelShader::ToransparentShader()
{
	ShareData& pSD = ShareData::GetInstance();

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
	pSD.GetContext()->PSSetSamplers(0, 1, sampler);
	//半透明描画指定
	ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
	// 透明判定処理
	pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度ステンシルステートの設定
	pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthNone(), 1);

	// カリングは左周り
	pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

	// ピクセルシェーダーに適応
	pSD.GetContext()->PSSetShader(GetModelTransparentShader().Get(), nullptr, 0);

}
