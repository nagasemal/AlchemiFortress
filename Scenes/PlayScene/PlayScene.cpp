#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/DataManager.h"

#define MAX_SPEED 4

//
#define SKYDORM_LIGHTCOLOR SimpleMath::Color(0.2f, 0.5f,3.3f, 1.0f)

PlayScene::PlayScene()
{
	m_stageNumber = DataManager::GetInstance()->GetStageNum();

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	//　ステージナンバーのWave1を読み込む
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber, 1);

	// 等倍速に設定
	m_doubleSpeedNum = 1;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	// 画面の縦横値の取得
	auto device			= ShareData::GetInstance().GetDeviceResources();
	int width			= device->GetOutputSize().right;
	int height			= device->GetOutputSize().bottom;

	// ポストプロセスの生成
	m_postProcess		= std::make_unique<MyPostProcess>();
	m_postProcess		->CreateShader();

	// フィールドマネージャークラスの生成
	m_fieldManager		= std::make_unique<FieldObjectManager>();
	m_fieldManager		->Initialize();

	// マウスポインタークラスの生成
	m_mousePointer		= std::make_unique<MousePointer>();
	m_mousePointer		->Initialize();

	// ユニット(マシン)マネージャークラスの生成
	m_AM_Manager		= std::make_unique<AlchemicalMachineManager>();
	m_AM_Manager		->Initialize();

	// カメラを動かすクラスの生成
	m_moveCamera		= std::make_unique<MoveCamera>();
	m_moveCamera		->Initialize();

	// エネミーマネージャークラスの生成
	m_enemyManager		= std::make_unique<EnemyManager>();
	m_enemyManager		->Initialize();

	// リソースを示すゲージクラスの生成
	m_resourceGauge		= std::make_unique<Gauge>();
	m_resourceGauge		->Initialize();

	// 拠点のLvを示すクラスの生成
	UI_Data uiData		= ShareJsonData::GetInstance().GetUIData("GaugeBaseLv");
	m_baseLv			= std::make_unique<BaseLv>();
	m_baseLv			->SetPosition(uiData.pos);
	m_baseLv			->SetScale(uiData.rage);

	// ミッションマネージャークラスの生成
	m_missionManager	= std::make_unique<MissionManager>();
	m_missionManager	->Initialize();

	// 操作方法クラスの生成
	m_explanation		= std::make_unique<Explanation>();

	// 倍速ボタンの生成
	m_doubleSpeedButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / 1.05f, height / 1.6f), SimpleMath::Vector2(1.0f, 1.0f));

	// 天球モデルのロード
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkyDome.cmo", *fx);

	// 天球モデルのロード
	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// ライトの取得
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 当たる光の色を変更
			lights->SetAmbientLightColor(SKYDORM_LIGHTCOLOR);
		});

	// チュートリアルクラスの生成
	m_operationInstructions = std::make_unique<OperationInstructions>();
	m_operationInstructions->Initialize(ShareJsonData::GetInstance().GetStageData().tutorial, this);

	m_tutorialManager = std::make_unique<TutorialManager>(this);
	m_tutorialManager->ChangeWave(1);

	// レンダーテクスチャの作成（シーン全体）
	m_offscreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_offscreenRT->SetDevice(pSD.GetDevice());
	RECT rect = pSD.GetDeviceResources()->GetOutputSize();
	m_offscreenRT->SetWindow(rect);
	m_offscreenRT->GetShaderResourceView();


}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DeltaTime& pDelta = DeltaTime::GetInstance();

	bool operationNow = m_operationInstructions->GetExplanationFlag();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_PLAY, false);

	// 倍速ボタンの更新処理
	m_doubleSpeedButton->HitMouse();
	m_doubleSpeedNum += m_doubleSpeedButton->ClickMouse();
	if (m_doubleSpeedNum > MAX_SPEED) m_doubleSpeedNum = 1;

	if (m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven())
	{
		// 低減
		pDelta.SetDoubleSpeed((float)m_doubleSpeedNum / 1.5f);
	}
	else
	{
		// 倍速にする
		pDelta.SetDoubleSpeed((float)m_doubleSpeedNum);
	}

	m_operationInstructions->Update(this, m_moveCamera->GetStopCameraFlag());

	m_missionManager->Update(m_AM_Manager.get(), m_enemyManager.get(), m_fieldManager.get());

	m_tutorialManager->Update(m_missionManager->NextWaveFlag());

	m_explanation->Update();

	//　次のWaveに進んだことを知らせる
	if (m_missionManager->NextWaveFlag())
	{
		DataManager* pDM = DataManager::GetInstance();
		ShareJsonData& pSJD = ShareJsonData::GetInstance();
		Stage_Data stageData = pSJD.GetStageData();

		m_AM_Manager->ReloadResource();
		m_enemyManager->ReloadEnemyData();
		//m_operationInstructions->RelodeTutorial(stageData.tutorial, this);
		m_missionManager->ReloadWave();
		m_tutorialManager->ChangeWave(m_missionManager->GetWave());

		// リソース群を追加する
		pDM->SetNowBaseHP(pDM->GetNowBaseHP() + stageData.resource.hp);
		pDM->SetNowCrystal(pDM->GetNowCrystal() + stageData.resource.crystal);
		pDM->SetNowMP(pDM->GetNowMP() + stageData.resource.mp);

	}

	// タイトル移行ボタンが押されたらタイトルシーンに向かう
	if (m_operationInstructions->GetTitleSceneButton()->ClickMouse())	return GAME_SCENE::TITLE;
	// セレクト移行ボタンが押されたらセレクトシーンに向かう
	if (m_operationInstructions->GetSelectSceneButton()->ClickMouse())	return GAME_SCENE::SELECT;

	// チュートリアル中ならば以下の処理を通さない
	if (operationNow) 		return GAME_SCENE();

	m_moveCamera		->Update(!m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), true);

	m_fieldManager		->Update(m_enemyManager.get());
	m_mousePointer		->Update();

	// ユニット(マシン)マネージャーのアップデート
	m_AM_Manager		->Update(m_fieldManager.get(),
								 m_mousePointer.get(),
								 m_enemyManager.get(),
								 m_moveCamera.get());

	m_missionManager	->TimerUpdate();

	m_enemyManager		->Update(m_fieldManager->GetPlayerBase()->GetPos());

	m_resourceGauge		->Update();

	m_baseLv			->Update(m_fieldManager.get());

	bool enemyActivs = !m_enemyManager->GetEnemyData()->empty();

	// エネミーToバレット
	// ダングリング対策
	if (!m_AM_Manager->GetBullet()->empty() && enemyActivs) EnemyToBullet();

	// エネミーToプレイヤーベース(当たり判定)
	if (enemyActivs)	  EnemyToPlayerBase();


	InputSupport* pINP = &InputSupport::GetInstance();

	if (pINP->GetKeybordState().IsKeyReleased(Keyboard::C))
	{
		m_moveCamera->SetTargetProsition(SimpleMath::Vector3(0.0f,15.0f,0.0f));
	}

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_moveCamera->GetEyePosition());

	// 拠点のHPが0になる、ミッションの内容的にクリアが不可能になった場合リザルトへ切り替える
	if (m_missionManager->MissionmFailure())		return GAME_SCENE::RESULT;

	// ミッションを全て達成したらリザルトへ切り替える
	if (m_missionManager->MissionComplete())
	{
		// ステージ攻略情報を得る
		Json::WritingJsonFile_ClearData(m_stageNumber,
			m_AM_Manager->GetAlchemicalMachineObject(),
			m_missionManager->GetStartTimer());

		return GAME_SCENE::RESULT;
	}

	// マシンのデータ再読み込み
	if (pINP->GetKeybordState().IsKeyReleased(Keyboard::M))
	{
		ModelShader::GetInstance().CreateModelShader();
		ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto context = pSD.GetContext();

	ModelShader& pMS = ModelShader::GetInstance();

	// 深度マップ生成
	auto srv = pMS.GetShadowMap()->GetShaderResourceView();

	// 深度マップに書き込む
	DrawShadow();

	//　====================[　元に戻すためのレンダーターゲット　]
	auto renderTarget = pSD.GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = pSD.GetDeviceResources()->GetDepthStencilView();

	//　====================[　新規レンダーターゲット　]
	auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
	auto offscreenSRV = m_offscreenRT->GetShaderResourceView();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットを変更（sceneRT）
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(m_offscreenRT->GetRenderTargetView(), Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &offscreenRTV, depthStencil);
	// -------------------------------------------------------------------------- //

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.0f,1.0f,1.0f });
	modelData *= SimpleMath::Matrix::CreateTranslation({ 0.0f,10.0f,0.0f });

	// 天球描画
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
		});

	//　====================[　フィールドオブジェクト描画　] 
	m_fieldManager		->Draw();

	//　====================[　エネミー描画　]
	m_enemyManager		->Render();

	//　====================[　マシンの描画　]
	m_AM_Manager		->Render();

	pSD.GetSpriteBatch()->End();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(renderTarget, Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = pSD.GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	// -------------------------------------------------------------------------- //

	// リソースの割り当てを解除する
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	//// ポストプロセスを適応させる
	m_postProcess->Render(&offscreenSRV, &srv);

	//pSD.GetSpriteBatch()->Begin();
	//pSD.GetSpriteBatch()->Draw(srv, SimpleMath::Vector2::Zero);
	//pSD.GetSpriteBatch()->End();

	// マウスポインターモデルを出す
	m_mousePointer->Draw();
	m_mousePointer->ModelDraw(m_AM_Manager->GetSelectModel());
}

