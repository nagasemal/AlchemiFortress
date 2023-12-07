#include "pch.h"
#include "DrawArrow.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/Commons/DrawKey.h"

DrawArrow::DrawArrow(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,int direction)
{
	m_saveData.pos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;
	m_direction = direction;

	m_boxColor = { 1.0f,1.0f,1.0f,1.0f };

	m_drawKey = std::make_unique<DrawKey>(this);

}

DrawArrow::~DrawArrow()
{
}

void DrawArrow::Update()
{
	m_keySelectFlag = false;


	m_drawKey->Update();

}

void DrawArrow::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	// 画像のサイズ
	RECT srcRect = { 0, 0, 64, 64 };
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour = m_boxColor;

	if (m_hitMouseFlag) colour = { 0.9f,0.9f,0.9f,1.0f };

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };


	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 選択BOX
	pSB->Draw(pSL->GetUIArrow().Get(),
		m_data.pos,
		&srcRect,
		colour,
		XMConvertToRadians(90.0f * m_direction),
		DirectX::XMFLOAT2(64 / 2, 64 / 2),
		m_data.rage);

	pSB->End();

	m_drawKey->Draw();

}


