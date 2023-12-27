#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "Scenes/Commons/DrawVariableNum.h"

#include "DirectXHelpers.h"

#include "Scenes/PlayScene/Camera/MoveCamera.h"
#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

#define SPEED 0.0025f

// 魔力を回復する間隔
#define MPPLUSTIME 2.0f
// 魔力の回復量
#define MPPLUSNUM  1.0f
// 魔力を回収しはじめる最低マシン量
#define MPPUSLMACHINE 8

// 円周の一番小さい場合におけるマシンの最大数
#define CIRCLE_MAX_MIN  4

// 円周の最大ライン
#define CIRCLE_MAX_LINE 9

// ライン同士の間隔
#define CIRCLE_LINE_DISTANCE 5.5f

// 魔法陣出現の高さ(大)
#define MAGICCIRCLE_HEIGHT	0.3f 

// バレットの大きさ
#define BULLET_RAGE 0.75f


AlchemicalMachineManager::AlchemicalMachineManager() :
	m_allHitObjectToMouse(),
	m_selectNumber(-1),
	m_prevSelectMachinePos(0, 0, 0),
	m_mpPulsTimer(),
	m_AMnums{ 0,0,0,0,0,0 },
	m_saveWheelValue(0),
	m_scrollValue(),
	m_rotationStop(),
	m_crystalPulsVal(),
	m_mpPulsVal(),
	m_spawnMachine		(MACHINE_TYPE::NONE),
	m_destroyMachine	(MACHINE_TYPE::NONE),
	m_recoveryMachine	(MACHINE_TYPE::NONE),
	m_lvUpMachine		(MACHINE_TYPE::NONE)
{
}

AlchemicalMachineManager::~AlchemicalMachineManager()
{
}

void AlchemicalMachineManager::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	//　====================[　クラス取得　]

	//　　|=>　マシンの詳細描画クラス
	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	//　　|=>　マシンUIの描画クラス
	m_selectManager		= std::make_unique<MachineSelectManager>();
	m_selectManager		->TextuerLoader();
	m_selectManager		->Initialize();

	//　　|=>　マシンのモデルパッククラス
	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	//　　|=>　バレットに使用するモデル
	m_testBox = GeometricPrimitive::CreateSphere(pSD.GetContext(),0.75f);

	//　　|=>　丸影描画クラス
	m_dorpShadow		= std::make_unique<DorpShadow>();
	m_dorpShadow		->Initialize();

	//　　|=>　[パーティクル] バレットToエネミー
	m_particle_hit		= std::make_unique<Particle>(Particle::HIT_BULLET);
	m_particle_hit		->Initialize();

	//　　|=>　[パーティクル] マシン設置時
	m_particle_Put		= std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Put		->Initialize();

	//　　|=>　[パーティクル] ディフェンサー攻撃時
	m_particle_Gurd		= std::make_unique<Particle>(Particle::DEFENSE_EFFECT);
	m_particle_Gurd		->Initialize();

	//　　|=>　[パーティクル] クリスタル回収時
	m_particle_Mining	= std::make_unique<Particle>(Particle::MINING_EFFECT);
	m_particle_Mining	->Initialize(L"Crystal");
	m_particle_Mining	->SetParticleSpawnTime(1.0f);

	//　　|=>　[パーティクル] 魔力回収時
	m_particle_Recovery = std::make_unique<Particle>(Particle::RECOVERY_EFFECT);
	m_particle_Recovery	->Initialize(L"MP");
	m_particle_Recovery	->SetParticleSpawnTime(1.0f);

	//　　|=>　[パーティクル] バレットの軌跡
	m_particle_Bullet	= std::make_unique<Particle>(Particle::BULLET_LINE);
	m_particle_Bullet	->Initialize();

	//　　|=>　マシン用魔法陣描画クラス
	m_magicCircle		= std::make_unique<MagicCircle>();
	m_magicCircle		->Initialize();

	//　　|=>　拠点用魔法陣描画クラス
	m_magicCircle_Field = std::make_unique<MagicCircle>();
	m_magicCircle_Field	->Initialize();


	//　====================[　マシンの生成　]
	CreateAMMachine();
	
	//　====================[　現在場に出せるマシンの設定　]
	LvToObjectActives(1);
	
	//　====================[　Jsonからリソースを取得する　]
	JsonLoadResources();

	//　====================[　ビルボード描画に使用　]
	//　　|=> エフェクトを作成
	m_effect = std::make_unique<BasicEffect>(ShareData::GetInstance().GetDevice());
	m_effect->SetTextureEnabled(true);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetLightingEnabled(false);

	//　　|=> 入力レイアウトを作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(
			ShareData::GetInstance().GetDevice(),
			m_effect.get(),
			VertexPositionColorTexture::InputElements,
			VertexPositionColorTexture::InputElementCount,
			m_inputLayout.ReleaseAndGetAddressOf())
	);

}

