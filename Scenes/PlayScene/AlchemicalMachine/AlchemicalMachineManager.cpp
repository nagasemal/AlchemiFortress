#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#define SPEED 0.005f

AlchemicalMachineManager::AlchemicalMachineManager():
	m_allHitObjectToMouse(),
	m_selectNumber(-1)
{
}

AlchemicalMachineManager::~AlchemicalMachineManager()
{
}

//void AlchemicalMachineManager::ModeLoader()
//{
//
//	ShareData& pSD = ShareData::GetInstance();
//
//	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
//	fx->SetDirectory(L"Resources/Models");
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::NONE]		=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Siroma.cmo", *fx);
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER]	=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER]	=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::MINING]		=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::RECOVERY]	=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
//
//	m_Model[AlchemicalMachineObject::MACHINE_TYPE::UPEER]		=	 DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
//}

void AlchemicalMachineManager::Initialize()
{
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		// 仮取得
		m_AMObject[i] = std::make_unique<AM_None>();
		m_AMObject[i]->Initialize();
	}

	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	m_selectManager = std::make_unique<MachineSelectManager>();
	m_selectManager->TextuerLoader();
	m_selectManager->Initialize();

	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateSphere(pSD.GetContext());

	m_bullets = std::make_unique<std::list<Bullet>>();

}

void AlchemicalMachineManager::Update(bool hitFiledToMouse, bool hitBaseToMouse, MousePointer* pMP, std::list<EnemyObject> enemys)
{

	InputSupport& pINP = InputSupport::GetInstance();
	auto mouse = pINP.GetMouseState();
	bool leftRelease	= mouse.leftButton == mouse.RELEASED;
	bool leftDrag		= mouse.leftButton == mouse.HELD;

	// 現在出ているアルケミカルマシンの数
	int amNum = 0;

	// 全てのアルケミカルマシンToマウスを判定する
	m_allHitObjectToMouse = false;

	// セレクトマネージャーのアップデートを回す
	m_selectManager->Update();

	// 何もないところで左クリックをすると選択状態が解除される
	if (leftRelease)
	{
		m_selectNumber = -1;
	}

	//　現在存在するマシンを動かすための処理
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		//　存在していれば処理を続ける 存在しなくなった = 以降存在しないので回す必要もない
		if (!m_AMObject[i]->GetActiv()) break;

		amNum++;
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToObject(pMP);
		MajicBulletUpdate(i,enemys);


		MovingMachine(i);

		// オブジェクトにマウスが入っているかどうか
		if (m_AMObject[i]->GetHitMouse())
		{
			m_allHitObjectToMouse = true;

			// クリックで選択状態に移行
			if (leftRelease)
			{
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}

		}

		// ToDO::ベタ打ちコード		修正必須
		//　A選択時B変化
		bool powerUp = false;

		if (m_selectManager->GetSelectMachineType() == AlchemicalMachineObject::MACHINE_TYPE::ATTACKER 
		 && m_AMObject[i]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::UPEER)
		{
			// 効果範囲内にマウスが入っている
			if (m_AMObject[i]->OnCollisionEnter_MagicCircle(pMP)) 		powerUp = true;
		}
	}

	// ドラッグ中は配置箇所を決める
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// フィールド上　左クリックをした瞬間　選択ボックスに触れていない　対象オブジェクトが他のオブジェクトに入っていない
	//　ならばオブジェクトを出す
	if (!hitBaseToMouse && !m_allHitObjectToMouse && !m_selectManager->GetHitMouseToSelectBoxEven() && hitFiledToMouse && leftRelease)
	{
		// 本取得
		m_AMObject[amNum].reset(m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType()));
		// 初期化
		m_AMObject[amNum]->Initialize();
		// 召喚
		m_AMObject[amNum]->SummonAM(pINP.GetMousePosWolrd());

		// 召喚したオブジェクトを選択状態としてみなす
		m_selectNumber = amNum;

	}

	// 選択状態のオブジェクトがある
	if (m_selectNumber != -1)
	{
		// 説明文のアップデート処理を回す
		m_machineExplanation->Update();

		// 選択済みのオブジェクトの特殊アップデートを回す
		m_AMObject[m_selectNumber]->SelectUpdate();

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
	}

	// 離したのでマウスの当たり判定を元の大きさに戻す
	if(leftRelease)  pMP->ReleaseLeftButtom();

	// バレットの更新処理
	for (std::list<Bullet>::iterator it = m_bullets->begin(); it != m_bullets->end(); it++)
	{
		it->Update();
		// 子クラスからfalseで消す
		if ((it)->deleteRequest())
		{
			it = m_bullets->erase(it);
			if (it == m_bullets->end()) break;
		}
	}


}

