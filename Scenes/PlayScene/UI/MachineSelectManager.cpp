#include "pch.h"
#include "MachineSelectManager.h"
#include <WICTextureLoader.h>

#define POS 		DirectX::SimpleMath::Vector2(64, 64)
#define DIRECTION	120.f

MachineSelectManager::MachineSelectManager() :
	m_selectMachineType(AlchemicalMachineObject::MACHINE_TYPE::NONE),
	m_selectBoxAll(false)
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

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i] = std::make_unique<MachineSelect>();
		m_machineSelect[i]->Initialize();

		m_machineSelect[i]->SetMachineType((AlchemicalMachineObject::MACHINE_TYPE)i);
		m_machineSelect[i]->SetPosition({ POS.x + DIRECTION * i , POS.y });
	}

	m_selectBoxAll = false;
}

void MachineSelectManager::Update()
{

	m_selectBoxAll = false;


	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Update();

		if (m_machineSelect[i]->GetHitMouseFlag())
		{
			m_selectBoxAll = true;
			m_selectMachineType = m_machineSelect[i]->GetMachineType();
		}

	}

	//// 選択ボックスに触れていない
	//if (!m_selectBoxAll)
	//{
	//	m_selectMachineType = AlchemicalMachineObject::MACHINE_TYPE::NONE;
	//}

}

void MachineSelectManager::Render()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Draw();
	}

}

// ばぐりそう。 AlchemicalMachinManagerで要素分回している
void MachineSelectManager::ModelRender(DirectX::Model* model,int index)
{

	m_machineSelect[index]->DisplayObject(m_boxTextuer,model, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());

}

void MachineSelectManager::Finalize()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

}