void AlchemicalMachineManager::Update(
	FieldObjectManager* fieldManager,
	MousePointer* pMP,
	EnemyManager* enemys,
	MoveCamera* moveCamera)
{
	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();
	PlayerBase* pPlayerBase = fieldManager->GetPlayerBase();

	//　====================[　入力関係　]
	auto mouse			= pINP.GetMouseState();
	auto keyboard		= pINP.GetKeybordState();
	bool leftRelease	= pINP.LeftButton_Release();

	//　====================[　回転の停止/再開　]
	if (pINP.RightButton_Press())
	{
		m_rotationStop = !m_rotationStop;
	}

	//　====================[　設置個所の設定　]
	LvToObjectActives(pPlayerBase->GetBaseLv());

	//　====================[　マシンの数をカウントする変数　]
	//　　|=>　全てのマシン
	int amNum			= 0;
	//　　|=>　Noneマシン
	int amNum_Nomal		= 0;

	//　====================[　リソースの取得量のリセット　]
	m_mpPulsVal			= 0;
	m_crystalPulsVal	= 0;

	//　====================[　マシンToマウスの判定リセット　]
	m_allHitObjectToMouse = false;

	//　====================[　通知系変数のリセット　]
	m_spawnMachine = m_destroyMachine = m_recoveryMachine = m_lvUpMachine = MACHINE_TYPE::NONE;

	//　====================[　魔力回復周期　]
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	//　====================[　パーティクルの更新　]
	Update_Particle();

	//　====================[　マシンのLvUP,修繕,破壊を行うクラスの更新　]
	m_machineExplanation->Update();

	//　====================[　選択状態の解除　]
	if (leftRelease)
	{
		// カメラ移動用の時間変数を可能ならばリセットする
		moveCamera->ResetTargetChangeTimer();

		// 選択されたオブジェクトがない場合の処理
		if (m_selectNumber != -1)
		{
			m_prevSelectMachinePos = m_AMObject[m_selectNumber]->GetPos();
		}
		else
		{
			m_prevSelectMachinePos = SimpleMath::Vector3();
		}

		// 選択状態の解除
		if (!pINP.GetHitUI())
		{
			m_selectNumber = -1;
		}
	}

	//　====================[　セレクトマネージャーの更新処理　]
	m_selectManager->Update(fieldManager);

	//　====================[　選択中のオブジェクトがある場合の処理　]
	if (m_selectNumber != -1)
	{

		//　注視点移動
		moveCamera->TargetChange(m_AMObject[m_selectNumber]->GetData().pos);

		//　説明文のアップデート処理を回す
		m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());

		//　選択済みのオブジェクトの選択時アップデートを回す
		m_AMObject[m_selectNumber]->SelectUpdate();
		m_AMObject[m_selectNumber]->SelectUpdate_Common();

		//　選択オブジェクトに魔法陣展開
		m_magicCircle->CreateMagicCircle(
			m_AMObject[m_selectNumber]->GetPos(),
			m_AMObject[m_selectNumber]->GetMagicCircle().r,
			m_AMObject[m_selectNumber]->GetColor());

	}
	//　====================[　選択中のオブジェクトがない場合の処理　]
	else
	{
		m_machineExplanation->ResetMoveTime();
		m_magicCircle		->DeleteMagicCircle();

		//　注視点移動
		moveCamera->TargetChange({ 0,0,0 });

	}

	//　====================[　登録した影情報を消す　]
	m_dorpShadow->DeleteShadow();

	//　====================[　全マシンの更新処理　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		//　================[　マシンを動かす　]
		MovingMachine(i);
		m_AMObject[i]->Update_Common();

		//　================[　マシンToマウス　]
		if (m_AMObject[i]->GetHitMouse())
		{

			m_allHitObjectToMouse = true;

			// クリックで選択状態に移行
			if (leftRelease && m_selectNumber != i)
			{
				// 選択されているマシンのインデックス番号を渡す
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}
		}

		m_AMObject[i]->SetSelectModeFlag(m_selectNumber == i);

		//　存在していれば処理を続ける
		if (!m_AMObject[i]->GetActive()) continue;

		// 丸影の表示位置を定める
		m_dorpShadow->CreateShadow(m_AMObject[i]->GetData().pos);

		// 現状場に存在しているマシンの総数を調べる (None)
		if (m_AMObject[i]->GetModelID() == MACHINE_TYPE::NONE) amNum_Nomal++;
		// 全体数
		amNum++;

		// アルケミカルマシンの更新処理
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToMouse(pMP);

		// MP追加処理 (インデックスがリカバリーを指し、魔力回復周期が来た時、回転を止めていれば魔力を回復させる)
		if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop && m_AMObject[i]->GetModelID() == MACHINE_TYPE::RECOVERY)
		{
			// リカバリーマシンが生きていたら加算を行う
			m_mpPulsVal += (int)m_AMObject[i]->GetHP() > 0 ? (int)m_AMObject[i]->GetMachineEffectValue() * m_AMObject[i]->GetLv() : 0;
		}

		// アルケミカルマシンの個別更新処理
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager,enemys);
		Update_Upper(i, enemys);
		Update_Recovery(i,enemys);
	}

	//　====================[　マシンに変化が起こった際の処理　]
	//　　|=> 配列外参照を避けるために-1を弾く
	if (m_selectNumber != -1)
	{

		// 解体された通知(マシン)を受け取る
		if (m_machineExplanation->GetDismantlingFlag())
		{
			Dismantling(m_selectNumber);
		}

		// 修繕された通知(マシン)を受け取る
		if (m_machineExplanation->GetRepairFlag())
		{
			m_recoveryMachine = m_AMObject[m_selectNumber]	->GetModelID();
		}

		// LvUpされた通知(マシン)を受け取る
		if (m_machineExplanation->GetLvUpFlag())
		{
			m_lvUpMachine = m_AMObject[m_selectNumber]		->GetModelID();
		}

	}

	//　====================[　マシンを召喚する処理　]
	SpawnAMMachine((leftRelease && !pINP.GetHitUI() && m_allHitObjectToMouse) || m_machineExplanation->GetSpawnFlag());

	//　====================[　魔力リソースの増加　]
	//　　|=>　周期が来る　回転が止まっていない
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer	= 0;
		m_mpPulsVal		+= ((int)MPPLUSNUM * ((amNum - amNum_Nomal) / MPPUSLMACHINE));
		pDM.SetNowMP(pDM.GetNowMP() + m_mpPulsVal);
	}

	//　====================[　クリスタルリソースの増加　]
	pDM.SetNowCrystal(pDM.GetNowCrystal() + m_crystalPulsVal);


	//　====================[　所持数の増加　]
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;
	}

	//　====================[　次のマシンを選択　]
	if (m_machineExplanation->GetNextMachineFlag())
	{
		m_selectNumber++;
		if (m_selectNumber > amNum - 1) m_selectNumber = 0;
	}

	//　====================[　前のマシンを選択　]
	if (m_machineExplanation->GetBackMachineFlag())
	{
		m_selectNumber--;
		if (m_selectNumber < 0) m_selectNumber = amNum - 1;
	}


	//　====================[　マウスの当たり判定を戻す　]
	if(leftRelease)  pMP->ReleaseLeftButtom();

	//　====================[　バレット更新処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		m_particle_Bullet->Update(it->get()->GetPos(),true,it->get()->GetColor());

		// 子クラスからfalseで消す
		if ((it)->get()->deleteRequest())
		{
			m_particle_hit->OnShot(it->get()->GetPos(),true,it->get()->GetColor());
			it = m_bullets.erase(it);
			if (it == m_bullets.end()) break;
		}
	}

	//　====================[　魔法陣の位置を定める　]
	m_magicCircle_Field->CreateMagicCircle(
		SimpleMath::Vector3{ 0,MAGICCIRCLE_HEIGHT,0 },
		pPlayerBase->GetBaseLv() * CIRCLE_LINE_DISTANCE);

	if(!m_rotationStop) m_magicCircle_Field->CreateWorld();
}

