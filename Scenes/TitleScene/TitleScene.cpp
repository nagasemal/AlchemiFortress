#include "pch.h"
#include "TitleScene.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"
#include "Scenes/Commons/PopLine.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

#define COLOR SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.55f)

TitleScene::TitleScene()
{
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	m_titleCall = std::make_unique<DrawMachine>();
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
			//// 今回はライトだけ欲しい
			//auto lights = dynamic_cast<IEffectLights*>(effect);

			//// 光の当たり方変更
			//lights->SetAmbientLightColor(SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));

		});

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	m_titleLogo = std::make_unique<TitleLogo>();
	m_titleLogo->Create(pSL.GetTitleLogoPath());
	m_titleLogo->SetWindowSize(width, height);
	m_titleLogo->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.6f, 1.0f));
	m_titleLogo->SetPosition(SimpleMath::Vector2(width / 1.3f, height / 1.2f));

	m_veil = std::make_unique<Veil>();
	m_veil->Create(L"Resources/Textures/TitleText.png");
	m_veil->LoadShaderFile(L"Veil");
	m_veil->SetWindowSize(width, height);
	m_veil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_veil->SetScale(SimpleMath::Vector2(450,height));
	m_veil->SetPosition(SimpleMath::Vector2(width / 1.95f, 0.0f));


	// ボタンのアップデート
	for (int i = 0; i < ButtonType::Num; i++)
	{
		m_selectionButton[i] = std::make_unique<PopLine>(SimpleMath::Vector2(width / 1.4f, 30 + (i * 120.0f)),
														 SimpleMath::Vector2(150,60),SimpleMath::Vector2(150.0f,60.0f));
	}
}

GAME_SCENE TitleScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	InputSupport* pINP = &InputSupport::GetInstance();

	m_titleCall->Update();
	m_titleCamera->Update();
	m_magicCircle->Update();
	if (m_titleCamera->GetAnimTime() >= 0.75f)
	{
		m_titleLogo->Update();
		m_veil->Update();
	}

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_titleCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_titleCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_titleCamera->GetEyePosition());

	// 魔法陣展開
	m_magicCircle->CreateMagicCircle(SimpleMath::Vector3{ 0,0,0 },30);

	// ボタンのアップデート
	for(int i = 0;i < ButtonType::Num;i++)
	{
		m_selectionButton[i]->Update();
	}



	//　ステージセレクトシーンに遷移
 	if (m_selectionButton[ButtonType::Restert]->ClickMouse()) return GAME_SCENE::SELECT;

	return GAME_SCENE();
}

void TitleScene::Draw()
{
	m_titleCall->Render();
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "TitleScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 60.f));

	m_magicCircle->CreateWorld();
	m_magicCircle->Render(0);
}

void TitleScene::DrawUI()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto device = pSD.GetDeviceResources();

	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	m_veil->Render();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT rect_circle = { 0, 0, 1280, 1280 };

	SimpleMath::Vector2 box_Pos = { (float)width,0.0f};

	// 魔法陣描画
	pSB->Draw(pSL.GetMagicCircleTexture(1).Get(),
			  box_Pos,
			  &rect_circle,
			  COLOR,
			  m_magicCircle->GetAnimationTime(),
			  DirectX::XMFLOAT2(1280 / 2, 1280 / 2),
			  0.5f);


	// ボタンの描画
	for (int i = 0; i < ButtonType::Num; i++)
	{

		m_selectionButton[i]->Draw();

	}

	pSB->End();

	m_titleLogo->Render();

}

void TitleScene::Finalize()
{
}
