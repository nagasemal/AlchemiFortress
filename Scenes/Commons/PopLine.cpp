#include "pch.h"
#include "PopLine.h"

#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

PopLine::PopLine(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, SimpleMath::Vector2 maxRage, float lineRage)
{
	m_animationData = AnimationData();

	m_data.pos  = pos;
	m_data.rage = rage;
	m_maxRage = maxRage * 2;
	m_lineRage = SimpleMath::Vector2();

	m_color = Colors::Black;

	m_lineRage.y = lineRage;

	m_rect = RECT{ 1,1,2,2};

	m_popPos	= m_data.pos + maxRage;
	m_ancerRect	= SimpleMath::Vector2(1,0);

}

PopLine::~PopLine()
{
}

void PopLine::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_popPos = m_data.pos + (m_maxRage / 2);

	// 時間計測
	if (m_hitMouseFlag)	m_animationData += deltaTime * 1.2f;
	else				m_animationData -= deltaTime * 1.2f;

	HitMouse();

	// 線を動かす
	m_lineRage.x = Easing::EaseInOutCirc(0.0f, m_maxRage.x, m_animationData);

	auto context = ShareData::GetInstance().GetContext();
	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPosition>>(context);

	m_keySelectFlag = false;

}

void PopLine::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	// 魔法陣描画
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		m_popPos,
		&m_rect,
		m_color,
		0.0f,
		m_ancerRect,
		m_lineRage);

}