void AlchemicalMachineManager::Render()
{

	//　====================[　置いた際に出すパーティクル　]
	//　　|=>　描画順の影響で先に描画
	m_particle_Put->Render();

	//　====================[　シルエット描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()),true);

		}
	}

	//　====================[　通常描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()), false);
		
			m_AMObject[i]->Draw();
		
		}

	}

	//　====================[　マシンが選択されている　]
	if (m_selectNumber != -1)
	{
		//　　|=>　マシンの魔法陣描画処理
		m_magicCircle->CreateWorld();
		m_magicCircle->Render(m_AMObject[m_selectNumber]->GetModelID());
	}

	
	//　====================[　丸影の描画処理　]
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	//　====================[　パーティクルの描画処理　]
	m_particle_hit		->Render();
	m_particle_Gurd		->Render();
	m_particle_Mining	->Render();
	m_particle_Recovery	->Render();
	m_particle_Bullet	->Render();

	//　====================[　バレットの描画処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}

	//　====================[　拠点の魔法陣描画処理　]
	m_magicCircle_Field->Render(0);

}

void AlchemicalMachineManager::WriteDepath()
{
	//　====================[　影描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			m_AMObject[i]->WriteDepathRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
											 m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()));
		}
	}
}


void AlchemicalMachineManager::DrawUI()
{

	//　====================[　マシンUIの指定可視化UIの描画　]
	m_selectManager->Render();

	//　====================[　マシンアイコンUIの描画　]
	//　　|=>　Noneマシンを省くために1スタート
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(i);
	}

	//　====================[　UIのビルボード描画　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しなければそれ以降も存在しないため、処理を終了する
		if (!m_AMObject[i]->GetActive()) break;
		// 選択番号と一致している場合は二回ドローコールをする必要はない為、飛ばす
		if (m_selectNumber == i) continue;

		// マウスの周辺にあるマシンの情報開示
		Circle mouseCircle = Circle(InputSupport::GetInstance().GetMousePosWolrd(),5.0f);

		if (PointerToCircle(mouseCircle, m_AMObject[i]->GetPos()) ||
			m_AMObject[i]->GetPopTextTime() > 0.0f)
		{
			BillboardRenderUI(i);
		}
	}

	//　====================[　マシンUIに関連する描画　]
	//　　|=>　錬金ボタンUI
	//　　|=>　マシンの名前
	//　　|=>　必要リソース量
	m_selectManager->RenderUI(m_AMnums);

	//　====================[　UIの表示　]
	if (m_selectNumber != -1)
	{
		BillboardRenderUI(m_selectNumber);

		m_machineExplanation->Draw();

		// 選択したモデルのIDがNoneなら表示しない
		if (m_AMObject[m_selectNumber]->GetModelID() == MACHINE_TYPE::NONE) return;

		m_AMObject[m_selectNumber]->RenderUI();
		m_AMObject[m_selectNumber]->SelectRenderUI_Common();

	}

}

