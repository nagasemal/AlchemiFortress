#include "pch.h"
#include "TitleScene.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

#define COLOR DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.55f)

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
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 光の当たり方変更
			lights->SetAmbientLightColor(DirectX::SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));

		});

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	m_titleLogo = std::make_unique<TitleLogo>();
	m_titleLogo->Create(pSL.GetTitleLogoPath());
	m_titleLogo->SetWindowSize(width, height);
	m_titleLogo->SetColor(DirectX::SimpleMath::Color(0.2f, 0.2f, 0.65f, 1.0f));
	m_titleLogo->SetPosition(DirectX::SimpleMath::Vector2(width / 2, height / 2.6f));

}

GAME_SCENE TitleScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	InputSupport* pINP = &InputSupport::GetInstance();

	m_titleCall->Update();
	m_titleCamera->Update();
	m_magicCircle->Update();

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_titleCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_titleCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_titleCamera->GetEyePosition());

	// 魔法陣展開
	m_magicCircle->CreateMagicCircle(DirectX::SimpleMath::Vector3{ 0,0,0 },30);

	//　ステージセレクトシーンに遷移
 	if (pINP->GetKeybordState().IsKeyReleased(DirectX::Keyboard::Space)) return GAME_SCENE::SELECT;

	return GAME_SCENE();
}

void TitleScene::Draw()
{
	m_titleCall->Render();
	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "TitleScene";
	ShareData::GetInstance().GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));

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

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT rect_title = { 0, 0, 1000, 580 };
	RECT rect_circle = { 0, 0, 1280, 1280 };

	DirectX::SimpleMath::Vector2 box_Pos = {(float) width / 2.0f,(float)height / 2.0f};

	// 魔法陣描画
	pSB->Draw(pSL.GetMagicCircleTexture(1).Get(),
			  box_Pos,
			  &rect_circle,
			  COLOR,
			  m_magicCircle->GetAnimationTime(),
			  DirectX::XMFLOAT2(1280 / 2, 1280 / 2),
			  0.5f);

	//// タイトル描画
	//pSB->Draw(pSL.GetTitleLogo().Get(),
	//		  box_Pos,
	//		  &rect_title,
	//		  DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,1.0f),
	//		  0.0f,
	//		  DirectX::XMFLOAT2(1000 / 2, 580 / 2),
	//		  1.0f);


	pSB->End();

	m_titleLogo->Render();

}

void TitleScene::Finalize()
{
}
