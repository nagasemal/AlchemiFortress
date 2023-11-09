#include "pch.h"
#include "DrawVariableNum.h"

#include "NecromaLib/Singleton/ShareData.h"

#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/UI/Number.h"

DrawVariableNum::DrawVariableNum(SimpleMath::Vector2 position, SimpleMath::Vector2 rage)
{

	SimpleMath::Vector2 boxRage = rage;

	boxRage.x += 0.5f;

	m_box = std::make_unique<SelectionBox>(position, boxRage);

	SimpleMath::Vector2 lArrowPos = position;
	lArrowPos.x -= 80 * rage.x;

	m_leftArrow = std::make_unique<DrawArrow>(lArrowPos, rage,4);
	m_leftArrow->Initialize();


	SimpleMath::Vector2 rArrowPos = position;
	rArrowPos.x += 80 * rage.x;

	m_rightArrow = std::make_unique<DrawArrow>(rArrowPos, rage,2);
	m_rightArrow->Initialize();

	m_number = std::make_unique<Number>(position, rage / 1.75f);
	m_number->SetNumber(0);
}

DrawVariableNum::~DrawVariableNum()
{

}

void DrawVariableNum::Update(int minval, int maxVal)
{

	m_box->HitMouse();

	m_leftArrow->HitMouse();
	m_rightArrow->HitMouse();

	m_number->SetNumber(m_number->GetNumber() - m_leftArrow->ClickMouse());
	m_number->SetNumber(m_number->GetNumber() + m_rightArrow->ClickMouse() + m_box->ClickMouse());

	m_number->SetNumber(std::min(std::max(minval, m_number->GetNumber()), maxVal));

}

void DrawVariableNum::Render()
{
	m_box->Draw();

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	m_leftArrow->Draw();
	m_rightArrow->Draw();
	pSB->End();

	m_number->Render();

}

int DrawVariableNum::GetNumber()
{
	return m_number->GetNumber();
}

void DrawVariableNum::SetNumber(int number)
{

	m_number->SetNumber(number);

}