void AlchemicalMachineManager::Finalize()
{

	//　====================[　全マシンの終了処理　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		m_AMObject[i]->Finalize();
		m_AMObject[i].reset();
	}

	m_AMObject.clear();
	m_AMObject.shrink_to_fit();

	//　====================[　全バレットの終了処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Finalize();
	}

	m_bullets.clear();

	//　====================[　UI描画クラスの終了処理　]
	m_selectManager->Finalize();
	m_selectManager.reset();

	//　====================[　マシン詳細クラスの終了処理　]
	m_machineExplanation->Finalize();
	m_machineExplanation.reset();

	m_AMFilter.reset();
	m_testBox.reset();

}

void AlchemicalMachineManager::BillboardRenderUI(int index)
{
	ShareData& pSD = ShareData::GetInstance();
	auto status = pSD.GetCommonStates();
	auto camera = ShareData::GetInstance().GetCamera();
	auto context = pSD.GetContext();

	//　====================[　ビルボード行列生成　]
	//　　|=>　スクリーン座標はY軸が＋－逆なので-1をかける
	SimpleMath::Matrix invertY = SimpleMath::Matrix::CreateScale(1.0f, -1.0f, 1.0f);

	//　　|=> ビュー行列の回転を打ち消す行列を作成する
	SimpleMath::Matrix invView = camera->GetViewMatrix().Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//　　|=> エフェクトにビュー行列と射影行列を設定する
	m_effect->SetView(camera->GetViewMatrix());
	m_effect->SetProjection(camera->GetProjectionMatrix());


	//　====================[　UIをワールド空間に出す]
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, status->NonPremultiplied(), nullptr, status->DepthNone(), status->CullCounterClockwise(), [=]
	{	
		// ワールド行列作成
		SimpleMath::Matrix world =
			SimpleMath::Matrix::CreateScale(0.01f) *
			invertY *
			invView *
			SimpleMath::Matrix::CreateTranslation(m_AMObject[index]->GetPos() + SimpleMath::Vector3(0.0f, 4.5f, 0.0f));

		// エフェクトを適応する
		m_effect->SetWorld(world);
		m_effect->Apply(context);
		// 入力レイアウトを設定する
		context->IASetInputLayout(m_inputLayout.Get());
	});

	//========  開始　========//

	m_AMObject[index]->RenderHP();

	//========  終了　========//

	pSD.GetSpriteBatch()->End();


}

