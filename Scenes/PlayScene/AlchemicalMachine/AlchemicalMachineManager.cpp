#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
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

#define MPPLUSTIME 1.0f
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
	m_AMnums{1,1,1,1,1,4},
	m_saveWheelValue(0),
	m_scrollValue(),
	m_rotationStop()
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

	m_magicCircle		= std::make_unique<MagicCircle>();
	m_magicCircle		->Initialize();

	m_magicCircle_Field = std::make_unique<MagicCircle>();
	m_magicCircle_Field	->Initialize();

	m_machineNumRender = std::make_unique<Number>();
	m_machineNumRender->SetRage({ 0.25f,0.25f });

	CreateAMMachine();
	LvToObjectActives(1);

}

void AlchemicalMachineManager::Update(FieldObjectManager* fieldManager, MousePointer* pMP, EnemyManager* enemys, MoveCamera* moveCamera)
{

	auto pPlayerBase = fieldManager->GetPlayerBase();

	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

	auto mouse = pINP.GetMouseState();
	auto keyboard = pINP.GetKeybordState();

	bool leftRelease	= mouse.leftButton == mouse.RELEASED;
	bool leftDrag		= mouse.leftButton == mouse.HELD;

	m_rotationStop		= mouse.rightButton == mouse.HELD && !keyboard.GetLastState().LeftShift;

	// Mp追加までの時間計測
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	// 現在出ているアルケミカルマシンの数
	int amNum = 0;

	// Noneの出ている数
	int amNum_Nomal = 0;

	// 全てのアルケミカルマシンToマウスを判定する
	m_allHitObjectToMouse = false;

	// セレクトマネージャーのアップデートを回す
	m_selectManager->Update(fieldManager);

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
		}
		else
		{
			m_prevSelectMachinePos = DirectX::SimpleMath::Vector3();
		}

		m_selectNumber = -1;

	}


	// 選択状態のオブジェクトがある
	if (m_selectNumber != -1)
	{

		// 注視点移動
		moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);

		//// Noneを弾く
		//if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::NONE) return;

		// 説明文のアップデート処理を回す
		m_machineExplanation->Update();
		m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());

		// 選択済みのオブジェクトの特殊アップデートを回す
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

	m_dorpShadow->DeleteShadow();

	//　現在存在するマシンを動かすための処理
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// アルケミカルマシンを中心点を軸に回す
		MovingMachine(i);
		m_AMObject[i]->SetSelectModeFlag(m_selectManager->GetHitMouseToSelectBoxEven());

		// オブジェクトにマウスが入っているかどうか
		if (m_AMObject[i]->GetHitMouse())
		{
			m_allHitObjectToMouse = true;

			// クリックで選択状態に移行
			if (leftRelease && m_selectNumber != i)
			{
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}
		}

		//　存在していれば処理を続ける
		if (!m_AMObject[i]->GetActive()) continue;

		m_dorpShadow->CreateShadow(m_AMObject[i]->GetData().pos);

		// 現状場に存在しているマシンの総数を調べる
		if (m_AMObject[i]->GetModelID() == AlchemicalMachineObject::NONE) amNum_Nomal++;
		amNum++;

		// アルケミカルマシンの更新処理
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToObject(pMP);

		// アルケミカルマシンの個別更新処理
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager);
	}

	// MP追加処理
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer = 0;
		pDM.SetNowMP(pDM.GetNowMP() + ((int)MPPLUSNUM * (amNum - amNum_Nomal)));
		Update_Recovery();
	}

	// ドラッグ中は配置箇所を決める
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// フィールド上
	// 選択ボックスUIに触れていない
	// 対象オブジェクトに触れている
	// 説明UIに触れていない
	//　に左ボタンを離すとオブジェクトを入れ替える
	if (m_allHitObjectToMouse &&
		fieldManager->GetFieldObject()->GetHitMouse() &&
		!m_machineExplanation->OnMouse() &&
		leftRelease)
	{
		// MachineType::Noneを選択している場合に限り処理を通す
		if (m_AMObject[m_selectNumber]->GetModelID() != AlchemicalMachineObject::NONE)return;

		// 召喚できるオブジェクトの数が足りない場合は召喚しない
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0)
		{
			pMP->ReleaseLeftButtom();
			return;
		}

		// 位置情報を取得
		DirectX::SimpleMath::Vector3 savePos = m_AMObject[m_selectNumber]->GetData().pos;

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
	}

	// 製造ボタンが押されたら所持数を増やす
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;
	}

	// 選択状態のオブジェクトがある
	//if (m_selectNumber != -1)
	//{
	//	// 注視点移動
	//	moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);
	//	// Noneを弾く
	//	if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::NONE) return;
	//	// 説明文のアップデート処理を回す
	//	m_machineExplanation->Update();
	//	m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());
	//	// 選択済みのオブジェクトの特殊アップデートを回す
	//	m_AMObject[m_selectNumber]->SelectUpdate();
	//	m_AMObject[m_selectNumber]->SelectUpdate_Common();
	//	// 選択オブジェクトに魔法陣展開
	//	m_magicCircle->CreateMagicCircle(
	//		m_AMObject[m_selectNumber]->GetPos(),
	//		m_AMObject[m_selectNumber]->GetMagicCircle().r,
	//		m_AMObject[m_selectNumber]->GetColor());
	//}
	//else
	//{
	//	m_machineExplanation->ResetMoveTime();
	//	m_magicCircle->DeleteMagicCircle();
	//	// 注視点移動
	//	moveCamera->TargetChange(m_prevSelectMachinePos, { 0,0,0 });
	//}

	// 離したのでマウスの当たり判定を元の大きさに戻す
	if(leftRelease)  pMP->ReleaseLeftButtom();

	m_particle_hit->UpdateParticle();

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
		DirectX::SimpleMath::Vector3{ 0,MAGICCIRCLE_HEIGHT,0 },
		pPlayerBase->GetBaseLv() * CIRCLE_LINE_DISTANCE);
}

