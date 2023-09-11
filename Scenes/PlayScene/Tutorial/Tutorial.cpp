#include "pch.h"
#include "Tutorial.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/PopLine.h"

// UIを指すにあたり必要となるクラス
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/UI/Gauge.h"

#define CANCEL_BUTTON SimpleMath::Vector2(1220.0f,64.0f)

#define TUTORIAL_TEXT_POS SimpleMath::Vector2(200.0f,200.0f)

#define MACHINE_EXPLANATION 5

// 矢印のX座標指定
#define ARROW_POS_X 350
// 示すラインの大きさ
#define LINE_RAGE SimpleMath::Vector2(50.0f,50.0f)

const std::vector<const wchar_t*> Tutorial::FILENAME =
{
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/Attacker.png"},
	{L"Resources/Textures/Explanation/Defencer.png"},
	{L"Resources/Textures/Explanation/Upper.png"},
	{L"Resources/Textures/Explanation/Recovery.png"},
	{L"Resources/Textures/Explanation/Excavator.png"},
	{L"Resources/Textures/Explanation/Instructions_1.png"},
	{L"Resources/Textures/Explanation/Instructions_2.png"},
	{L"Resources/Textures/Explanation/Instructions_3.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
};

Tutorial::Tutorial():
	m_tutorialFlag(false),
	m_selectNumber(1),
	m_doubleSpeedNum()
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

	m_arrowL = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) + ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) - ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),4);

	m_cancelButton = std::make_unique<SelectionBox>(CANCEL_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));

	m_doubleSpeedButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(width - 100.0f, height - 100.0f), SimpleMath::Vector2(1.0f, 1.0f));

	m_showLine = std::make_unique<PopLine>(SimpleMath::Vector2(), LINE_RAGE, LINE_RAGE,15.0f);
	m_showLine->SetColor(SimpleMath::Color(1.0f,0.6f,0.6f,1.0f));
	CreateInterfase();
}

void Tutorial::Update(AlchemicalMachineManager* machineManager, Gauge* gauge)
{
	DeltaTime& pDltaT = DeltaTime::GetInstance();

	m_showLine->Update();
	m_showLine->SetHitMouseFlag(m_tutorialFlag);

	if (m_tutorialFlag)
	{
		m_arrowL->HitMouse();
		m_arrowR->HitMouse();


		// 上限下限設定
		m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)SpriteLoder::INSTRUCTION_TYPE::NUM);

		// 左ボタンでm_selectNumber増加
		if (m_arrowL->ClickMouse() && m_selectNumber < SpriteLoder::INSTRUCTION_TYPE::NUM)
		{
			m_selectNumber++;
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// アニメーション値を0に戻す
			m_showLine->ResetAnimationData();
		}

		// 右ボタンでm_selectNumber増加
		if (m_arrowR->ClickMouse() && m_selectNumber > 0)
		{
			m_selectNumber--;
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// アニメーション値を0に戻す
			m_showLine->ResetAnimationData();
		}

		// 線を引く位置を決める
		LinePosSet(machineManager, gauge);

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
		// 背景黒くする
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		RECT rect = { 0,0,800,600};

		pSB->Draw(pSL.GetMagicRule().Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);

		pSD.GetSpriteBatch()->End();

	}

	m_cancelButton->DrawUI(pSL.GetUIIcons().Get(), SpriteCutter(64, 64, SpriteLoder::UIICON_TYPE::EXPRANATION, 0), { 0.0f,0.0f,0.0f,1.0f });
	m_doubleSpeedButton->DrawUI();
}

void Tutorial::Render_Layer2()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	if (m_tutorialFlag)
	{
		// テキスト描画
		m_backFlame->Render();
		m_textTexture->Render();

		// 示す先の線と矢印UI描画
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		// 
		m_showLine->Draw();

		//
		if (m_selectNumber < SpriteLoder::INSTRUCTION_TYPE::NUM)m_arrowL->Draw();
		if (m_selectNumber > 0)m_arrowR->Draw();

		pSD.GetSpriteBatch()->End();
	}
}

void Tutorial::Finalize()
{
}

void Tutorial::CreateInterfase()
{
	ShareData& pSD = ShareData::GetInstance();
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	SimpleMath::Vector2 pos{ width / 4.0f + TUTORIAL_TEXT_POS.x,100.0f + TUTORIAL_TEXT_POS.y};
	SimpleMath::Vector2 rage{ 0.5f,0.5f};

	m_textTexture = std::make_unique<UserInterface>();
	m_backFlame = std::make_unique<UserInterface>();

	m_textTexture->Create(device, L"Resources/Textures/Explanation/Attacker.png", pos, rage,UserInterface::TOP_LEFT);
	m_textTexture->SetWindowSize((const int)width,(const int)height);

	pos = { width / 2 + TUTORIAL_TEXT_POS.x,height / 3 + TUTORIAL_TEXT_POS.y};
	rage = { 0.75f,0.6f };

	m_backFlame->Create(device, L"Resources/Textures/Explanation/BackFlame.png", pos, rage, UserInterface::MIDDLE_CENTER);
	m_backFlame->SetWindowSize((const int)width, (const int)height);
	m_backFlame->SetColor(SimpleMath::Color(0.8f, 0.8f, 1.0f, 0.95f));

	pos = { width / 2.0f,height / 2.0f};
	rage = { 100.0f,100.0f };

}

void Tutorial::LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge)
{

	SimpleMath::Vector2 linePos = SimpleMath::Vector2();
	SimpleMath::Vector2 lineRage = LINE_RAGE;

	MachineSelect* machineSelect = machineManager->GetMachineSelect()->get()->GetMachineSelect(MACHINE_TYPE::ATTACKER);
	machineSelect->SetHitMouseFlag(false);
	machineManager->SetSelectMachineNumber(-1);

	// 何を指しているのか分かるようにする
	// :マシン範囲
	if (m_selectNumber <= MACHINE_EXPLANATION && m_selectNumber >= 1)
	{
		linePos = machineManager->GetMachineSelect()->get()->GetMachineSelect(m_selectNumber)->GetData().pos;
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::OPERATION)
	{
		linePos  = SimpleMath::Vector2(892.0f,64.0f);
		lineRage = SimpleMath::Vector2(300.0f, 50.0f);
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::OPERATION_MACHINE)
	{
		machineSelect->SetHitMouseFlag(true);
		linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(85.0f, 40.0f);
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::MACHINE_UI)
	{

		machineManager->SetSelectMachineNumber(0);

		linePos = SimpleMath::Vector2();
		lineRage = SimpleMath::Vector2();
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::GAUGE_HP)
	{
		linePos = gauge->GetHPGaugePos();
		lineRage = SimpleMath::Vector2(230.0f, 20.0f);
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::GAUGE_MP)
	{
		linePos = gauge->GetMPGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 15.0f);
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::GAUGE_CRYSTAL)
	{
		linePos = gauge->GetCrystalGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 15.0f);
	}

	if (m_selectNumber == SpriteLoder::INSTRUCTION_TYPE::ALCHEMI)
	{
	}
	m_showLine->SetPos(linePos);
	m_showLine->SetRage(lineRage);
	m_showLine->SetMaxRage(lineRage);

}

