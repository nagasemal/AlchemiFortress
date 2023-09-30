#include <pch.h>
#include "ShareData.h"
#include "ShareJsonData.h"

#include "NecromaLib/GameData/BinaryFile.h"

ShareData* ShareData::instance = nullptr;

ShareData::ShareData():
	m_debugFont{nullptr},
	m_commonStates(),
	m_deviceResources(),
	m_spriteBatch(),
	m_camera(),
	m_effectFactory()
{
}

void ShareData::CreateStencilData()
{

	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/ModelShadow_PS.cso");
	// ピクセルシェーダ作成
	GetDevice()->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_modelShadowShader.ReleaseAndGetAddressOf());

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
		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Base.ReleaseAndGetAddressOf());
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

		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Nomal.ReleaseAndGetAddressOf());
	}
	// 影描画
	{
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;					// 常に更新
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// 書き込まない

		desc.StencilEnable = FALSE;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// 同じならば通す

		desc.StencilReadMask = 1;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK　何もしない
		desc.BackFace = desc.FrontFace;	// 裏面も同じ

		GetDevice()->CreateDepthStencilState(&desc, m_depthStencilState_Shadow.ReleaseAndGetAddressOf());
	}
}

void ShareData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareData;
	}
}

void ShareData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}