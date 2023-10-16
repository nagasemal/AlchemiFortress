#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/DataManager.h"

#define MAX_SPEED 4

PlayScene::PlayScene()
{
	m_stageNumber = DataManager::GetInstance()->GetStageNum();

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	//　ステージナンバーのWave1を読み込む
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber,1);

	// 等倍速に設定
	m_doubleSpeedNum = 1;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	// 画面の縦横値の取得
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// フィールドマネージャークラスの生成
	m_fieldManager = std::make_unique<FieldObjectManager>();
	m_fieldManager ->Initialize();
	// マウスポインタークラスの生成
	m_mousePointer	= std::make_unique<MousePointer>();
	m_mousePointer	->Initialize();
	// ユニット(マシン)マネージャークラスの生成
	m_AM_Manager	= std::make_unique<AlchemicalMachineManager>();
	m_AM_Manager	->Initialize();
	// カメラを動かすクラスの生成
	m_moveCamera	= std::make_unique<MoveCamera>();
	m_moveCamera	->Initialize();
	// エネミーマネージャークラスの生成
	m_enemyManager  = std::make_unique<EnemyManager>();
	m_enemyManager	->Initialize();
	// リソースを示すゲージクラスの生成
	m_resourceGauge = std::make_unique<Gauge>();
	m_resourceGauge	->Initialize();
	// 拠点のLvを示すクラスの生成
	m_baseLv = std::make_unique<BaseLv>();
	// ミッションマネージャークラスの生成
	m_missionManager = std::make_unique<MissionManager>();
	m_missionManager->Initialize();

	// チュートリアルクラスの生成
	m_tutorial = std::make_unique<Tutorial>();
	m_tutorial->Initialize(ShareJsonData::GetInstance().GetStageData().tutorial);

	// 操作方法クラスの生成
	m_explanation = std::make_unique<Explanation>();

	// 倍速ボタンの生成
	m_doubleSpeedButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / 1.04f, height / 1.1f), SimpleMath::Vector2(1.0f, 1.0f));

	// 天球モデルのロード
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	// 天球モデルのロード
	m_skySphere->UpdateEffects([&](IEffect* effect)
	{
		// 今回はライトだけ欲しい
		auto lights = dynamic_cast<IEffectLights*>(effect);

		// 光の当たり方変更
		lights->SetAmbientLightColor(SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));
	});
}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DeltaTime& pDelta = DeltaTime::GetInstance();
	bool tutorialFlag = m_tutorial->GetTutorialFlag();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_PLAY, false);

	m_tutorial			->Update(m_AM_Manager.get(),
								 m_resourceGauge.get(),
								 m_missionManager->GetMissionRender()->get(),
								 m_moveCamera->GetStopCameraFlag());

	m_explanation->Update();

	m_missionManager->Update(m_AM_Manager.get(), m_enemyManager.get(), m_fieldManager.get());

	//　次のWaveに進んだことを知らせる
	if (m_missionManager->NextWaveFlag())
	{
		DataManager* pDM = DataManager::GetInstance();
		ShareJsonData& pSJD = ShareJsonData::GetInstance();
		Stage_Data stageData = pSJD.GetStageData();

		m_AM_Manager->ReloadResource();
		m_enemyManager->ReloadEnemyData();
		m_tutorial->RelodeTutorial(stageData.tutorial);
		m_missionManager->ReloadWave();

		// リソース群を追加する
		pDM->SetNowBaseHP(pDM->GetNowBaseHP() + stageData.resource.hp);
		pDM->SetNowCrystal(pDM->GetNowCrystal() + stageData.resource.crystal);
		pDM->SetNowMP(pDM->GetNowMP() + stageData.resource.mp);

	}

	// チュートリアル中ならば以下の処理を通さない
	if (tutorialFlag) 		return GAME_SCENE();

	// 倍速ボタンのアップデート
	m_doubleSpeedButton->HitMouse();
	m_doubleSpeedNum += m_doubleSpeedButton->ClickMouse();
	if (m_doubleSpeedNum > MAX_SPEED) m_doubleSpeedNum = 1;
	// 倍速にする
	pDelta.SetDeltaTime(pDelta.GetDeltaTime() * m_doubleSpeedNum);

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

	bool enemyActivs	= !m_enemyManager->GetEnemyData()->empty();

	// エネミーToバレット
	// ダングリング対策
	if (!m_AM_Manager	->GetBullet()->empty() && enemyActivs) EnemyToBullet();

	// エネミーToプレイヤーベース(当たり判定)
	if (enemyActivs)	  EnemyToPlayerBase();

	// カメラを動かす
	pSD.GetCamera()		->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()		->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()		->SetEyePosition	(m_moveCamera->GetEyePosition());

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



	InputSupport* pINP = &InputSupport::GetInstance();

	// マシンのデータ再読み込み
	if (pINP->GetKeybordState().IsKeyReleased(Keyboard::M))
	{
		ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	///*===[ デバッグ文字描画 ]===*/
	//std::wostringstream oss;
	//oss << "PlayScene";

	//pSD.GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 60.f));

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.8f,1.8f,1.8f });
	modelData = SimpleMath::Matrix::CreateTranslation({0.0f,70.0f,0.0f });

	// 天球描画
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	// フィールドオブジェクト描画
	m_fieldManager      ->Draw();

	// エネミー描画
	m_enemyManager		->Render();

	// マシンの描画
	m_AM_Manager		->Render();

	// マウスポインターモデルを出す
	m_mousePointer->Draw();
	m_mousePointer->ModelDraw(m_AM_Manager->GetSelectModel());

	pSD.GetSpriteBatch()->End();
}