void AlchemicalMachineManager::MajicBulletUpdate(int index, std::list<EnemyObject> enemys)
{

	if (m_AMObject[index]->BulletRequest(&enemys))
	{
		m_bullets->push_back(*std::make_unique<Bullet>(m_AMObject[index]->GetBulletData()));
	}

}

void AlchemicalMachineManager::Render()
{
	ShareData& pSD = ShareData::GetInstance();

	for (int i = 0; i < AM_MAXNUM; i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActiv())
		{
			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()));
			m_AMObject[i]->Draw();

			if (m_AMObject[i]->GetHitMouse())
			{
				/*===[ デバッグ文字描画 ]===*/
				std::wostringstream oss;
				oss << "object - " << i;
				pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(60.f, 560.f));
			}
		}
	}

	// バレットの描画処理
	for (std::list<Bullet>::iterator it = m_bullets->begin(); it != m_bullets->end(); it++)
	{
		it->Render(m_testBox.get());
	}
}

void AlchemicalMachineManager::DrawUI()
{

	ShareData& pSD = ShareData::GetInstance();

	// オブジェクトセレクトのrenderを呼び出す
	for (int i = 0; i < (int)AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((AlchemicalMachineObject::MACHINE_TYPE)i), i);

	}

	// UIの表示 m_selectNumberが-1 = 選択されていない
	if (m_selectNumber != -1)
	{
		/*===[ 確認用モデルの表示 ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()));

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());

		/*===[ データの表示 ]===*/
		std::wostringstream oss;
		oss << "Type - " << m_AMObject[m_selectNumber]->GetObjectName().c_str();
		pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(120.f, 320.f));

		return;
	}
}

void AlchemicalMachineManager::Finalize()
{
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		m_AMObject[i]->Finalize();

		m_AMObject[i].reset();

		delete m_AMObject[i].get();
	}

	for (std::list<Bullet>::iterator it = m_bullets->begin(); it != m_bullets->end(); it++)
	{
		it->Finalize();
	}

	m_bullets->clear();

	m_selectManager->Finalize();
	m_selectManager.reset();

	m_machineExplanation->Finalize();
	m_machineExplanation.reset();

	m_AMFilter.reset();
	m_testBox.reset();

}

void AlchemicalMachineManager::MovingMachine(int number)
{
	// 0,0,0を中心に回転移動
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(SPEED);

	// 回転後の座標を代入
	m_AMObject[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

//void AlchemicalMachineManager::SelectMachins(int num)
//{
//	// TODO:より良い方法を模索したい 
//	switch (m_selectManager->GetSelectMachineType())
//	{
//	case AlchemicalMachineObject::MACHINE_TYPE::NONE:
//		m_AMObject[num] = std::make_unique<AM_None>();
//		break;
//
//	case AlchemicalMachineObject::MACHINE_TYPE::ATTACKER:
//		m_AMObject[num] = std::make_unique<AM_Attacker>();
//		break;
//
//	case AlchemicalMachineObject::MACHINE_TYPE::DEFENSER:
//		m_AMObject[num] = std::make_unique<AM_Defenser>();
//		break;
//
//	case AlchemicalMachineObject::MACHINE_TYPE::MINING:
//		m_AMObject[num] = std::make_unique<AM_Mining>();
//		break;
//
//	case AlchemicalMachineObject::MACHINE_TYPE::RECOVERY:
//		m_AMObject[num] = std::make_unique<AM_Recovery>();
//		break;
//
//	case AlchemicalMachineObject::MACHINE_TYPE::UPEER:
//		m_AMObject[num] = std::make_unique<AM_Upper>();
//		break;
//
//	default:
//		break;
//	}
//
//}
