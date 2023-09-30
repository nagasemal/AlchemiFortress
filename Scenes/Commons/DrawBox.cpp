#include "pch.h"
#include "DrawBox.h"

#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

DrawBox::DrawBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, float lineRage)
{

	m_animationData_First = AnimationData(0.0f,0.0f,1.0f,0.8f);
	m_lineRage_Vertical = m_lineRage_Beside = SimpleMath::Vector2();

	m_animationData_Second = AnimationData();
	m_rotate_easing = 0.0f;

	m_data.pos = pos;
	m_data.rage = rage;

	m_color = Colors::Black;

	m_lineRage_Vertical.x = m_lineRage_Beside.y = lineRage;

	m_rect = RECT{ 1,1,2,2 };

}

DrawBox::~DrawBox()
{
}

void DrawBox::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// ŽžŠÔŒv‘ª
	if (m_hitMouseFlag)	m_animationData_First += deltaTime * 1.1f;
	else				m_animationData_First -= deltaTime * 1.1f;

	HitMouse();

	// ü‚ð“®‚©‚·
	m_lineRage_Vertical.y = Easing::EaseInOutCirc(0.0f	, m_data.rage.y * 2.0f, m_animationData_First);
	m_lineRage_Beside.x	  = Easing::EaseInOutCirc(0.0f  , m_data.rage.x * 2.0f, m_animationData_First);

	m_keySelectFlag = false;

}

void DrawBox::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	SimpleMath::Vector2 popPos = SimpleMath::Vector2();
	RECT rect = RECT();
		
	// ‰Ecü
	popPos = SimpleMath::Vector2(m_data.pos.x + m_data.rage.x, m_data.pos.y + m_data.rage.y);
	rect = RECT{ 2,2,1,1 };
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		popPos,
		&rect,
		m_color,
		0.0f,
		SimpleMath::Vector2(0, 0),
		m_lineRage_Vertical);

	// ¶cü
	popPos = SimpleMath::Vector2(m_data.pos.x - m_data.rage.x, m_data.pos.y - m_data.rage.y);
	rect = RECT{ 1,1,2,2 };
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		popPos,
		&rect,
		m_color,
		0.0f,
		SimpleMath::Vector2(0, 0),
		m_lineRage_Vertical);

	// ã‰¡ü
	popPos = SimpleMath::Vector2(m_data.pos.x + m_data.rage.x, m_data.pos.y - m_data.rage.y);
	rect = RECT{ 1,1,2,2 };
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		popPos,
		&rect,
		m_color,
		0.0f,
		SimpleMath::Vector2(1, 0),
		m_lineRage_Beside);


	// ‰º‰¡ü
	popPos = SimpleMath::Vector2(m_data.pos.x - m_data.rage.x, m_data.pos.y + m_data.rage.y);
	rect = RECT{ 1,1,2,2 };
	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		popPos,
		&rect,
		m_color,
		0.0f,
		SimpleMath::Vector2(0, 0),
		m_lineRage_Beside);

}
