#include "pch.h"
#include "ResultUIManager.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define UI_POS		SimpleMath::Vector2{1280.0f / 2.0f,720.0f / 2.0f}

#define UI_RAGE 	SimpleMath::Vector2{1.5f, 1.0f}

ResultUIManager::ResultUIManager()
{

	m_selectionBox_Next = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x - 200, UI_POS.y }, UI_RAGE);

	m_selectionBox_Retry = std::make_unique<SelectionBox>(UI_POS,UI_RAGE);

	m_selectionBox_Back = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x + 200, UI_POS.y }, UI_RAGE);

}

ResultUIManager::~ResultUIManager()
{
}

void ResultUIManager::Update()
{

	m_selectionBox_Next->HitMouse();
	m_selectionBox_Back->HitMouse();
	m_selectionBox_Retry->HitMouse();

}

void ResultUIManager::Render()
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	m_selectionBox_Next->DrawUI(pSL.GetResultTextTexture().Get(),SpriteCutter(128,28,0,0),SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));
	m_selectionBox_Back->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(128, 28, 1, 0), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	m_selectionBox_Retry->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(128, 28, 2, 0), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

}