void AlchemicalMachineManager::DrawMachineExplanationModel()
{
	//　====================[　マシンが選択されている　]
	if (m_selectNumber != -1)
	{
		//　　|=>　ディスプレイ用マシンの描画処理
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMObject[m_selectNumber].get());
	}
}

Model* AlchemicalMachineManager::GetSelectModel()
{
	return m_AMFilter->HandOverAMModel(m_selectManager->GetSelectMachineType());
}

void AlchemicalMachineManager::ReloadResource()
{

	auto pSJD = &ShareJsonData::GetInstance();
	Stage_Resource resource = pSJD->GetStageData().resource;

	//　====================[　マシン所持数を追加　]
	m_AMnums[MACHINE_TYPE::ATTACKER]	+= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	+= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		+= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	+= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		+= resource.mining;

}

void AlchemicalMachineManager::Update_Particle()
{

	//　====================[　パーティクル群の更新処理　]
	m_particle_hit			->UpdateParticle();
	m_particle_Put			->UpdateParticle();
	m_particle_Gurd			->UpdateParticle();
	m_particle_Mining		->UpdateParticle();
	m_particle_Recovery		->UpdateParticle();
	m_particle_Bullet		->UpdateParticle();
}

void AlchemicalMachineManager::Update_Attacker(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::ATTACKER) return;


	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	// 判定を取るマシン(Upper)の情報を渡す
	for (int j = 0; j < m_AMObject.size(); j++)
	{
		int upperMachineLine	= m_AMObject[j]->GetLine();
		int attackerMachineLine = m_AMObject[index]->GetLine();

		// 判定を取る条件(ラインが±1の属性が同じUpperで且つ生存している)
		bool toUpperflag = (upperMachineLine + 1 >= attackerMachineLine ||
							upperMachineLine - 1 >= attackerMachineLine) &&
							m_AMObject[j]->GetModelID() == MACHINE_TYPE::UPPER &&
							m_AMObject[j]->GetElement() == m_AMObject[index]->GetElement() &&
							m_AMObject[j]->GetHP() >= 0;

		if(toUpperflag)	 attacker->AllAlchemicalMachine(m_AMObject[j].get());

	}

	//　====================[　バレットの発射リクエスト　]
	if (attacker->BulletRequest(enemys->GetEnemyData()))
	{
		m_bullets.push_back(std::make_unique<Bullet>(attacker->GetBulletData()));
	}
}

