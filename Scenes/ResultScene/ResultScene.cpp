#include "pch.h"
#include "ResultScene.h"

#include "NecromaLib/Singleton/InputSupport.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

GAME_SCENE ResultScene::Update()
{
	InputSupport* pINP = &InputSupport::GetInstance();

	//　ステージセレクトシーンに遷移
	if (pINP->GetKeybordState().IsKeyReleased(DirectX::Keyboard::Space)) return GAME_SCENE::SELECT;

	return GAME_SCENE();
}

void ResultScene::Draw()
{
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "ResultScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));
}

void ResultScene::DrawUI()
{
}

void ResultScene::Finalize()
{
}
