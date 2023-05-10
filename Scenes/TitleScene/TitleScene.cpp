#include "pch.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

GAME_SCENE TitleScene::Update()
{
	return GAME_SCENE();
}

void TitleScene::Draw()
{
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "TitleScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));
}

void TitleScene::Finalize()
{
}