void PlayScene::DrawShadow()
{
	ShareData& pSD = ShareData::GetInstance();
	auto context = pSD.GetContext();

	ModelShader& pMS = ModelShader::GetInstance();

	//　====================[　新しいレンダーターゲット　]
	auto rtv = pMS.GetShadowMap()->GetRenderTargetView();

	auto dsv = pMS.GetDepthStencilView().Get();

	//　====================[　元に戻すためのレンダーターゲット　]
	auto renderTarget = pSD.GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = pSD.GetDeviceResources()->GetDepthStencilView();

	// レンダーターゲットを変更（shadowMapRT）
	context->ClearRenderTargetView(pMS.GetShadowMap()->GetRenderTargetView(), Colors::Black);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートを設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, 1280, 720, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.0f,1.0f,1.0f });
	modelData *= SimpleMath::Matrix::CreateTranslation({ 0.0f,10.0f,0.0f });

	// 天球描画
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw(false);
		});

	m_fieldManager->WriteDepath();

	m_AM_Manager->WriteDepath();

	// -------------------------------------------------------------------------- //
// レンダーターゲットとビューポートを元に戻す
// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(renderTarget, Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = pSD.GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	// -------------------------------------------------------------------------- //
}

void PlayScene::DrawUI()
{

	m_operationInstructions->Render();

	m_tutorialManager->Render();

	m_AM_Manager->DrawUI();
	m_AM_Manager->DrawMachineExplanationModel();


	m_enemyManager->RenderUI();

	m_resourceGauge->Render();

	if (m_resourceGauge->GaugeActive())
	{
		m_baseLv->Render();
	}

	m_missionManager->Render();

	// 倍速ボタン
	m_doubleSpeedButton->DrawUI(10 + m_doubleSpeedNum);

	// 操作説明描画
	m_explanation->Render(m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), m_AM_Manager->GetRotateStopFlag());

	m_operationInstructions->Render_Layer2();

	m_tutorialManager->Render_Layer2();

}

