#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#define SPEED 0.01f

AlchemicalMachineManager::AlchemicalMachineManager():
	m_allHitObjectToMouse(),
	m_selectNumber(-1)
{
}

AlchemicalMachineManager::~AlchemicalMachineManager()
{
}

void AlchemicalMachineManager::ModeLoader()
{

	ShareData& pSD = ShareData::GetInstance();

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::NONE]		= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER]	= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER]	= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::MINING]	= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::RECOVERY]	= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_Model[AlchemicalMachineObject::MACHINE_TYPE::UPEER]		= DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());
}

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

}

void AlchemicalMachineManager::Update(bool hitFiledToMouse, bool hitBaseToMouse, MousePointer* pMP)
{

	InputSupport& pINP = InputSupport::GetInstance();
	auto mouse = pINP.GetMouseState();
	bool leftRelease	= mouse.leftButton == mouse.RELEASED;
	bool leftDrag		= mouse.leftButton == mouse.HELD;

	// 現在出ているアルケミカルマシンの数
	int amNum = 0;

	// 全てのアルケミカルマシンToマウスを判定する
	m_allHitObjectToMouse = false;

	// 何もないところで左クリックをすると選択状態が解除される
	if (leftRelease)
	{
		m_selectNumber = -1;
	}

	//　現在存在するマシンにを動かすための処理
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		//　存在していれば処理を続ける
		if (m_AMObject[i]->GetActiv())
		{
			amNum++;
			m_AMObject[i]->Update();
			m_AMObject[i]->HitToObject(pMP);

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

		}
	}

	// ドラッグ中は配置箇所を決める
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// フィールド上　左クリックをした瞬間　対象オブジェクトが他のオブジェクトに入っていない
	//　ならばオブジェクトを出す
	if (hitFiledToMouse && !hitBaseToMouse && !m_allHitObjectToMouse && leftRelease)
	{
		// 本取得
		m_AMObject[amNum] = std::make_unique<AM_Attacker>();
		m_AMObject[amNum]->Initialize();

		m_AMObject[amNum]->SummonAM(pINP.GetMousePosWolrd());

		// 召喚したオブジェクトを選択状態としてみなす
		m_selectNumber = amNum;
	}

	// 選択状態のオブジェクトがある
	if (m_selectNumber != -1)
	{
		// 説明文のアップデート処理を回す
		m_machineExplanation->Update();

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
	}

	// 離したのでマウスの当たり判定を元の大きさに戻す
	if(leftRelease)  pMP->ReleaseLeftButtom();


	// セレクトマネージャーのアップデートを回す
	m_selectManager->Update();

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
			m_AMObject[i]->ModelRender(m_Model[m_AMObject[i]->GetModelID()].get());

			if (m_AMObject[i]->GetHitMouse())
			{
				/*===[ デバッグ文字描画 ]===*/
				std::wostringstream oss;
				oss << "object - " << i;
				pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(60.f, 560.f));

			}
		}
	}

}

void AlchemicalMachineManager::DrawUI()
{

	ShareData& pSD = ShareData::GetInstance();

	// オブジェクトセレクトのrenderを呼び出す
	m_selectManager->ModelRender(m_Model->get());

	// UIの表示 m_selectNumberが-1 = 選択されていない
	if (m_selectNumber != -1)
	{
		/*===[ 確認用モデルの表示 ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_Model[m_AMObject[m_selectNumber]->GetModelID()].get());

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
	}

	m_selectManager->Finalize();

}

void AlchemicalMachineManager::MovingMachine(int number)
{
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(SPEED);

	m_AMObject[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}
