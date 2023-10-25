#include "pch.h"
#include "Tutorial.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

#include "NecromaLib/GameData/ScreenToWorld.h"

// UIを指すにあたり必要となるクラス
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/PlayScene/AlchemicalMachine/AM_Attacker.h"
#include "Scenes/SelectScene/MissionRender.h"
#include "Scenes/PlayScene/Shadow/Particle.h"

#define EXPLANATION_BUTTON		SimpleMath::Vector2(1220.0f,64.0f)
#define TUTORIAL_EXIT_BUTTON	SimpleMath::Vector2(1180.0f,380.0f)

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
	{L"Resources/Textures/Explanation/Gauge_1.png"},
	{L"Resources/Textures/Explanation/Gauge_2.png"},
	{L"Resources/Textures/Explanation/Gauge_3.png"},
	{L"Resources/Textures/Explanation/Mission.png"},
	{L"Resources/Textures/Explanation/Instructions_4.png"},
	{L"Resources/Textures/Explanation/Instructions_5.png"},
	{L"Resources/Textures/Explanation/Instructions_6.png"},
	{L"Resources/Textures/Explanation/Instructions_7.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
};

Tutorial::Tutorial():
	m_explanationFlag(false),
	m_tutorialFlag(false),
	m_cameraFlag(false),
	m_selectNumber(1),
	m_tutorialNumber(0),
	m_maxSelectVal(),
	m_tutorialTime(0)
{

}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(std::vector<int> tutorialNumber, PlayScene* pPlayScene)
{

	m_showBox = std::make_unique<DrawBox>(SimpleMath::Vector2(), LINE_RAGE, 5.0f);
	m_showBox->SetColor(SimpleMath::Color(1.0f, 1.0f, 0.0f, 1.0f));

	// 位置を読み込む
	RelodeTutorial(tutorialNumber,pPlayScene);

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// 取得
	m_arrowL = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) + ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) - ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),4);

	m_explanationButton  = std::make_unique<SelectionBox>(EXPLANATION_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));
	m_tutorialExitButton = std::make_unique<SelectionBox>(TUTORIAL_EXIT_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));

	// チュートリアル表示用 マシンUI
	m_amMachine = std::make_unique<AM_Attacker>();
	m_amMachine -> Initialize();

	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	CreateInterfase();

	// 説明画像テクスチャの読み込み
	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[0]]);

	// 対象オブジェクトを指すパーティクルクラス
	m_particle_Select = std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Select->Initialize();
	m_particle_Select->SetParticleSpawnTime(1.0f);

	// チュートリアル数を取得
	m_selectNumber = 0;
	m_maxSelectVal = (const int)m_tutorialNumber.size();
	LinePosSet(pPlayScene, m_tutorialNumber[m_selectNumber]);

}

