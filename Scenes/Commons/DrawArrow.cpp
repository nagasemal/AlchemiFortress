#include "pch.h"
#include "DrawArrow.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

DrawArrow::DrawArrow()
{
}

DrawArrow::~DrawArrow()
{
}

void DrawArrow::Initialize()
{
}

void DrawArrow::Update()
{
}

void DrawArrow::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	auto pSL = &SpriteLoder::GetInstance();

	// 画像のサイズ
	RECT srcRect = { 0, 0, 64, 64 };

	// 選択BOX
	pSB->Draw(pSL->GetUIArrow().Get(), m_data.pos, &srcRect, Colors::White, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), m_data.rage);

}

void DrawArrow::Finalize()
{
}
