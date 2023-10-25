#include "pch.h"
#include "ResultScene.h"
#include "Scenes/ResultScene/Camera/ResultCamera.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/Commons/DrawMachine.h"

#include "Scenes/TitleScene/TitleLogo/Veil.h"

#include "Scenes/DataManager.h"

#define UI_POS		SimpleMath::Vector2{1280.0f / 1.35f,720.0f / 1.35f}

#define UI_RAGE 	SimpleMath::Vector2{2.5f, 1.0f}

ResultScene::ResultScene()
{
	m_selectionBox_Next = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x + 200, UI_POS.y }, UI_RAGE);

	m_selectionBox_Retry = std::make_unique<SelectionBox>(UI_POS, UI_RAGE);

	m_selectionBox_Back = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x - 200, UI_POS.y }, UI_RAGE);

	m_uiKeyControl = std::make_unique<UIKeyControl>();

	m_uiKeyControl->AddUI(m_selectionBox_Next.get());
	m_uiKeyControl->AddUI(m_selectionBox_Retry.get());
	m_uiKeyControl->AddUI(m_selectionBox_Back.get());

	ShareJsonData::GetInstance().LoadingJsonFile_ClearData(DataManager::GetInstance()->GetStageNum());

	//m_veil = std::make_unique<Veil>(3);
	//m_veil->Create(L"Resources/Textures/TitleText.png");
	//m_veil->LoadShaderFile(L"Veil");
	//m_veil->SetWindowSize(width, height);
	//m_veil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	//m_veil->SetScale(SimpleMath::Vector2(width, height / 2));
	//m_veil->SetPosition(SimpleMath::Vector2(0, height / 2));

	m_drawMachine = std::make_unique<DrawMachine>();
	m_drawMachine->Initialize(0);

	m_resultCamera = std::make_unique<ResultCamera>();
	m_resultCamera->Initialize();

	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_baseModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower.cmo", *fx);
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{

}

GAME_SCENE ResultScene::Update()
{

	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DataManager& pDataM = *DataManager::GetInstance();
	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_RESULT, false);

	m_resultCamera->Update();

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_resultCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_resultCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_resultCamera->GetEyePosition());

	m_selectionBox_Next->HitMouse();
	m_selectionBox_Back->HitMouse();
	m_selectionBox_Retry->HitMouse();

	m_uiKeyControl->Update();

	// マシンのアップデート
	m_drawMachine->Update();

	//　次のステージに遷移
	if (m_selectionBox_Next->ClickMouse() && pDataM.GetStageNum() < ShareJsonData::GetInstance().GetGameParameter().stage_Max)
	{

		pDataM.SetStageNum(pDataM.GetStageNum() + 1);
		// 次のステージのデータを読みとる
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(pDataM.GetStageNum(),1);

		return GAME_SCENE::PLAY;
	}

	//　ステージセレクトシーンに遷移
	if (m_selectionBox_Retry->ClickMouse())
	{

		// 今のステージのデータを読みとる
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum(),1);

		return GAME_SCENE::PLAY;
	}

	//　ステージセレクトシーンに遷移
	if (m_selectionBox_Back->ClickMouse()) return GAME_SCENE::SELECT;

	return GAME_SCENE();
}

void ResultScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(3.0f,3.0f,3.0f));
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, 1.0f, 0.0f));

	m_baseModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{
			//// 深度ステンシルステートの設定
			//pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilBase().Get(), 1);
		});

	m_drawMachine->Render();
}

void ResultScene::DrawUI()
{
	SpriteLoder& pSL = SpriteLoder::GetInstance();

	m_selectionBox_Next	->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(128, 28, 0, 0), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	m_selectionBox_Back	->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(128, 28, 1, 0), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	m_selectionBox_Retry->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(128, 28, 2, 0), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

	//m_veil->Render();
}

void ResultScene::Finalize()
{
	m_drawMachine->Finalize();
}
