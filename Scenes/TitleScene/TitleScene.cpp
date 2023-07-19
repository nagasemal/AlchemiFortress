#include "pch.h"
#include "TitleScene.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"

#include "NecromaLib/Singleton/ShareData.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	m_titleCall = std::make_unique<TitleCall>();
	m_titleCall->Initialize();

	m_titleCamera = std::make_unique<TitleCamera>();
	m_titleCamera->Initialize();

	m_magicCircle = std::make_unique<MagicCircle>();
	m_magicCircle->Initialize();


	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 光の当たり方変更
			lights->SetAmbientLightColor(DirectX::SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));

		});

}

GAME_SCENE TitleScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();

	m_titleCall->Update();
	m_titleCamera->Update();
	m_magicCircle->Update();

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_titleCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_titleCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_titleCamera->GetEyePosition());

	// 魔法陣展開
	m_magicCircle->CreateMagicCircle(DirectX::SimpleMath::Vector3{ 0,0,0 },30);

	return GAME_SCENE();
}

void TitleScene::Draw()
{
	m_titleCall->Render();
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "TitleScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));

	//ShareData& pSD = ShareData::GetInstance();
	//DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	//modelData = DirectX::SimpleMath::Matrix::CreateScale({ 1.8f,1.8f,1.8f });
	//modelData = DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0f,70.0f,0.0f });
	//modelData *= DirectX::SimpleMath::Matrix::CreateRotationY(0.8f);
	//m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	m_magicCircle->CreateWorld();
	m_magicCircle->Render();
}

void TitleScene::DrawUI()
{
}

void TitleScene::Finalize()
{
}