void AlchemicalMachineManager::Update_Defenser(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::DEFENSER) return;

	AM_Defenser* defenser = dynamic_cast<AM_Defenser*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	defenser->EnemyHit(enemys->GetEnemyData());

	//　　|=>　専用のパーティクルを出す
	m_particle_Gurd->Update(defenser->GetPos(), defenser->CounterAttack(), defenser->GetColor());

}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	mining->HitEnemy(enemys->GetEnemyData());

	//　====================[　クリスタルリソースの回収　]
	m_crystalPulsVal += mining->AllFieldObject(fieldManager);

	//　　|=>　専用のパーティクルを出す
	m_particle_Mining->Update(mining->GetPos(), !mining->GetCrystalFlag(),mining->GetColor());

}

void AlchemicalMachineManager::Update_Recovery(int index,EnemyManager* enemys)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::RECOVERY) return;

	AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	recovery->HitEnemy(enemys->GetEnemyData());

	//　====================[　回収状態ならばパーティクルを出す　]
	m_particle_Recovery->OnShot(recovery->GetPos(), m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop, recovery->GetColor());
}

void AlchemicalMachineManager::Update_Upper(int index,EnemyManager* enemyManager)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::UPPER) return;

	AM_Upper* upper = dynamic_cast<AM_Upper*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	upper->HitEnemy(enemyManager->GetEnemyData());

}

void AlchemicalMachineManager::MovingMachine(int number)
{
	//　====================[　早期リターン　]
	//　　|=>　回転が停止している
	if (m_rotationStop) return;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	//　====================[　回転速度取得　]
	int rotateSpeed = (int)ShareJsonData::GetInstance().GetGameParameter().rotateSpeed;

	//　====================[　原点を中心に回転移動　]
	SimpleMath::Matrix matrix = SimpleMath::Matrix::Identity;
	matrix *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(rotateSpeed * deltaTime));
	//　　|=>  回転後の座標をマシンに代入
	m_AMObject[number]->SetPos(SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

// Lvに応じて置ける場所が増える為、設定をする
void AlchemicalMachineManager::CreateAMMachine()
{
	int counter = 0;

	//　====================[　影の配列をリセットする　]
	m_dorpShadow->DeleteShadow();

	int circle_max_line = ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX;

	//　====================[　配置箇所を決める　]
	for (int i = 1; i < circle_max_line; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{

			// Noneマシンを追加
			m_AMObject.push_back(m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE));

			// 初期化処理
			m_AMObject[counter]->Initialize();

			// 存在するライン決め
			m_AMObject[counter]->SetLine(i);
			m_AMObject[counter]->SummonAM(SetVelocityCircle(j, CIRCLE_MAX_MIN * i, i * CIRCLE_LINE_DISTANCE));
			m_AMObject[counter]->SetActive(false);
			m_dorpShadow->CreateShadow(m_AMObject[counter]->GetData().pos);

			counter++;

		}
	}

}

void AlchemicalMachineManager::LvToObjectActives(int lineNumber)
{
	for (int i = 0; i < m_AMObject.size(); i++)
	{

		if (m_AMObject[i]->GetModelID() != MACHINE_TYPE::NONE) continue;

		if (m_AMObject[i]->GetLine() <= lineNumber)
		{
			m_AMObject[i]->SetActive(true);
		}
		else
		{
			m_AMObject[i]->SetActive(false);
		}

	}
}