void PlayScene::Finalize()
{
	m_fieldManager->Finalize();
	m_fieldManager.reset();

	m_mousePointer->Finalize();
	m_mousePointer.reset();

	m_AM_Manager->Finalize();
	m_AM_Manager.reset();

	m_enemyManager->Finalize();
	m_enemyManager.reset();

	m_moveCamera.reset();

}

void PlayScene::EnemyToAMMachine()
{
}

void PlayScene::EnemyToPlayerBase()
{
	PlayerBase* playerBase = m_fieldManager->GetPlayerBase();

	for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitEnemy = CircleCollider(playerBase->GetObject3D(), enemyIt->GetObject3D());

		// 当たり判定処理
		if (hitEnemy)
		{
			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
			enemyIt->SetStopFlag(hitEnemy);
			// 反発させる
			enemyIt->SetAccele(-enemyIt->GetAccele());

		}
	}
}

void PlayScene::EnemyToBullet()
{

	for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
	{
		for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
		{

			// 当たり判定を取るか否か
			if (!enemyIt->GetColliderActive()) continue;

			// 当たり判定処理
			if (CircleCollider(bulletIt->get()->GetObject3D(), enemyIt->GetObject3D()))
			{

				SimpleMath::Vector3 hitVectol = bulletIt->get()->GetPos() - enemyIt->GetPos();

				bulletIt->get()->SetEnemyHit(true);
				bulletIt->get()->SetLife(0);

				enemyIt->HitBullet(hitVectol, (int)bulletIt->get()->GetDamage());

			}
		}
	}

}
