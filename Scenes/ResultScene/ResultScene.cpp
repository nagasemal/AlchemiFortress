#include "pch.h"
#include "ResultScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/DataManager.h"

#define UI_POS		SimpleMath::Vector2{1280.0f / 2.0f,720.0f / 2.0f}

#define UI_RAGE 	SimpleMath::Vector2{2.5f, 1.5f}

ResultScene::ResultScene()
{
	m_selectionBox_Next = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x - 200, UI_POS.y }, UI_RAGE);

	m_selectionBox_Retry = std::make_unique<SelectionBox>(UI_POS, UI_RAGE);

	m_selectionBox_Back = std::make_unique<SelectionBox>(SimpleMath::Vector2{ UI_POS.x + 200, UI_POS.y }, UI_RAGE);

	m_uiKeyControl = std::make_unique<UIKeyControl>();

	m_uiKeyControl->AddUI(m_selectionBox_Next.get(), 0, 0);
	m_uiKeyControl->AddUI(m_selectionBox_Retry.get(), 1, 0);
	m_uiKeyControl->AddUI(m_selectionBox_Back.get(), 2, 0);

}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

GAME_SCENE ResultScene::Update()
{
	m_selectionBox_Next->HitMouse();
	m_selectionBox_Back->HitMouse();
	m_selectionBox_Retry->HitMouse();

	m_uiKeyControl->Update();

	//　ステージセレクトシーンに遷移
	if (m_selectionBox_Next->ClickMouse())
	{

		DataManager::GetInstance()->SetStageNum(DataManager::GetInstance()->GetStageNum() + 1);
		// 次のステージのデータを読みとる
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum());

		return GAME_SCENE::PLAY;
	}

	//　ステージセレクトシーンに遷移
	if (m_selectionBox_Retry->ClickMouse())
	{

		// 今のステージのデータを読みとる
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum());

		return GAME_SCENE::PLAY;
	}

	//　ステージセレクトシーンに遷移
	if (m_selectionBox_Back->ClickMouse()) return GAME_SCENE::SELECT;

	return GAME_SCENE();
}

void ResultScene::Draw()
{
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "ResultScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 60.f));
}

void ResultScene::DrawUI()
{
	m_selectionBox_Next->DrawUI();
	m_selectionBox_Back->DrawUI();
	m_selectionBox_Retry->DrawUI();
}

void ResultScene::Finalize()
{
}