void PlayScene::DrawUI()
{

	auto pSB = ShareData::GetInstance().GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	m_tutorial			->Render();

	m_AM_Manager		->DrawUI();
	m_resourceGauge		->Render();

	m_baseLv			->Render();

	m_missionManager	->Render();

	m_tutorial			->Render_Layer2();

	// 倍速ボタン
	m_doubleSpeedButton->DrawUI(10 + m_doubleSpeedNum);

	// 操作説明描画
	m_explanation->Render(m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), m_AM_Manager->GetRotateStopFlag());


	//SimpleMath::Vector2 origin = SimpleMath::Vector2(1280, 1280);
	//RECT rect = { 0,0,origin.x,origin.y };
	//pSB->Begin();
	//pSB->Draw(pSL->GetMagicCircleTexture(0).Get(), SimpleMath::Vector2(), &rect, Colors::White, 0.0f, origin / 2, 0.3f);
	//pSB->End();

}

void PlayScene::Finalize()
{
	m_fieldManager      ->Finalize();
	m_fieldManager.reset();

	m_mousePointer		->Finalize();
	m_mousePointer.reset();
	
	m_AM_Manager		->Finalize();
	m_AM_Manager.reset();

	m_enemyManager		->Finalize();
	m_enemyManager.reset();

	m_moveCamera.reset();

}

void PlayScene::EnemyToAMMachine()
{
	//for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	//{
	//	for (int i = 0; i < m_AM_Manager->GetAlchemicalMachineObject().size();i++)
	//	{
	//		AlchemicalMachineObject* object = m_AM_Manager->GetAlchemicalMachineObject(i)->get();
	//		bool hitEnemy = false;
	//		// 当たり判定処理
	//		if (hitEnemy = CircleCollider(object->GetCircle(), enemyIt->GetCircle())
	//		&&  object->GetModelID() == AlchemicalMachineObject::DEFENSER)
	//		{
	//			enemyIt->HitMachine(hitEnemy);
	//		}
	//	}
	//}
}

void PlayScene::EnemyToPlayerBase()
{
	PlayerBase* playerBase = m_fieldManager->GetPlayerBase();

	for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	{
		bool hitEnemy;

		hitEnemy = CircleCollider(playerBase->GetCircle(), enemyIt->GetCircle());

		// 当たり判定処理
		if (hitEnemy)
		{
			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
			enemyIt->SetStopFlag(hitEnemy);
		}
	}
}

void PlayScene::EnemyToBullet()
{

	for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
	{
		for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
		{
			// 当たり判定処理
			if (CircleCollider(bulletIt->get()->GetCircle(), enemyIt->GetCircle()))
			{
				bulletIt->get()->SetEnemyHit(true);
				bulletIt->get()->SetLife(0);
				enemyIt->SetHp(enemyIt->GetHp() - (int)bulletIt->get()->GetDamage());
			}
		}
	}

}
