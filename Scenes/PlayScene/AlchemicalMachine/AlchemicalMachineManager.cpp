#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define SPEED 0.0025f

#define MPPLUSTIME 1.0f
#define MPPLUSNUM  1.0f

AlchemicalMachineManager::AlchemicalMachineManager():
	m_allHitObjectToMouse(),
	m_selectNumber(-1),
	m_mpPulsTimer(),
	m_AMnums()
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

	m_testBox = GeometricPrimitive::CreateSphere(pSD.GetContext(),0.75f);

}

void AlchemicalMachineManager::Update(FieldObjectManager* fieldManager, bool hitBaseToMouse, MousePointer* pMP, std::list<EnemyObject> enemys)
{

	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

	auto mouse = pINP.GetMouseState();
	bool leftRelease	= mouse.leftButton == mouse.RELEASED;
	bool leftDrag		= mouse.leftButton == mouse.HELD;

	// Mp�ǉ��܂ł̎��Ԍv��
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	// ���ݏo�Ă���A���P�~�J���}�V���̐�
	int amNum = 0;

	// �S�ẴA���P�~�J���}�V��To�}�E�X�𔻒肷��
	m_allHitObjectToMouse = false;

	// �Z���N�g�}�l�[�W���[�̃A�b�v�f�[�g����
	m_selectManager->Update();

	// �����Ȃ��Ƃ���ō��N���b�N������ƑI����Ԃ����������
	if (leftRelease && !m_machineExplanation->OnMouse())
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
		MovingMachine(i);

		Update_Attacker(i, enemys);
		Update_Mining(i, fieldManager);

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

	// MP�ǉ�����
	if (m_mpPulsTimer >= MPPLUSTIME)
	{
		m_mpPulsTimer = 0;
		pDM.SetNowMP(pDM.GetNowMP() + (MPPLUSNUM * amNum));
		Update_Recovery();
	}

	// �h���b�O���͔z�u�ӏ������߂�
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// �t�B�[���h��@�I���{�b�N�X�ɐG��Ă��Ȃ��@�����{�b�N�X�ɐG��Ă��Ȃ��@���N���b�N�𗣂����u�ԁ@�ΏۃI�u�W�F�N�g�����̃I�u�W�F�N�g�ɓ����Ă��Ȃ� 
	//�@�Ȃ�΃I�u�W�F�N�g���o���@�@���ǂ����@��͍����Ă�������
	if (!hitBaseToMouse &&
		!m_allHitObjectToMouse &&
		!m_selectManager->GetHitMouseToSelectBoxEven() &&
		fieldManager->GetFieldObject()->GetHitMouse() &&
		leftRelease &&
		!m_machineExplanation->OnMouse())
	{

		// �����ł���I�u�W�F�N�g�̐�������Ȃ��ꍇ�͏������Ȃ�
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0)
		{
			pMP->ReleaseLeftButtom();
			return;
		}

		// �{�擾
		m_AMObject[amNum].reset(m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType()));
		
		// ������
		m_AMObject[amNum]->Initialize();
		
		// ����
		m_AMObject[amNum]->SummonAM(pINP.GetMousePosWolrd());
		
		// ���������I�u�W�F�N�g��I����ԂƂ��Ă݂Ȃ�
		m_selectNumber = amNum;
		
		// ���������I�u�W�F�N�g�ۗ̕L��������炷
		m_AMnums[m_selectManager->GetSelectMachineType()]--;
	}


	// �I����Ԃ̃I�u�W�F�N�g������
	if (m_selectNumber != -1)
	{
		// �������̃A�b�v�f�[�g��������
		m_machineExplanation->Update();

		// �I���ς݂̃I�u�W�F�N�g�̓���A�b�v�f�[�g����
		m_AMObject[m_selectNumber]->SelectUpdate();

		if (mouse.rightButton == mouse.RELEASED)
		{
			m_AMObject[m_selectNumber]->LvUp();
		}


	}
	else
	{
		m_machineExplanation->ResetMoveTime();
	}

	// �������̂Ń}�E�X�̓����蔻������̑傫���ɖ߂�
	if(leftRelease)  pMP->ReleaseLeftButtom();

	// �o���b�g�̍X�V����
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		// �q�N���X����false�ŏ���
		if ((it)->get()->deleteRequest())
		{
			it = m_bullets.erase(it);
			if (it == m_bullets.end()) break;
		}
	}
}

