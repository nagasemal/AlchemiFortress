#include "pch.h"
#include "MachineSelectManager.h"
#include "Scenes/DataManager.h"
#include <WICTextureLoader.h>

#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/PlayScene/UI/DisplayMagicCircle.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#define POS 		SimpleMath::Vector2(532, 64)
#define DIRECTION	120.f

#define MACHINE_NAME_POS_SHIFT SimpleMath::Vector2(20, 60)

MachineSelectManager::MachineSelectManager() :
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_selectBoxAll(false),
	m_manufacturingFlag(false),
	m_selectNoneFlag(false)
{
}

MachineSelectManager::~MachineSelectManager()
{
}

void MachineSelectManager::TextuerLoader()
{
	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Log.png",
		nullptr,
		m_boxTextuer.ReleaseAndGetAddressOf()
	);
}

void MachineSelectManager::Initialize()
{

	m_camera = std::make_unique<Camera>();

	// Noneを省くために1スタート
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i] = std::make_unique<MachineSelect>();

		m_machineSelect[i]->SetMachineType((MACHINE_TYPE)i);
		m_machineSelect[i]->SetPosition({ POS.x + DIRECTION * i , POS.y });

		m_machineSelect[i]->Initialize();

	}

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_displayMagicCircle = std::make_unique<DisplayMagicCircle>();

	// マシンの名前描画
	m_machineName = std::make_unique<SelectionBox>(SimpleMath::Vector2(), SimpleMath::Vector2(1.0f, 1.0f));
	m_machineName->Initialize();
}

void MachineSelectManager::Update(FieldObjectManager* fieldObjectManager)
{

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_selectMachineType = MACHINE_TYPE::NONE;

	auto datas = DataManager::GetInstance();
	// jsonから読み取った値を使用する
	auto pSJD = &ShareJsonData::GetInstance();

	auto pINP = &InputSupport::GetInstance();

	// Noneを省くために1スタート
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		// 要素の更新処理
		m_machineSelect[i]->Update();

		// 要素から製造ボタンが押された判定を受け取る リソースが足りない場合は弾く
		m_machineSelect[i]->GetManufacturingBox()->SetActiveFlag(
			datas->GetNowMP() - pSJD->GetMachineData((MACHINE_TYPE)i).alchemi_mp >= 0 &&
			datas->GetNowCrystal() - pSJD->GetMachineData((MACHINE_TYPE)i).alchemi_crystal >= 0);

		if (m_machineSelect[i]->GetManufacturingFlag())
		{
			m_manufacturingFlag = true;
			m_selectMachineType = m_machineSelect[i]->GetMachineType();
			ReduceResource((MACHINE_TYPE)i);
		}

		// 何かマシンが選択された
		if (m_machineSelect[i]->GetHitMouseFlag() || m_machineSelect[i]->GetManufacturingFlag())
		{
			m_selectBoxAll = true;
			m_selectMachineType = m_machineSelect[i]->GetMachineType();

			// 選択状態以外のマシンの選択状態をfalseにする
			for (int j = 1; j < MACHINE_TYPE::NUM; j++)
			{
				if (j == i) continue;

				m_machineSelect[j]->GetMachineBox()->SetSelectFlag(false);
				m_machineSelect[j]->SetHitMouseFlag(false);
			}
		}
	}

	//m_uiKeyControl->Update();

	// 何処かのクリスタルが選択されているならば、type：マイニングの選択ボックスの色を変える
	m_machineSelect[MACHINE_TYPE::MINING]->
		SetChangeColorFlag(fieldObjectManager->GetCrystalToMouse());

	m_displayMagicCircle->Update();
	m_displayMagicCircle->TransparentUpdate(m_selectBoxAll);


	m_machineName->SetPos(pINP->GetMousePosScreen() - MACHINE_NAME_POS_SHIFT);
}

void MachineSelectManager::Render()
{
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Draw();
	}
}

void MachineSelectManager::RenderUI(int machineNum[])
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		 // マウスが置かれているマシンの名前を描画する(設置可能なら灰色,不可能であれば赤色で描画)
		if (m_machineSelect[i]->GetHitMouseFlag())
		{

			SimpleMath::Color name_color = SimpleMath::Color(0.9f, 0.9f, 0.9f, 0.95f);

			if(machineNum[i] <= 0) name_color = SimpleMath::Color(1.0f, 0.0f, 0.0f, 0.95f);

			m_machineName->DrawUI(pSL.GetMachineNameTexture().Get(),
				SpriteCutter(768 / 6,28,i,0),
				nullptr,
				RECT(),
				SimpleMath::Color(),
				name_color);
		}
	}

}

// 呼び出し元で要素分回している
void MachineSelectManager::ModelRender(DirectX::Model* model,int index, DirectX::Model* secondModel)
{

	m_machineSelect[index]->DisplayObject(m_boxTextuer,model, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(),secondModel);
}

void MachineSelectManager::MagicCircleRender()
{
	m_displayMagicCircle->SpritebatchRender();
}

void MachineSelectManager::Finalize()
{

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

	m_camera.reset();
	m_boxTextuer.Reset();

}

void MachineSelectManager::ReduceResource(MACHINE_TYPE type)
{
	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();

	datas->SetNowMP(datas->GetNowMP() - pSJD->GetMachineData(type).alchemi_mp);

	datas->SetNowCrystal(datas->GetNowCrystal() - pSJD->GetMachineData(type).alchemi_crystal);

}
