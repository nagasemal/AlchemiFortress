#include "pch.h"
#include "Explanation.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"

Explanation::Explanation():
	m_mouseSelectButton(),
	m_prvMouseValue()
{
}

Explanation::~Explanation()
{
}

void Explanation::Update()
{


	auto mouse = InputSupport::GetInstance().GetMouseState();
		
	m_mouseSelectButton[0] = mouse.leftButton == mouse.HELD;
	m_mouseSelectButton[2] = mouse.rightButton == mouse.HELD;

	m_mouseSelectButton[1] = m_prvMouseValue != mouse.GetLastState().scrollWheelValue || 
							 mouse.middleButton == mouse.HELD;

	m_prvMouseValue = mouse.GetLastState().scrollWheelValue;
}

void Explanation::Render(bool selectNowFlag, bool rotateStopFlag)
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// É}ÉEÉXï`âÊ
	for (int i = 0; i < 3; i++)
	{

		RECT rect = SpriteCutter(296, 458, i, 0);

		pSB->Draw(pSL.GetMouseTexture().Get(), SimpleMath::Vector2(1280 / 2, 680),
			&rect,
			SimpleMath::Color(1.5f - m_mouseSelectButton[i], 1.5f - m_mouseSelectButton[i], 1.5f - m_mouseSelectButton[i], 0.8f),
			0.0f, SimpleMath::Vector2(296 / 2, 458 / 2), 0.25f);
	}

	ExplanationRender(SimpleMath::Vector2(1280 / 2 - 80,630), MACHINE_SELECT   + selectNowFlag);
	ExplanationRender(SimpleMath::Vector2(1280 / 2, 590), ZOOM			  + selectNowFlag);
	ExplanationRender(SimpleMath::Vector2(1280 / 2 + 80, 630), ROTATE_STOP	  + rotateStopFlag);

	pSB->End();

}

void Explanation::ExplanationRender(SimpleMath::Vector2 pos, int type)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	RECT rect = SpriteCutter(128, 36, type, 0);

	pSB->Draw(pSL.GetExplanationTextTexture().Get(), pos,
		&rect,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f,SimpleMath::Vector2(128 / 2,36 / 2),1.0f);

}
