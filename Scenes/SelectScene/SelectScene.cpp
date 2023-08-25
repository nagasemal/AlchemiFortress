#include "pch.h"
#include "SelectScene.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "Scenes/DataManager.h"

#define MAX_STAGE 4
#define MIN_STAGE 1

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

	m_missionDraw = std::make_unique<MissionRender>(DirectX::SimpleMath::Vector2{200,300 }, DirectX::SimpleMath::Vector2{1,1});

}

GAME_SCENE SelectScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	InputSupport* pINP = &InputSupport::GetInstance();

	m_selectCamera->Update();
	m_machineDraw->Update();

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_selectCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_selectCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_selectCamera->GetEyePosition());
 
	if (pINP->GetKeybordState().IsKeyReleased(DirectX::Keyboard::Q) && m_selectStageNumber < MAX_STAGE)
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

	//　プレイシーンに遷移
	if (pINP->GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED)
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
}

void SelectScene::Finalize()
{
}
