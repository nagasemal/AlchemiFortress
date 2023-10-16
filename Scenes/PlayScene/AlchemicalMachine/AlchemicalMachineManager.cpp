#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

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

// 円周の一番小さい場合におけるマシンの最大数
#define CIRCLE_MAX_MIN  4

// 円周の最大ライン
#define CIRCLE_MAX_LINE 9

// ライン同士の間隔
#define CIRCLE_LINE_DISTANCE 5.5f

// 魔法陣出現の高さ(大)
#define MAGICCIRCLE_HEIGHT	0.3f 

AlchemicalMachineManager::AlchemicalMachineManager():
	m_allHitObjectToMouse(),
	m_selectNumber(-1),
	m_prevSelectMachinePos(0,0,0),
	m_mpPulsTimer(),
	m_AMnums{0,0,0,0,0,0},
	m_saveWheelValue(0),
	m_scrollValue(),
	m_rotationStop(),
	m_spawnMachine(MACHINE_TYPE::NONE)
{
}

AlchemicalMachineManager::~AlchemicalMachineManager()
{
}

void AlchemicalMachineManager::Initialize()
{
	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	m_selectManager		= std::make_unique<MachineSelectManager>();
	m_selectManager		->TextuerLoader();
	m_selectManager		->Initialize();

	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateSphere(pSD.GetContext(),0.75f);

	m_dorpShadow		= std::make_unique<DorpShadow>();
	m_dorpShadow		->Initialize();

	m_particle_hit		= std::make_unique<Particle>(Particle::HIT_BULLET);
	m_particle_hit		->Initialize();

	m_particle_Put		= std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Put		->Initialize();

	m_particle_Gurd		= std::make_unique<Particle>(Particle::SPAWN_ENEMY);
	m_particle_Gurd		->Initialize();

	m_particle_Mining	= std::make_unique<Particle>(Particle::MINING_EFFECT);
	m_particle_Mining	->Initialize(L"Crystal");
	m_particle_Mining	->SetParticleSpawnTime(1.0f);

	m_particle_Recovery = std::make_unique<Particle>(Particle::RECOVERY_EFFECT);
	m_particle_Recovery	->Initialize(L"MP");
	m_particle_Recovery	->SetParticleSpawnTime(1.0f);

	m_magicCircle		= std::make_unique<MagicCircle>();
	m_magicCircle		->Initialize();

	m_magicCircle_Field = std::make_unique<MagicCircle>();
	m_magicCircle_Field	->Initialize();

	m_machineNumRender	= std::make_unique<Number>();
	m_machineNumRender	->SetRage({ 0.25f,0.25f });

	CreateAMMachine();
	LvToObjectActives(1);
	JsonLoadResources();

}

