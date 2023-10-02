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
	//　初期化　　オブジェクト配置時は当たり判定を大きくする
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

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(POINTER_RAGE);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos);

	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			//画像用サンプラーの登録
			ID3D11SamplerState* sampler[1] = { pSD.GetCommonStates()->LinearWrap() };
			pSD.GetContext()->PSSetSamplers(0, 1, sampler);
			//半透明描画指定
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// 透明判定処理
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

			// 深度ステンシルステートの設定
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthNone(), 0);

			// カリングは左周り
			pSD.GetContext()->RSSetState(pSD.GetCommonStates()->CullNone());

			// ピクセルシェーダーに適応
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