void Tutorial::Update(PlayScene* pPlayScene, bool stopFlag)
{

	m_cameraFlag = stopFlag;

	m_showBox->Update();
	m_showBox->SetHitMouseFlag( GetTutorialFlag() );

	// パーティクルのアップデート処理
	m_particle_Select->UpdateParticle();


	if(!m_cameraFlag) m_showBox->ResetAnimationData();

	// 説明時モード
	if (m_explanationFlag)
	{
		m_arrowL->HitMouse();
		m_arrowR->HitMouse();

		m_maxSelectVal = INSTRUCTION_TYPE::NUM;

		// 左ボタンでm_selectNumber増加
		if (m_arrowL->ClickMouse())
		{
			m_selectNumber++;
			// 上限下限設定
			m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// 線を引く位置を決める
			LinePosSet(pPlayScene,m_selectNumber);

			// アニメーション値を0に戻す
			m_showBox->ResetAnimationData();
		}

		// 右ボタンでm_selectNumber増加
		if (m_arrowR->ClickMouse())
		{

			m_selectNumber--;
			// 上限下限設定
			m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// 線を引く位置を決める
			LinePosSet(pPlayScene,m_selectNumber);

			// アニメーション値を0に戻す
			m_showBox->ResetAnimationData();
		}

	}

	// チュートリアル時処理
	if (m_tutorialFlag && m_cameraFlag)
	{		
		InputSupport& pINP = InputSupport::GetInstance();

		// 離された瞬間を取得する説明用変数
		bool pushFlag = pINP.GetMouseState().leftButton == Mouse::ButtonStateTracker::RELEASED;

		if (pushFlag)
		{
			// クリックされたら次のチュートリアルに向かう
			m_selectNumber++;
			// 上限設定
			m_selectNumber = std::min(std::max(m_selectNumber, 0), m_maxSelectVal);

			LinePosSet(pPlayScene, m_tutorialNumber[m_selectNumber]);
		}

		// 全チュートリアルが終わったらチュートリアルモードを解除する
		if (m_selectNumber >= m_maxSelectVal)
		{

			m_tutorialFlag = false;
			m_selectNumber = 0;

		}

		//m_arrowL->HitMouse();
		//m_arrowR->HitMouse();
		//
		//// 選択可能範囲の最大値を取得
		//m_maxSelectVal = (const int)m_tutorialNumber.size() - 1;
		//
		//// 左ボタンでm_selectNumber増加
		//if (m_arrowL->ClickMouse())
		//{
		//	m_selectNumber++;
		//	// 上限下限設定
		//	m_selectNumber = std::min(std::max(m_selectNumber, 0), m_maxSelectVal);
		//
		//	// テクスチャを読み込む
		//	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[m_selectNumber]]);
		//
		//	// アニメーション値を0に戻す
		//	m_showBox->ResetAnimationData();
		//}
		//
		//// 右ボタンでm_selectNumber増加
		//if (m_arrowR->ClickMouse())
		//{
		//
		//	m_selectNumber--;
		//	// 上限下限設定
		//	m_selectNumber = std::min(std::max(m_selectNumber, 0), m_maxSelectVal);
		//
		//	// テクスチャを読み込む
		//	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[m_selectNumber]]);
		//
		//	// アニメーション値を0に戻す
		//	m_showBox->ResetAnimationData();
		//}
		//
		//// ラインを引く位置を決める
		//LinePosSet(pPlayScene,m_selectNumber);
		//
		//if (m_selectNumber >= m_maxSelectVal)
		//{
		//	m_tutorialExitButton->HitMouse();
		//	if (m_tutorialExitButton->ClickMouse()) 			m_tutorialFlag = false;
		//}

	}

	// 説明モード移行ボタン
	m_explanationButton->HitMouse();
	if (m_explanationButton->ClickMouse())
	{
		m_explanationFlag = !m_explanationFlag;

		// 線を引く位置を決める
		LinePosSet(pPlayScene,m_selectNumber);
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

	m_particle_Select->Render();

	if (m_explanationFlag)
	{
		// 背景黒くする
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		RECT rect = { 0,0,800,600};
	
		// 背景を黒くする
		pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);
	
		rect = SpriteCutter(64, 64, SpriteLoder::UIICON_TYPE::STOP, 0);
	
		// 背景を黒くする
		pSB->Draw(pSL.GetUIIcons().Get(), SimpleMath::Vector2(width / 2.0f, height / 2.0f),
			&rect,
			SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.8f),
			0.0f,
			DirectX::XMFLOAT2{ 64.0f / 2.0f,64.0f / 2.0f }, 10.0f);
	
		pSD.GetSpriteBatch()->End();
		
		// マシンUIの説明の際に描画
		if (m_selectNumber == MACHINE_UI)
		{
			m_machineExplanation->Draw();
			m_amMachine->SetSelectModeFlag(true);
			m_amMachine->SelectRenderUI_Common();
			m_amMachine->RenderUI(pSL.GetSelectBoxTexture());
		}

	}
	
	m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::EXPRANATION);

}

void Tutorial::Render_Layer2()
{
	ShareData& pSD = ShareData::GetInstance();
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	m_showBox->Draw();
	pSD.GetSpriteBatch()->End();
	
	if ( m_explanationFlag )
	{
		// テキスト描画
		m_backFlame->Render();
		m_textTexture->Render();
	
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		// 示す先の線と矢印UI描画
		// 対象を明確にする
		m_showBox->Draw();
	
		// 選択移動矢印の描画　(上限下限に達したら描画を切る)
		if (m_selectNumber < m_maxSelectVal)	m_arrowL->Draw();
		if (m_selectNumber > 0)					m_arrowR->Draw();
	
		pSD.GetSpriteBatch()->End();
	
		// チュートリアル時に描画する
		if ( m_tutorialFlag && m_selectNumber >= (const int)m_tutorialNumber.size() - 1)
		{
			m_tutorialExitButton->DrawUI(SpriteLoder::UIICON_TYPE::CANCEL);
		}
	
	}

}

void Tutorial::Finalize()
{
}

void Tutorial::RelodeTutorial(std::vector<int> tutorialNumber, PlayScene* pPlayScene)
{
	m_selectNumber = 0;
	m_tutorialFlag = (bool)tutorialNumber[0];
	m_tutorialNumber = tutorialNumber;

	// 選択可能範囲の最大値を取得
	m_maxSelectVal = (const int)m_tutorialNumber.size();

	// UserInterFaceが生成されているならば再度画像を読み込む
	if(m_textTexture) m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[0]]);

	// 線を引く位置を決める
	LinePosSet(pPlayScene, m_tutorialNumber[0]);

}

void Tutorial::CreateInterfase()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	// テクスチャを描画する際の位置情報と大きさ
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