void AlchemicalMachineManager::Update(
	FieldObjectManager* fieldManager,
	MousePointer* pMP,
	EnemyManager* enemys,
	MoveCamera* moveCamera)
{

	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

	// 入力関係
	auto mouse		= pINP.GetMouseState();
	auto keyboard	= pINP.GetKeybordState();

	bool leftRelease	= mouse.leftButton == mouse.RELEASED;
	bool leftDrag		= mouse.leftButton == mouse.HELD;

	leftDrag;

	// 回転を止めるフラグ
	if (mouse.rightButton == mouse.PRESSED && !keyboard.GetLastState().LeftShift)
	{
		m_rotationStop = !m_rotationStop;
	}

	// プレイヤー拠点
	auto pPlayerBase = fieldManager->GetPlayerBase();

	// 現在出ているアルケミカルマシンの数
	int amNum = 0;

	// Noneの出ている数
	int amNum_Nomal = 0;

	// Mp追加までの時間計測
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	// 全てのアルケミカルマシンToマウスを判定する
	m_allHitObjectToMouse = false;

	// 設置されていないならばNoneを返す
	m_spawnMachine = MACHINE_TYPE::NONE;

	// セレクトマネージャーのアップデートを回す
	m_selectManager->Update(fieldManager);

	// パーティクルの更新
	Update_Particle();

	// 召喚マシンを選択中ならば選択ラインをホイールで決める
	if(m_selectManager->GetHitMouseToSelectBoxEven())
	{
		Update_None(pPlayerBase->GetBaseLv());
	}

	// 何もないところで左クリックをすると選択状態が解除される
	if (leftRelease && !m_machineExplanation->OnMouse())
	{

		moveCamera->ResetTargetChangeTimer();

		// 選択されたオブジェクトがない場合の処理
		if (m_selectNumber != -1)
		{
			m_prevSelectMachinePos = m_AMObject[m_selectNumber]->GetPos();
			moveCamera->SetSaveTargetProsition(m_AMObject[m_selectNumber]->GetPos());
		}
		else
		{
			m_prevSelectMachinePos = SimpleMath::Vector3();
			moveCamera->SetSaveTargetProsition(SimpleMath::Vector3());
		}

		// 選択状態の解除
		m_selectNumber = -1;

	}

	// 選択状態のオブジェクトがある
	if (m_selectNumber != -1)
	{

		// 注視点移動
		moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);

		// 説明文のアップデート処理を回す
		m_machineExplanation->Update();
		m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());

		// 選択済みのオブジェクトの選択時アップデートを回す
		m_AMObject[m_selectNumber]->SelectUpdate();
		m_AMObject[m_selectNumber]->SelectUpdate_Common();

		// 選択オブジェクトに魔法陣展開
		m_magicCircle->CreateMagicCircle(
			m_AMObject[m_selectNumber]->GetPos(),
			m_AMObject[m_selectNumber]->GetMagicCircle().r,
			m_AMObject[m_selectNumber]->GetColor());

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
		m_magicCircle->DeleteMagicCircle();

		// 注視点移動
		moveCamera->TargetChange(m_prevSelectMachinePos, { 0,0,0 });
	}

	// 丸影を落とす処理
	m_dorpShadow->DeleteShadow();

	//　現在存在するマシンを動かすための処理
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// アルケミカルマシンを中心点を軸に回す
		MovingMachine(i);
		m_AMObject[i]->SetSelectModeFlag(m_selectManager->GetHitMouseToSelectBoxEven());
		m_AMObject[i]->Update_Common();

		// オブジェクトにマウスが入っているかどうか
		if (m_AMObject[i]->GetHitMouse())
		{
			// マシンに当たっている判定を是にする
			m_allHitObjectToMouse = true;

			// クリックで選択状態に移行
			if (leftRelease && m_selectNumber != i)
			{
				// 選択されているマシンのインデックス番号を渡す
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}
		}

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

		// 解体されたことを受け取る
		if (m_AMObject[i]->GetDismantlingFlag())
		{
			Dismantling(i);
		}

		// アルケミカルマシンの個別更新処理
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager,enemys);
		Update_Upper(i, enemys);
	}

	// MP追加処理
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer = 0;
		pDM.SetNowMP(pDM.GetNowMP() + ((int)MPPLUSNUM * (amNum - amNum_Nomal)));

		// 処理順の影響でRecoveryのみここで処理をする
		Update_Recovery(enemys);
	}

	// マシンを召喚する処理
	SpawnAMMachine(leftRelease);

	// 錬金ボタンが押されたら所持数を増やす
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;
	}

	// 離したのでマウスの当たり判定を元の大きさに戻す
	if(leftRelease)  pMP->ReleaseLeftButtom();

	// バレットの更新処理
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		// 子クラスからfalseで消す
		if ((it)->get()->deleteRequest())
		{
			m_particle_hit->OnShot(it->get()->GetPos(),true,it->get()->GetColor());
			it = m_bullets.erase(it);
			if (it == m_bullets.end()) break;
		}
	}

	// 魔法陣展開
	m_magicCircle_Field->CreateMagicCircle(
		SimpleMath::Vector3{ 0,MAGICCIRCLE_HEIGHT,0 },
		pPlayerBase->GetBaseLv() * CIRCLE_LINE_DISTANCE);

	if(!m_rotationStop) m_magicCircle_Field->CreateWorld();
}

void AlchemicalMachineManager::Render()
{

	// 置いた際に出すパーティクル
	m_particle_Put->Render();

	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()));

			m_AMObject[i]->Draw();
		}
	}

	if (m_selectNumber != -1)
	{
		m_magicCircle->CreateWorld();
		m_magicCircle->Render(m_AMObject[m_selectNumber]->GetModelID());
	}

	// 丸影表示
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	m_particle_hit->Render();
	m_particle_Gurd->Render();
	m_particle_Mining->Render();
	m_particle_Recovery->Render();

	// バレットの描画処理
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}

	m_magicCircle_Field->Render(0);

}

