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
		// ���擾
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

	// ���ݏo�Ă���A���P�~�J���}�V���̐�
	int amNum = 0;

	// �S�ẴA���P�~�J���}�V��To�}�E�X�𔻒肷��
	m_allHitObjectToMouse = false;

	// �Z���N�g�}�l�[�W���[�̃A�b�v�f�[�g����
	m_selectManager->Update();

	// �����Ȃ��Ƃ���ō��N���b�N������ƑI����Ԃ����������
	if (leftRelease)
	{
		m_selectNumber = -1;
	}

	//�@���ݑ��݂���}�V���𓮂������߂̏���
	for (int i = 0; i < AM_MAXNUM; i++)
	{
		//�@���݂��Ă���Ώ����𑱂��� ���݂��Ȃ��Ȃ��� = �ȍ~���݂��Ȃ��̂ŉ񂷕K�v���Ȃ�
		if (!m_AMObject[i]->GetActiv()) break;

		amNum++;
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToObject(pMP);
		MajicBulletUpdate(i,enemys);


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

		// ToDO::�x�^�ł��R�[�h		�C���K�{
		//�@A�I����B�ω�
		bool powerUp = false;

		if (m_selectManager->GetSelectMachineType() == AlchemicalMachineObject::MACHINE_TYPE::ATTACKER 
		 && m_AMObject[i]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::UPEER)
		{
			// ���ʔ͈͓��Ƀ}�E�X�������Ă���
			if (m_AMObject[i]->OnCollisionEnter_MagicCircle(pMP)) 		powerUp = true;
		}
	}

	// �h���b�O���͔z�u�ӏ������߂�
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// �t�B�[���h��@���N���b�N�������u�ԁ@�I���{�b�N�X�ɐG��Ă��Ȃ��@�ΏۃI�u�W�F�N�g�����̃I�u�W�F�N�g�ɓ����Ă��Ȃ�
	//�@�Ȃ�΃I�u�W�F�N�g���o��
	if (!hitBaseToMouse && !m_allHitObjectToMouse && !m_selectManager->GetHitMouseToSelectBoxEven() && hitFiledToMouse && leftRelease)
	{
		// �{�擾
		m_AMObject[amNum].reset(m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType()));
		// ������
		m_AMObject[amNum]->Initialize();
		// ����
		m_AMObject[amNum]->SummonAM(pINP.GetMousePosWolrd());

		// ���������I�u�W�F�N�g��I����ԂƂ��Ă݂Ȃ�
		m_selectNumber = amNum;

	}

	// �I����Ԃ̃I�u�W�F�N�g������
	if (m_selectNumber != -1)
	{
		// �������̃A�b�v�f�[�g��������
		m_machineExplanation->Update();

		// �I���ς݂̃I�u�W�F�N�g�̓���A�b�v�f�[�g����
		m_AMObject[m_selectNumber]->SelectUpdate();

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
	}

	// �������̂Ń}�E�X�̓����蔻������̑傫���ɖ߂�
	if(leftRelease)  pMP->ReleaseLeftButtom();

	// �o���b�g�̍X�V����
	for (std::list<Bullet>::iterator it = m_bullets->begin(); it != m_bullets->end(); it++)
	{
		it->Update();
		// �q�N���X����false�ŏ���
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
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActiv())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()));
			m_AMObject[i]->Draw();

			if (m_AMObject[i]->GetHitMouse())
			{
				/*===[ �f�o�b�O�����`�� ]===*/
				std::wostringstream oss;
				oss << "object - " << i;
				pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(60.f, 560.f));
			}
		}
	}

	// �o���b�g�̕`�揈��
	for (std::list<Bullet>::iterator it = m_bullets->begin(); it != m_bullets->end(); it++)
	{
		it->Render(m_testBox.get());
	}
}

void AlchemicalMachineManager::DrawUI()
{

	ShareData& pSD = ShareData::GetInstance();

	// �I�u�W�F�N�g�Z���N�g��render���Ăяo��
	for (int i = 0; i < (int)AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((AlchemicalMachineObject::MACHINE_TYPE)i), i);

	}

	// UI�̕\�� m_selectNumber��-1 = �I������Ă��Ȃ�
	if (m_selectNumber != -1)
	{
		/*===[ �m�F�p���f���̕\�� ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()));

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());

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
	// 0,0,0�𒆐S�ɉ�]�ړ�
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(SPEED);

	// ��]��̍��W����
	m_AMObject[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

//void AlchemicalMachineManager::SelectMachins(int num)
//{
//	// TODO:���ǂ����@��͍������� 
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
