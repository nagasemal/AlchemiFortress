#include "pch.h"
#include "Tutorial.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"

#define CANCEL_BUTTON DirectX::SimpleMath::Vector2(1220.0f,64.0f)

#define TUTORIAL_TEXT_POS DirectX::SimpleMath::Vector2(200.0f,200.0f)


const std::vector<const wchar_t*> Tutorial::FILENAME =
{
	{L"Resources/Textures/Explanation/Attacker.png"},
	{L"Resources/Textures/Explanation/Attacker.png"},
	{L"Resources/Textures/Explanation/Defencer.png"},
	{L"Resources/Textures/Explanation/Upper.png"},
	{L"Resources/Textures/Explanation/Recovery.png"},
	{L"Resources/Textures/Explanation/Excavator.png"},
	{L"Resources/Textures/Explanation/Attacker.png"},
	{L"Resources/Textures/Explanation/Attacker.png"},
};

Tutorial::Tutorial():
	m_tutorialFlag(false),
	m_selectNumber(0)
{
}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize()
{

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	m_arrowL = std::make_unique<DrawArrow>(DirectX::SimpleMath::Vector2((width / 2) + 300.0f + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), DirectX::SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(DirectX::SimpleMath::Vector2((width / 2) - 300.0f + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), DirectX::SimpleMath::Vector2(1.0f, 1.0f),4);

	m_cancelButton = std::make_unique<SelectionBox>(CANCEL_BUTTON, DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	m_doubleSpeedButton = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(width - 100.0f, height - 100.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	CreateInterfase();
}

void Tutorial::Update()
{
	DeltaTime& pDltaT = DeltaTime::GetInstance();

	pDltaT.SetStopFlag(m_tutorialFlag);

	if (m_tutorialFlag)
	{
		m_arrowL->HitMouse();
		m_arrowR->HitMouse();

		// 左右ボタンで値減増
		if (m_arrowL->ClickMouse() && m_selectNumber < SpriteLoder::INSTRUCTION_TYPE::NUM)
		{
			m_selectNumber++;
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);
		}

		if (m_arrowR->ClickMouse() && m_selectNumber > 0)
		{
			m_selectNumber--;
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);
		}


		// 上限下限設定
		m_selectNumber = std::min(std::max(m_selectNumber, 0),(const int)SpriteLoder::INSTRUCTION_TYPE::NUM);
	}

	m_cancelButton->HitMouse();

	if (m_cancelButton->ClickMouse())
	{
		m_tutorialFlag = !m_tutorialFlag;
	}

}

void Tutorial::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	if (m_tutorialFlag)
	{
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		RECT rect = { 0,0,800,600};

		pSB->Draw(pSL.GetMagicRule().Get(), DirectX::SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  DirectX::SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);

		pSD.GetSpriteBatch()->End();

		m_backFlame->Render();
		m_textTexture->Render();

		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		if(m_selectNumber < SpriteLoder::INSTRUCTION_TYPE::NUM)m_arrowL->Draw();
		if(m_selectNumber > 0)m_arrowR->Draw();

		pSD.GetSpriteBatch()->End();

	}

	m_cancelButton->DrawUI(pSL.GetUIIcons().Get(), SpriteCutter(64, 64, SpriteLoder::UIICON_TYPE::EXPRANATION, 0), {0.0f,0.0f,0.0f,1.0f});
	m_doubleSpeedButton->DrawUI();
}

void Tutorial::Finalize()
{
}

void Tutorial::CreateInterfase()
{
	ShareData& pSD = ShareData::GetInstance();
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	DirectX::SimpleMath::Vector2 pos{ width / 4 + TUTORIAL_TEXT_POS.x,100 + TUTORIAL_TEXT_POS.y};
	DirectX::SimpleMath::Vector2 rage{ 0.5f,0.5f};

	m_textTexture = std::make_unique<UserInterface>();
	m_backFlame = std::make_unique<UserInterface>();
	m_blackFilm = std::make_unique<UserInterface>();

	m_textTexture->Create(device, L"Resources/Textures/Explanation/Attacker.png", pos, rage,UserInterface::TOP_LEFT);
	m_textTexture->SetWindowSize(width,height);

	pos = { width / 2 + TUTORIAL_TEXT_POS.x,height / 3 + TUTORIAL_TEXT_POS.y};
	rage = { 0.75f,0.6f };

	m_backFlame->Create(device, L"Resources/Textures/Explanation/BackFlame.png", pos, rage, UserInterface::MIDDLE_CENTER);
	m_backFlame->SetWindowSize(width, height);
	m_backFlame->SetColor(DirectX::SimpleMath::Color(0.8f, 0.8f, 1.0f, 0.95f));

	pos = { width / 2,height / 2};
	rage = { 100.0f,100.0f };

	//m_blackFilm->Create(device, L"Resources/Textures/Fade.png", pos, rage, UserInterface::MIDDLE_CENTER);
	//m_blackFilm->SetWindowSize(width, height);
	//m_blackFilm->SetColor(DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.65f));

	//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::NONE]);
//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::ATTACKER]);
//LoadingPngFile(L"Resources/Textures/Explanation/Defencer.png", m_explanationTexture[INSTRUCTION_TYPE::DEFENSER]);
//LoadingPngFile(L"Resources/Textures/Explanation/Upper.png", m_explanationTexture[INSTRUCTION_TYPE::UPPER]);
//LoadingPngFile(L"Resources/Textures/Explanation/Recovery.png", m_explanationTexture[INSTRUCTION_TYPE::RECOVERY]);
//LoadingPngFile(L"Resources/Textures/Explanation/Excavator.png", m_explanationTexture[INSTRUCTION_TYPE::MINING]);
//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::OPERATION]);

}