void AlchemicalMachineManager::SpawnAMMachine(bool leftButtom)
{

	//　====================[　マシンの召喚処理　]
	//　　|=>　 選択ボックスUIに触れていない
	//　　|=>　 対象オブジェクトに触れている
	//　　|=>　 説明UIに触れていない
	//　　|=>　 左ボタンを離すとオブジェクトを入れ替える
	if (leftButtom)
	{
		// MachineType::Noneを選択している場合に限り処理を通す
		if (m_AMObject[m_selectNumber]->GetModelID() != MACHINE_TYPE::NONE) return;

		// 召喚できるオブジェクトの数が足りない場合は召喚しない
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0) return;

		// 位置情報を取得
		SimpleMath::Vector3 savePos = m_AMObject[m_selectNumber]->GetData().pos;

		// ライン情報を取得
		int saveLine = m_AMObject[m_selectNumber]->GetLine();

		// 本取得
		m_AMObject[m_selectNumber] = m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType());

		// 初期化
		m_AMObject[m_selectNumber]->Initialize();

		// Noneと対象のオブジェクトを入れ替える
		m_AMObject[m_selectNumber]->SummonAM(savePos);

		// ライン情報を引き継ぐ
		m_AMObject[m_selectNumber]->SetLine(saveLine);

		// 召喚したオブジェクトの保有数を一つ減らす
		m_AMnums[m_selectManager->GetSelectMachineType()]--;
		
		// 召喚したマシンのIDを取得
		m_spawnMachine = m_AMObject[m_selectNumber]->GetModelID();

		// 召喚用のパーティクルを出現させる
		m_particle_Put->OnShot(m_AMObject[m_selectNumber]->GetPos(),true, m_AMObject[m_selectNumber]->GetColor());

		// 召喚用の音声を鳴らす
		SoundData::GetInstance().PlaySystemSE(XACT_WAVEBANK_SYSTEMSE_MACHINESPAWN);


	}

}

void AlchemicalMachineManager::Dismantling(int index)
{

	// 位置情報を取得
	SimpleMath::Vector3 savePos = m_AMObject[index]->GetData().pos;

	// ライン情報を取得
	int saveLine = m_AMObject[index]->GetLine();

	// 通知を行うため、破壊時通知用変数にIDを入れる
	m_destroyMachine = m_AMObject[index]->GetModelID();

	// 本取得
	m_AMObject[index] = m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE);

	// 初期化
	m_AMObject[index]->Initialize();

	// Noneと対象のオブジェクトを入れ替える
	m_AMObject[index]->SummonAM(savePos);

	// ライン情報を引き継ぐ
	m_AMObject[index]->SetLine(saveLine);

}

void AlchemicalMachineManager::JsonLoadResources()
{
	auto pSJD = &ShareJsonData::GetInstance();

	Stage_Resource resource = pSJD->GetStageData().resource;

	m_AMnums[MACHINE_TYPE::ATTACKER]	= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		= resource.mining;

	for (int i = 0; i < pSJD->GetStageData().machine.size(); i++)
	{
		// 位置情報を取得
		SimpleMath::Vector3 savePos = m_AMObject[i]->GetData().pos;

		// ライン情報を取得
		int saveLine = m_AMObject[i]->GetLine();

		// 本取得
		m_AMObject[i] = m_AMFilter->HandOverAMClass(pSJD->GetStageData().machine[i].type);

		// 初期化
		m_AMObject[i]->Initialize();

		// 属性設定
		m_AMObject[i]->SetElement(pSJD->GetStageData().machine[i].element);
		m_AMObject[i]->SetColor(Json::ChangeColor(pSJD->GetStageData().machine[i].element));

		// 属性設定
		m_AMObject[i]->SetLv(pSJD->GetStageData().machine[i].lv);

		// Noneと対象のオブジェクトを入れ替える
		m_AMObject[i]->SummonAM(savePos);

		// ライン情報を引き継ぐ
		m_AMObject[i]->SetLine(saveLine);

	}
}
