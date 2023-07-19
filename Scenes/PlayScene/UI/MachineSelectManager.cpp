#include "pch.h"
#include "MachineSelectManager.h"
#include "Scenes/DataManager.h"
#include <WICTextureLoader.h>

#include "Scenes/PlayScene/UI/DisplayMagicCircle.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#define POS 		DirectX::SimpleMath::Vector2(64, 64)
#define DIRECTION	120.f

MachineSelectManager::MachineSelectManager() :
	m_selectMachineType(AlchemicalMachineObject::MACHINE_TYPE::NONE),
	m_selectBoxAll(false),
	m_manufacturingFlag(false)
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

	m_camera = std::make_unique<Camera>();

	// None���Ȃ����߂�1�X�^�[�g
	for (int i = 1; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i] = std::make_unique<MachineSelect>();

		m_machineSelect[i]->SetMachineType((AlchemicalMachineObject::MACHINE_TYPE)i);
		m_machineSelect[i]->SetPosition({ POS.x + DIRECTION * i , POS.y });

		m_machineSelect[i]->Initialize();

	}

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_displayMagicCircle = std::make_unique<DisplayMagicCircle>();
	m_displayMagicCircle->Create(SpriteLoder::GetInstance().GetMagicCircleTexturePath());
}

void MachineSelectManager::Update(FieldObjectManager* fieldObjectManager)
{

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_selectMachineType = AlchemicalMachineObject::MACHINE_TYPE::NONE;

	// None���Ȃ����߂�1�X�^�[�g
	for (int i = 1; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		// �v�f�̍X�V����
		m_machineSelect[i]->Update();

		// �v�f���琻���{�^���������ꂽ������󂯎��
		if (m_machineSelect[i]->GetManufacturingFlag() &&
			DataManager::GetInstance()->GetNowMP() - ReduceResourceTable((AlchemicalMachineObject::MACHINE_TYPE)i) >= 0)
		{
			m_manufacturingFlag = true;
			ReduceResource((AlchemicalMachineObject::MACHINE_TYPE)i);
		}

		// �����}�V�����I�����ꂽ
		if (m_machineSelect[i]->GetHitMouseFlag())
		{
			m_selectBoxAll = true;
			m_selectMachineType = m_machineSelect[i]->GetMachineType();

			// �I����ԈȊO�̃}�V���̑I����Ԃ�false�ɂ���
			for (int j = 1; j < AlchemicalMachineObject::MACHINE_TYPE::NUM; j++)
			{
				if (j == i) continue;

				m_machineSelect[j]->GetMachineBox()->SetSelectFlag(false);
				m_machineSelect[j]->SetHitMouseFlag(false);
			}
		}
	}

	// �������̃N���X�^�����I������Ă���Ȃ�΁Atype�F�}�C�j���O�̑I���{�b�N�X�̐F��ς���
	m_machineSelect[AlchemicalMachineObject::MACHINE_TYPE::MINING]->
		SetChangeColorFlag(fieldObjectManager->GetCrystalToMouse());

	m_displayMagicCircle->Update();
	m_displayMagicCircle->TransparentUpdate(m_selectBoxAll);
}

void MachineSelectManager::Render()
{
	for (int i = 1; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Draw();
	}
}

// �Ăяo�����ŗv�f���񂵂Ă���
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

	for (int i = 1; i < AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

	m_camera.reset();
	m_boxTextuer.Reset();

}

int MachineSelectManager::ReduceResourceTable(AlchemicalMachineObject::MACHINE_TYPE type)
{
	int reduceResources[AlchemicalMachineObject::MACHINE_TYPE::NUM] =
	{
		0,	// None
		20, // Attacker
		20, // Defenser
		10, // Mining
		10, // Recovery
		50, // Upper
	};

	return reduceResources[type];
}

void MachineSelectManager::ReduceResource(AlchemicalMachineObject::MACHINE_TYPE type)
{
	auto datas = DataManager::GetInstance();
	datas->SetNowMP(datas->GetNowMP() - ReduceResourceTable(type));

}
