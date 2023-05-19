#include "pch.h"
#include "MachineSelectManager.h"
#include <WICTextureLoader.h>

MachineSelectManager::MachineSelectManager()
{
}

MachineSelectManager::~MachineSelectManager()
{
}

void MachineSelectManager::TextuerLoader()
{
	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Log.png",
		nullptr,
		m_boxTextuer.ReleaseAndGetAddressOf()
	);
}

void MachineSelectManager::Initialize()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i] = std::make_unique<MachineSelect>();
		m_machineSelect[i]->Initialize();
	}


}

void MachineSelectManager::Update()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Update();
	}

}

void MachineSelectManager::Render()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Draw();
	}

}

// �΂��肻���B
void MachineSelectManager::ModelRender(DirectX::Model model[])
{
	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->DisplayObject(m_boxTextuer,&model[i]);
	}

}

void MachineSelectManager::Finalize()
{

	for (int i = 0; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

}