void AlchemicalMachineManager::DrawUI()
{
	m_selectManager->MagicCircleRender();

	// オブジェクトセレクトのrenderを呼び出す Noneを省くために1スタート
	for (int i = 1; i < (int)MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((MACHINE_TYPE)i), i,
			m_AMFilter->GetRingModel((MACHINE_TYPE)i));

		// 所持数描画
		m_machineNumRender->SetNumber(m_AMnums[i]);
		m_machineNumRender->SetPosition({580 + 120.0f * i,110.0f});
		m_machineNumRender->Render();

	}

	m_selectManager->RenderUI(m_AMnums);

	// UIの表示 m_selectNumberが-1 = 選択されていない
	if (m_selectNumber != -1)
	{
		// 選択したモデルのIDがNoneなら表示しない
		if (m_AMObject[m_selectNumber]->GetModelID() == MACHINE_TYPE::NONE) return;

		/*===[ 確認用モデルの表示 ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMObject[m_selectNumber].get());

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());
		m_AMObject[m_selectNumber]->SelectRenderUI_Common();

	}
}

void AlchemicalMachineManager::Finalize()
{
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		m_AMObject[i]->Finalize();
		m_AMObject[i].reset();
	}

	m_AMObject.clear();
	m_AMObject.shrink_to_fit();

	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Finalize();
	}

	m_bullets.clear();

	m_selectManager->Finalize();
	m_selectManager.reset();

	m_machineExplanation->Finalize();
	m_machineExplanation.reset();

	m_AMFilter.reset();
	m_testBox.reset();

}

Model* AlchemicalMachineManager::GetSelectModel()
{
	return m_AMFilter->HandOverAMModel(m_selectManager->GetSelectMachineType());
}

void AlchemicalMachineManager::ReloadResource()
{

	auto pSJD = &ShareJsonData::GetInstance();

	Stage_Resource resource = pSJD->GetStageData().resource;

	m_AMnums[MACHINE_TYPE::ATTACKER]	+= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	+= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		+= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	+= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		+= resource.mining;

}

void AlchemicalMachineManager::Update_Particle()
{
	// パーティクル群のアップデート処理
	m_particle_hit			->UpdateParticle();
	m_particle_Put			->UpdateParticle();
	m_particle_Gurd			->UpdateParticle();
	m_particle_Mining		->UpdateParticle();
	m_particle_Recovery		->UpdateParticle();
}

void AlchemicalMachineManager::Update_None(int baseLv)
{
	InputSupport& pINP = InputSupport::GetInstance();
	
	int scrollWheelValue = pINP.GetMouseState().GetLastState().scrollWheelValue / 100;

	// scrollWheelValueが前回の値より大きければm_saveWheelValueを足す。小さければ引く
	if (m_saveWheelValue > scrollWheelValue) m_scrollValue++;
	if (m_saveWheelValue < scrollWheelValue) m_scrollValue--;

	m_saveWheelValue = scrollWheelValue;

	// 上限下限設定
	if (m_scrollValue <= 1)			m_scrollValue = 1;
	if (m_scrollValue >= baseLv)	m_scrollValue = baseLv;

	// 選択するラインを決める
	LvToObjectActives(m_scrollValue);
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
		bool flag = (upperMachineLine + 1 >= attackerMachineLine ||
					 upperMachineLine - 1 >= attackerMachineLine) &&
					 m_AMObject[j]->GetModelID() == MACHINE_TYPE::UPPER &&
					 m_AMObject[j]->GetElement() == m_AMObject[index]->GetElement() &&
					 m_AMObject[j]->GetHP() >= 0;

		if(flag)	 attacker->AllAlchemicalMachine(m_AMObject[j].get());
	}

	// 弾を発射する
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

	defenser->EnemyHit(enemys->GetEnemyData());
}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	mining->AllFieldObject(fieldManager);
	mining->HitEnemy(enemys->GetEnemyData());

	m_particle_Mining->Update(mining->GetPos(), !mining->GetCrystalFlag(),mining->GetColor());

}

void AlchemicalMachineManager::Update_Recovery(EnemyManager* enemys)
{
	DataManager& pDM = *DataManager::GetInstance();

	for (int i = 0; i < m_AMObject.size(); i++)
	{

		if (m_AMObject[i]->GetHP() <= 0) continue;
		if (m_AMObject[i]->GetModelID() != MACHINE_TYPE::RECOVERY) continue;

		AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[i].get());

		recovery->MPPuls(&pDM);
		recovery->HitEnemy(enemys->GetEnemyData());

		// 回収状態をパーティクルで示す
		m_particle_Recovery->OnShot(recovery->GetPos(), true, recovery->GetColor());

	}
}

void AlchemicalMachineManager::Update_Upper(int index,EnemyManager* enemyManager)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::UPPER) return;

	AM_Upper* upper = dynamic_cast<AM_Upper*>(m_AMObject[index].get());

	upper->HitEnemy(enemyManager->GetEnemyData());

}

void AlchemicalMachineManager::MovingMachine(int number)
{

	if (m_rotationStop) return;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0を中心に回転移動
	SimpleMath::Matrix matrix = SimpleMath::Matrix::Identity;

	matrix *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(10.0f * deltaTime));

	// 回転後の座標を代入
	m_AMObject[number]->SetPos(SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

// Lvに応じて置ける場所が増える為、設定をする
void AlchemicalMachineManager::CreateAMMachine()
{
	int counter = 0;

	m_dorpShadow->DeleteShadow();

	for (int i = 1; i < CIRCLE_MAX_LINE; i++)
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

		if (m_AMObject[i]->GetLine() == lineNumber)
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

	// 選択ボックスUIに触れていない
	// 対象オブジェクトに触れている
	// 説明UIに触れていない
	// 左ボタンを離すとオブジェクトを入れ替える
	if (m_allHitObjectToMouse &&
		!m_machineExplanation->OnMouse() &&
		leftButtom)
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

		m_spawnMachine = m_AMObject[m_selectNumber]->GetModelID();

		m_particle_Put->OnShot(m_AMObject[m_selectNumber]->GetPos(),true, m_AMObject[m_selectNumber]->GetColor());

	}

}

void AlchemicalMachineManager::Dismantling(int index)
{

	// 位置情報を取得
	SimpleMath::Vector3 savePos = m_AMObject[index]->GetData().pos;

	// ライン情報を取得
	int saveLine = m_AMObject[index]->GetLine();

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
