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
		// ���擾
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

	// ���ݏo�Ă���A���P�~�J���}�V���̐�
	int amNum = 0;

	// �S�ẴA���P�~�J���}�V��To�}�E�X�𔻒肷��
	m_allHitObjectToMouse = false;

	// �����Ȃ��Ƃ���ō��N���b�N������ƑI����Ԃ����������
	if (leftRelease)
	{
		m_selectNumber = -1;
	}

	//�@���ݑ��݂���}�V���ɂ𓮂������߂̏���
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		//�@���݂��Ă���Ώ����𑱂���
		if (m_AMObject[i]->GetActiv())
		{
			amNum++;
			m_AMObject[i]->Update();
			m_AMObject[i]->HitToObject(pMP);

			MovingMachine(i);

			// �I�u�W�F�N�g�Ƀ}�E�X�������Ă��邩�ǂ���
			if (m_AMObject[i]->GetHitMouse())
			{
				m_allHitObjectToMouse = true;

				// �N���b�N�őI����ԂɈڍs
				if (leftRelease)
				{
					m_selectNumber = i;
					m_machineExplanation->ResetMoveTime();
				}

			}

		}
	}

	// �h���b�O���͔z�u�ӏ������߂�
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// �t�B�[���h��@���N���b�N�������u�ԁ@�ΏۃI�u�W�F�N�g�����̃I�u�W�F�N�g�ɓ����Ă��Ȃ�
	//�@�Ȃ�΃I�u�W�F�N�g���o��
	if (hitFiledToMouse && !hitBaseToMouse && !m_allHitObjectToMouse && leftRelease)
	{
		// �{�擾
		m_AMObject[amNum] = std::make_unique<AM_Attacker>();
		m_AMObject[amNum]->Initialize();

		m_AMObject[amNum]->SummonAM(pINP.GetMousePosWolrd());

		// ���������I�u�W�F�N�g��I����ԂƂ��Ă݂Ȃ�
		m_selectNumber = amNum;
	}

	// �I����Ԃ̃I�u�W�F�N�g������
	if (m_selectNumber != -1)
	{
		// �������̃A�b�v�f�[�g��������
		m_machineExplanation->Update();

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
	}

	// �������̂Ń}�E�X�̓����蔻������̑傫���ɖ߂�
	if(leftRelease)  pMP->ReleaseLeftButtom();


	// �Z���N�g�}�l�[�W���[�̃A�b�v�f�[�g����
	m_selectManager->Update();

}

void AlchemicalMachineManager::Render()
{
	ShareData& pSD = ShareData::GetInstance();

	for (int i = 0; i < AM_MAXNUM; i++)
	{
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActiv())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_Model[m_AMObject[i]->GetModelID()].get());

			if (m_AMObject[i]->GetHitMouse())
			{
				/*===[ �f�o�b�O�����`�� ]===*/
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

	// �I�u�W�F�N�g�Z���N�g��render���Ăяo��
	m_selectManager->ModelRender(m_Model->get());

	// UI�̕\�� m_selectNumber��-1 = �I������Ă��Ȃ�
	if (m_selectNumber != -1)
	{
		/*===[ �m�F�p���f���̕\�� ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_Model[m_AMObject[m_selectNumber]->GetModelID()].get());

		/*===[ �f�[�^�̕\�� ]===*/
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