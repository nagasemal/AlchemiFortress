#include "pch.h"
#include "DrawLine.h"

#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

DrawLine::DrawLine(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, float lineRage)
{
	m_animationData = AnimationData();

	m_saveData.pos = m_data.pos  = pos;
	m_saveData.rage = m_data.rage = rage;
	//m_maxRage = maxRage * 2;
	m_lineRage = SimpleMath::Vector2();

	m_color = Colors::Black;

	m_lineRage.y = lineRage;

	m_rect = RECT{ 1,1,2,2};

	m_popPos	= m_data.pos + m_data.rage;
	m_ancerRect	= SimpleMath::Vector2(1,0);

}

DrawLine::~DrawLine()
{
}

void DrawLine::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

	m_popPos = m_data.pos + m_data.rage;

	// 時間計測
	if (m_hitMouseFlag)	m_animationData += deltaTime * 1.2f;
	else				m_animationData -= deltaTime * 1.2f;

	HitMouse();

	// 線を動かす
	m_lineRage.x = Easing::EaseInOutCirc(0.0f, m_data.rage.x * 2.0f, m_animationData);

	m_keySelectFlag = false;

}

void DrawLine::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	// 線を描画
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		m_popPos,
		&m_rect,
		m_color,
		0.0f,
		m_ancerRect,
		m_lineRage);

}