void AlchemicalMachineManager::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

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


	// 丸影表示
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	m_magicCircle->CreateWorld();
	m_magicCircle->Render();

	// 球が当たった際のエフェクト処理
	m_particle_hit->Render();

	// バレットの描画処理
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}

	m_magicCircle_Field->CreateWorld();
	m_magicCircle_Field->Render();
}

void AlchemicalMachineManager::DrawUI()
{
	ShareData& pSD = ShareData::GetInstance();

	m_selectManager->MagicCircleRender();

	// オブジェクトセレクトのrenderを呼び出す Noneを省くために1スタート
	for (int i = 1; i < (int)AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((AlchemicalMachineObject::MACHINE_TYPE)i), i,
			m_AMFilter->GetRingModel((AlchemicalMachineObject::MACHINE_TYPE)i));

		// 所持数
		m_machineNumRender->SetNumber(m_AMnums[i]);
		m_machineNumRender->SetPosition({540 + 120.0f * i,120.0f});
		m_machineNumRender->Render();
	}

	// UIの表示 m_selectNumberが-1 = 選択されていない
	if (m_selectNumber != -1)
	{
		// 選択したモデルのIDがNoneなら表示しない
		if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::NONE) return;

		/*===[ 確認用モデルの表示 ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMObject[m_selectNumber].get());

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());

		//*===[ データの表示 ]===*/
		//std::wostringstream oss;
		//oss << "Type - " << m_AMObject[m_selectNumber]->GetObjectName().c_str();
		//pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(120.f, 320.f));
		//*===[ デバッグ文字描画 ]===*/
		//if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::ATTACKER)
		//{
		//	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get());
		//	std::wostringstream oss2;
		//	oss2 << "Damage - " << attacker->GetBulletStatus().damage;
		//	pSD.GetDebugFont()->AddString(oss2.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 400.f));
		//	std::wostringstream oss3;
		//	oss3 << "Speed - " <<  attacker->GetBulletStatus().speed;
		//	pSD.GetDebugFont()->AddString(oss3.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 420.f));
		//	std::wostringstream oss4;
		//	oss4 << "Life - " << attacker->GetBulletStatus().life;
		//	pSD.GetDebugFont()->AddString(oss4.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 440.f));
		//	std::wostringstream oss5;
		//	oss5 << "Span - " << attacker->GetBulletStatus().span;
		//	pSD.GetDebugFont()->AddString(oss5.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 460.f));
		//	std::wostringstream oss6;
		//	oss6 << "LossMP - " << attacker->GetBulletStatus().lossMp;
		//	pSD.GetDebugFont()->AddString(oss6.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 480.f));
		//	std::wostringstream oss7;
		//	oss7 << "NextCrystal - " << attacker->GetNextLvCrystal();
		//	pSD.GetDebugFont()->AddString(oss7.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 500.f));
		//return;
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
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::ATTACKER) return;

	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	// 判定を取るマシン(Upper)の情報を渡す
	for (int j = 0; j < m_AMObject.size(); j++)
	{
		int upperMachineLine	= m_AMObject[j]->GetLine();
		int attackerMachineLine = m_AMObject[index]->GetLine();

		// 判定を取る条件
		bool flag = (upperMachineLine + 1 >= attackerMachineLine ||
					 upperMachineLine - 1 >= attackerMachineLine) &&
					 m_AMObject[j]->GetModelID() == AlchemicalMachineObject::UPEER &&
					 m_AMObject[j]->GetElement() == m_AMObject[index]->GetElement();

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
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::DEFENSER) return;

	AM_Defenser* defenser = dynamic_cast<AM_Defenser*>(m_AMObject[index].get());

	defenser->EnemyHit(enemys->GetEnemyData());
}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	mining->AllFieldObject(fieldManager);

}

void AlchemicalMachineManager::Update_Recovery()
{
	DataManager& pDM = *DataManager::GetInstance();

	for (int i = 0; i < m_AMObject.size(); i++)
	{

		if (m_AMObject[i]->GetHP() <= 0) return;
		if (m_AMObject[i]->GetModelID() != AlchemicalMachineObject::RECOVERY) return;

		AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[i].get());

		recovery->MPPuls(&pDM);
	}

}

void AlchemicalMachineManager::MovingMachine(int number)
{

	if (m_rotationStop) return;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0を中心に回転移動
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.1f)) * deltaTime;

	// 回転後の座標を代入
	m_AMObject[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

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
			m_AMObject.push_back(m_AMFilter->HandOverAMClass(AlchemicalMachineObject::NONE));

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
		if (m_AMObject[i]->GetModelID() != AlchemicalMachineObject::NONE) continue;

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
