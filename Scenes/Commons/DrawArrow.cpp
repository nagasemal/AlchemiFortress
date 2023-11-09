#include "pch.h"
#include "DrawArrow.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

DrawArrow::DrawArrow(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,int direction)
{
	m_saveData.pos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;
	m_direction = direction;

	m_boxColor = { 1.0f,1.0f,1.0f,1.0f };

}

DrawArrow::~DrawArrow()
{
}

void DrawArrow::Update()
{
	m_keySelectFlag = false;
}

void DrawArrow::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	auto pSL = &SpriteLoder::GetInstance();

	// 画像のサイズ
	RECT srcRect = { 0, 0, 64, 64 };
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour = m_boxColor;

	if (m_hitMouseFlag) colour = { 0.9f,0.9f,0.9f,1.0f };

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };

	// 選択BOX
	pSB->Draw(pSL->GetUIArrow().Get(),
		m_data.pos,
		&srcRect,
		colour,
		XMConvertToRadians(90.0f * m_direction),
		DirectX::XMFLOAT2(64 / 2, 64 / 2),
		m_data.rage);
}


