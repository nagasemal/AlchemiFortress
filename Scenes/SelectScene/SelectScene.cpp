#include "pch.h"
#include "SelectScene.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/DataManager.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"

#define MAX_STAGE 4
#define MIN_STAGE 1

#define MISSION_POS DirectX::SimpleMath::Vector2{545,720 / 2}
#define NEXTBOTTOM_POS DirectX::SimpleMath::Vector2{1280.0f / 2.0f,720.0f / 1.15f}
#define ARROW_POS_L DirectX::SimpleMath::Vector2{200,500}
#define ARROW_POS_R DirectX::SimpleMath::Vector2{1080,500}

SelectScene::SelectScene():
	m_selectStageNumber(1),
	m_changeMachine(true)
{

}

SelectScene::~SelectScene()
{
}

void SelectScene::Initialize()
{

	ShareJsonData::GetInstance().LoadingJsonFile_ClearData(m_selectStageNumber);
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_selectStageNumber);

	m_selectCamera = std::make_unique<SelectCamera>();
	m_selectCamera->Initialize();

	m_machineDraw = std::make_unique<DrawMachine>();
	m_machineDraw->Initialize(m_selectStageNumber);

	m_missionDraw = std::make_unique<MissionRender>(MISSION_POS, DirectX::SimpleMath::Vector2{1,1});

	m_arrowDraw[0] = std::make_unique<DrawArrow>(ARROW_POS_L, DirectX::SimpleMath::Vector2{1,1},0);
	m_arrowDraw[1] = std::make_unique<DrawArrow>(ARROW_POS_R, DirectX::SimpleMath::Vector2{1,1},2);

	m_nextSceneBox = std::make_unique<SelectionBox>(NEXTBOTTOM_POS, DirectX::SimpleMath::Vector2{5,1});

	m_uiKeyControl = std::make_unique<UIKeyControl>();
	m_uiKeyControl->AddUI(m_arrowDraw[0].get(), 0, 0);
	m_uiKeyControl->AddUI(m_arrowDraw[1].get(), 1, 0);
	m_uiKeyControl->AddUI(m_nextSceneBox.get(), 0, 1);

}

GAME_SCENE SelectScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();

	m_selectCamera->Update();
	m_machineDraw->Update();

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_selectCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_selectCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_selectCamera->GetEyePosition());
 
	m_arrowDraw[0]->HitMouse();
	if (m_arrowDraw[0]->ClickMouse() && m_selectStageNumber > 1)
	{
		m_selectStageNumber--;
		m_changeMachine = false;
		m_selectCamera->AnimationReset();
	}


	m_arrowDraw[1]->HitMouse();
	if (m_arrowDraw[1]->ClickMouse() && m_selectStageNumber < MAX_STAGE)
	{
		m_selectStageNumber++;
		m_changeMachine = false;
		m_selectCamera->AnimationReset();
	}

	if (m_selectCamera->GetAnimationTimer() >= 0.35f && !m_changeMachine)
	{
		// 読み込み
		ShareJsonData::GetInstance().LoadingJsonFile_ClearData(m_selectStageNumber);
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_selectStageNumber);

		m_machineDraw->Initialize(m_selectStageNumber);
		m_changeMachine = true;
	}

	// Update処理;
	m_nextSceneBox->HitMouse();

	m_uiKeyControl->Update();

	//　プレイシーンに遷移 (ボタンを押した時)
	if (m_nextSceneBox->ClickMouse())
	{
		DataManager::GetInstance()->SetStageNum(m_selectStageNumber);

		return GAME_SCENE::PLAY;
	}

	return GAME_SCENE();
}

void SelectScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "SelectScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	m_machineDraw->Render();
	m_missionDraw->Render();

	pSB->End();

}

void SelectScene::DrawUI()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	m_arrowDraw[0]->Draw();
	m_arrowDraw[1]->Draw();
	pSB->End();

	m_nextSceneBox->Draw();

}

void SelectScene::Finalize()
{
}