void AlchemicalMachineManager::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

	/*===[ �f�o�b�O�����`�� ]===*/
	std::wostringstream crystalNum;
	crystalNum << "Crystal - " << pDM.GetNowCrystal();
	pSD.GetDebugFont()->AddString(crystalNum.str().c_str(), DirectX::SimpleMath::Vector2(200.f, 60.f));

	std::wostringstream mpNum;
	mpNum << "NowMP - " << pDM.GetNowMP();
	pSD.GetDebugFont()->AddString(mpNum.str().c_str(), DirectX::SimpleMath::Vector2(200.f, 80.f));

	for (int i = 0; i < AM_MAXNUM; i++)
	{
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActiv())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()));
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
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}
}

void AlchemicalMachineManager::DrawUI()
{

	ShareData& pSD = ShareData::GetInstance();

	// �I�u�W�F�N�g�Z���N�g��render���Ăяo��
	for (int i = 0; i < (int)AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((AlchemicalMachineObject::MACHINE_TYPE)i), i,
									 m_AMFilter->GetRingModel((AlchemicalMachineObject::MACHINE_TYPE)i));

	}

	// UI�̕\�� m_selectNumber��-1 = �I������Ă��Ȃ�
	if (m_selectNumber != -1)
	{
		/*===[ �m�F�p���f���̕\�� ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMObject[m_selectNumber].get());

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());

		/*===[ �f�[�^�̕\�� ]===*/
		std::wostringstream oss;
		oss << "Type - " << m_AMObject[m_selectNumber]->GetObjectName().c_str();
		pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(120.f, 320.f));

		/*===[ �f�o�b�O�����`�� ]===*/
		if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::ATTACKER)
		{
			std::wostringstream oss2;
			oss2 << "Damage - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetBulletStatus().damage;
			pSD.GetDebugFont()->AddString(oss2.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 400.f));
			std::wostringstream oss3;
			oss3 << "Speed - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetBulletStatus().speed;
			pSD.GetDebugFont()->AddString(oss3.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 420.f));
			std::wostringstream oss4;
			oss4 << "Life - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetBulletStatus().life;
			pSD.GetDebugFont()->AddString(oss4.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 440.f));
			std::wostringstream oss5;
			oss5 << "Span - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetBulletStatus().span;
			pSD.GetDebugFont()->AddString(oss5.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 460.f));
			std::wostringstream oss6;
			oss6 << "LossMP - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetBulletStatus().lossMp;
			pSD.GetDebugFont()->AddString(oss6.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 480.f));
			std::wostringstream oss7;
			oss7 << "NextCrystal - " << dynamic_cast<AM_Attacker*>(m_AMObject[m_selectNumber].get())->GetNextLvCrystal();
			pSD.GetDebugFont()->AddString(oss7.str().c_str(), DirectX::SimpleMath::Vector2(150.f, 500.f));
		}

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

void AlchemicalMachineManager::Update_Attacker(int index, std::list<EnemyObject> enemys)
{
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::ATTACKER) return;

	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	// ���̃A���P�~�J���}�V���̏���n��
	for (int j = 0; j < AM_MAXNUM; j++)
	{
		attacker->AllAlchemicalMachine(m_AMObject[j].get());
	}

	// �e�𔭎˂���
	if (attacker->BulletRequest(&enemys))
	{
		m_bullets.push_back(std::make_unique<Bullet>(attacker->GetBulletData()));
	}
}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager)
{
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	mining->AllFieldObject(fieldManager);

}

void AlchemicalMachineManager::Update_Recovery()
{
	DataManager& pDM = *DataManager::GetInstance();

	for (int i = 0; i < AM_MAXNUM; i++)
	{

		if (m_AMObject[i]->GetModelID() != AlchemicalMachineObject::RECOVERY) return;

		AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[i].get());

		recovery->MPPuls(&pDM);
	}

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