void Tutorial::LinePosSet(PlayScene* pPlayScene, int number)
{

	SimpleMath::Vector2 linePos = SimpleMath::Vector2(-100,-100);
	SimpleMath::Vector2 lineRage = LINE_RAGE;

	// マシンマネージャークラスを得る
	AlchemicalMachineManager* machineManager = pPlayScene->GetAlchemicalMachineManager();
	// マシンセレクトクラスを得る
	MachineSelect* machineSelect = machineManager->GetMachineSelect()->get()->GetMachineSelect(MACHINE_TYPE::ATTACKER);
	machineSelect->SetHitMouseFlag(false);
	// ゲージマネージャークラスを得る
	Gauge* gauge = pPlayScene->GetGauge();
	// ミッションマネージャークラスを得る
	MissionRender* missionRender = pPlayScene->GetMissionManager()->GetMissionRender()->get();
	// 操作補助クラスを得る
	Explanation* explanation = pPlayScene->GetExplanation();

	//// チュートリアル画面時は常に未選択とする。
	//machineManager->SetSelectMachineNumber(-1);

	// ボタンを押した1Fの間にのみ反応する為、ここでは不問とする
	switch ((INSTRUCTION_TYPE)number)
	{
	case INSTRUCTION_TYPE::NONE:
		break;

	case INSTRUCTION_TYPE::ATTACKER:
	case INSTRUCTION_TYPE::DEFENSER:
	case INSTRUCTION_TYPE::UPPER:
	case INSTRUCTION_TYPE::RECOVERY:
	case INSTRUCTION_TYPE::MINING:
	{

		linePos = machineManager->GetMachineSelect()->get()->GetMachineSelect(number)->GetData().pos;
		lineRage = LINE_RAGE;

		break;
	}
	case INSTRUCTION_TYPE::OPERATION:
	{
		linePos = SimpleMath::Vector2(892.0f, 64.0f);
		lineRage = SimpleMath::Vector2(300.0f, 50.0f);

		break;
	}
	case INSTRUCTION_TYPE::OPERATION_MACHINE:
	{
		linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);

		break;
	}
	case INSTRUCTION_TYPE::MACHINE_UI:
	{
		m_amMachine->SetSelectModeFlag(true);
		linePos = m_machineExplanation->GetPos();
		lineRage = m_machineExplanation->GetRage();
		break;
	}
	case INSTRUCTION_TYPE::GAUGE_HP:
	{
		linePos = gauge->GetHPGaugePos();
		lineRage = SimpleMath::Vector2(240.0f, 19.0f);

		break;
	}
	case INSTRUCTION_TYPE::GAUGE_MP:
	{
		linePos = gauge->GetMPGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 14.0f);

		break;
	}
	case INSTRUCTION_TYPE::GAUGE_CRYSTAL:
	{
		linePos = gauge->GetCrystalGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 14.0f);

		break;
	}
	case INSTRUCTION_TYPE::MISSION:
	{

		linePos = missionRender->GetPos();
		lineRage = missionRender->GetRage();

		break;
	}
	case INSTRUCTION_TYPE::ALCHEMI:
	{

		linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);

		break;
	}
	case INSTRUCTION_TYPE::SPAWN:
	{

		break;
	}
	case INSTRUCTION_TYPE::ROTATE_STOP:
	{
		linePos = explanation->RightExplanationPos();
		lineRage = SimpleMath::Vector2(50.0f, 35.0f);
		break;
	}
	case INSTRUCTION_TYPE::LINE_SELECT:
	{
		linePos = explanation->MiddleExplanationPos();
		lineRage = SimpleMath::Vector2(50.0f, 35.0f);
		break;
	}
	case INSTRUCTION_TYPE::CAMERA_MOVE:
	{
		linePos = explanation->MiddleExplanationPos();
		lineRage = SimpleMath::Vector2(50.0f, 35.0f);
		break;
	}
	case INSTRUCTION_TYPE::TUTORIAL:
	{
		linePos = EXPLANATION_BUTTON;
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);
		break;
	}

	case 99:

		// NoneMachineの位置を探し出し、エフェクトを発生させます
		NonePosSearch(pPlayScene);

		break;

	default:
		break;
	}

	m_showBox->ResetAnimationData();
	m_showBox->SetPos(linePos);
	m_showBox->SetRage(lineRage);

}

void Tutorial::NonePosSearch(PlayScene* pPlayScene)
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);
	SimpleMath::Vector3 none_pos;

	for (const auto& none : pPlayScene->GetAlchemicalMachineManager()->GetAlchemicalMachineObject())
	{
		if (none->GetModelID() == MACHINE_TYPE::NONE)
		{
			// 早期リターン
			if (!none->GetActive()) continue;

			m_particle_Select->OnShot(none->GetPos(),true,SimpleMath::Color(1.0f,1.0,0.0f,0.6f));
		}
	}

}

