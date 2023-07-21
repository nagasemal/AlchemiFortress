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

// �~���̈�ԏ������ꍇ�ɂ�����}�V���̍ő吔
#define CIRCLE_MAX_MIN  4

// �~���̍ő僉�C��
#define CIRCLE_MAX_LINE 9

// ���C�����m�̊Ԋu
#define CIRCLE_LINE_DISTANCE 5.5f

// ���@�w�o���̍���(��)
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

	// Mp�ǉ��܂ł̎��Ԍv��
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	// ���ݏo�Ă���A���P�~�J���}�V���̐�
	int amNum = 0;

	// None�̏o�Ă��鐔
	int amNum_Nomal = 0;

	// �S�ẴA���P�~�J���}�V��To�}�E�X�𔻒肷��
	m_allHitObjectToMouse = false;

	// �Z���N�g�}�l�[�W���[�̃A�b�v�f�[�g����
	m_selectManager->Update(fieldManager);

	// �����}�V����I�𒆂Ȃ�ΑI�����C�����z�C�[���Ō��߂�
	if(m_selectManager->GetHitMouseToSelectBoxEven())
	{
		Update_None(pPlayerBase->GetBaseLv());
	}

	// �����Ȃ��Ƃ���ō��N���b�N������ƑI����Ԃ����������
	if (leftRelease && !m_machineExplanation->OnMouse())
	{

		moveCamera->ResetTargetChangeTimer();

		// �I�����ꂽ�I�u�W�F�N�g���Ȃ��ꍇ�̏���
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


	// �I����Ԃ̃I�u�W�F�N�g������
	if (m_selectNumber != -1)
	{

		// �����_�ړ�
		moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);

		//// None��e��
		//if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::NONE) return;

		// �������̃A�b�v�f�[�g��������
		m_machineExplanation->Update();
		m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());

		// �I���ς݂̃I�u�W�F�N�g�̓���A�b�v�f�[�g����
		m_AMObject[m_selectNumber]->SelectUpdate();
		m_AMObject[m_selectNumber]->SelectUpdate_Common();

		// �I���I�u�W�F�N�g�ɖ��@�w�W�J
		m_magicCircle->CreateMagicCircle(
			m_AMObject[m_selectNumber]->GetPos(),
			m_AMObject[m_selectNumber]->GetMagicCircle().r,
			m_AMObject[m_selectNumber]->GetColor());

	}
	else
	{
		m_machineExplanation->ResetMoveTime();
		m_magicCircle->DeleteMagicCircle();

		// �����_�ړ�
		moveCamera->TargetChange(m_prevSelectMachinePos, { 0,0,0 });
	}

	m_dorpShadow->DeleteShadow();

	//�@���ݑ��݂���}�V���𓮂������߂̏���
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// �A���P�~�J���}�V���𒆐S�_�����ɉ�
		MovingMachine(i);
		m_AMObject[i]->SetSelectModeFlag(m_selectManager->GetHitMouseToSelectBoxEven());

		// �I�u�W�F�N�g�Ƀ}�E�X�������Ă��邩�ǂ���
		if (m_AMObject[i]->GetHitMouse())
		{
			m_allHitObjectToMouse = true;

			// �N���b�N�őI����ԂɈڍs
			if (leftRelease && m_selectNumber != i)
			{
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}
		}

		//�@���݂��Ă���Ώ����𑱂���
		if (!m_AMObject[i]->GetActive()) continue;

		m_dorpShadow->CreateShadow(m_AMObject[i]->GetData().pos);

		// �����ɑ��݂��Ă���}�V���̑����𒲂ׂ�
		if (m_AMObject[i]->GetModelID() == AlchemicalMachineObject::NONE) amNum_Nomal++;
		amNum++;

		// �A���P�~�J���}�V���̍X�V����
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToObject(pMP);

		// �A���P�~�J���}�V���̌ʍX�V����
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager);
	}

	// MP�ǉ�����
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer = 0;
		pDM.SetNowMP(pDM.GetNowMP() + ((int)MPPLUSNUM * (amNum - amNum_Nomal)));
		Update_Recovery();
	}

	// �h���b�O���͔z�u�ӏ������߂�
	if (leftDrag)
	{
		pMP->ObjectDragMode();
	}

	// �t�B�[���h��
	// �I���{�b�N�XUI�ɐG��Ă��Ȃ�
	// �ΏۃI�u�W�F�N�g�ɐG��Ă���
	// ����UI�ɐG��Ă��Ȃ�
	//�@�ɍ��{�^���𗣂��ƃI�u�W�F�N�g�����ւ���
	if (m_allHitObjectToMouse &&
		fieldManager->GetFieldObject()->GetHitMouse() &&
		!m_machineExplanation->OnMouse() &&
		leftRelease)
	{
		// MachineType::None��I�����Ă���ꍇ�Ɍ��菈����ʂ�
		if (m_AMObject[m_selectNumber]->GetModelID() != AlchemicalMachineObject::NONE)return;

		// �����ł���I�u�W�F�N�g�̐�������Ȃ��ꍇ�͏������Ȃ�
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0)
		{
			pMP->ReleaseLeftButtom();
			return;
		}

		// �ʒu�����擾
		DirectX::SimpleMath::Vector3 savePos = m_AMObject[m_selectNumber]->GetData().pos;

		// ���C�������擾
		int saveLine = m_AMObject[m_selectNumber]->GetLine();

		// �{�擾
		m_AMObject[m_selectNumber] = m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType());

		// ������
		m_AMObject[m_selectNumber]->Initialize();

		// None�ƑΏۂ̃I�u�W�F�N�g�����ւ���
		m_AMObject[m_selectNumber]->SummonAM(savePos);

		// ���C�����������p��
		m_AMObject[m_selectNumber]->SetLine(saveLine);

		// ���������I�u�W�F�N�g�ۗ̕L��������炷
		m_AMnums[m_selectManager->GetSelectMachineType()]--;
	}

	// �����{�^���������ꂽ�珊�����𑝂₷
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;
	}

	// �I����Ԃ̃I�u�W�F�N�g������
	//if (m_selectNumber != -1)
	//{
	//	// �����_�ړ�
	//	moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);
	//	// None��e��
	//	if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::MACHINE_TYPE::NONE) return;
	//	// �������̃A�b�v�f�[�g��������
	//	m_machineExplanation->Update();
	//	m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());
	//	// �I���ς݂̃I�u�W�F�N�g�̓���A�b�v�f�[�g����
	//	m_AMObject[m_selectNumber]->SelectUpdate();
	//	m_AMObject[m_selectNumber]->SelectUpdate_Common();
	//	// �I���I�u�W�F�N�g�ɖ��@�w�W�J
	//	m_magicCircle->CreateMagicCircle(
	//		m_AMObject[m_selectNumber]->GetPos(),
	//		m_AMObject[m_selectNumber]->GetMagicCircle().r,
	//		m_AMObject[m_selectNumber]->GetColor());
	//}
	//else
	//{
	//	m_machineExplanation->ResetMoveTime();
	//	m_magicCircle->DeleteMagicCircle();
	//	// �����_�ړ�
	//	moveCamera->TargetChange(m_prevSelectMachinePos, { 0,0,0 });
	//}

	// �������̂Ń}�E�X�̓����蔻������̑傫���ɖ߂�
	if(leftRelease)  pMP->ReleaseLeftButtom();

	m_particle_hit->UpdateParticle();

	// �o���b�g�̍X�V����
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		// �q�N���X����false�ŏ���
		if ((it)->get()->deleteRequest())
		{
			m_particle_hit->OnShot(it->get()->GetPos(),true,it->get()->GetColor());
			it = m_bullets.erase(it);
			if (it == m_bullets.end()) break;
		}
	}

	// ���@�w�W�J
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
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActive())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()));
			m_AMObject[i]->Draw();
		}
	}


	// �ۉe�\��
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	m_magicCircle->CreateWorld();
	m_magicCircle->Render();

	// �������������ۂ̃G�t�F�N�g����
	m_particle_hit->Render();

	// �o���b�g�̕`�揈��
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

	// �I�u�W�F�N�g�Z���N�g��render���Ăяo�� None���Ȃ����߂�1�X�^�[�g
	for (int i = 1; i < (int)AlchemicalMachineObject::MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(m_AMFilter->HandOverAMModel((AlchemicalMachineObject::MACHINE_TYPE)i), i,
			m_AMFilter->GetRingModel((AlchemicalMachineObject::MACHINE_TYPE)i));

		// ������
		m_machineNumRender->SetNumber(m_AMnums[i]);
		m_machineNumRender->SetPosition({540 + 120.0f * i,120.0f});
		m_machineNumRender->Render();
	}

	// UI�̕\�� m_selectNumber��-1 = �I������Ă��Ȃ�
	if (m_selectNumber != -1)
	{
		// �I���������f����ID��None�Ȃ�\�����Ȃ�
		if (m_AMObject[m_selectNumber]->GetModelID() == AlchemicalMachineObject::NONE) return;

		/*===[ �m�F�p���f���̕\�� ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
			m_AMObject[m_selectNumber].get());

		m_AMObject[m_selectNumber]->RenderUI(m_selectManager->GetTextuer());

		//*===[ �f�[�^�̕\�� ]===*/
		//std::wostringstream oss;
		//oss << "Type - " << m_AMObject[m_selectNumber]->GetObjectName().c_str();
		//pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(120.f, 320.f));
		//*===[ �f�o�b�O�����`�� ]===*/
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

	// scrollWheelValue���O��̒l���傫�����m_saveWheelValue�𑫂��B��������Έ���
	if (m_saveWheelValue > scrollWheelValue) m_scrollValue++;
	if (m_saveWheelValue < scrollWheelValue) m_scrollValue--;

	m_saveWheelValue = scrollWheelValue;

	// ��������ݒ�
	if (m_scrollValue <= 1)			m_scrollValue = 1;
	if (m_scrollValue >= baseLv)	m_scrollValue = baseLv;

	// �I�����郉�C�������߂�
	LvToObjectActives(m_scrollValue);
}

void AlchemicalMachineManager::Update_Attacker(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != AlchemicalMachineObject::ATTACKER) return;

	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	// ��������}�V��(Upper)�̏���n��
	for (int j = 0; j < m_AMObject.size(); j++)
	{
		int upperMachineLine	= m_AMObject[j]->GetLine();
		int attackerMachineLine = m_AMObject[index]->GetLine();

		// �����������
		bool flag = (upperMachineLine + 1 >= attackerMachineLine ||
					 upperMachineLine - 1 >= attackerMachineLine) &&
					 m_AMObject[j]->GetModelID() == AlchemicalMachineObject::UPEER &&
					 m_AMObject[j]->GetElement() == m_AMObject[index]->GetElement();

		if(flag)	 attacker->AllAlchemicalMachine(m_AMObject[j].get());
	}

	// �e�𔭎˂���
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

	// 0,0,0�𒆐S�ɉ�]�ړ�
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.1f)) * deltaTime;

	// ��]��̍��W����
	m_AMObject[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

// Lv�ɉ����Ēu����ꏊ��������ׁA�ݒ������
void AlchemicalMachineManager::CreateAMMachine()
{
	int counter = 0;

	m_dorpShadow->DeleteShadow();

	for (int i = 1; i < CIRCLE_MAX_LINE; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{
			// None�}�V����ǉ�
			m_AMObject.push_back(m_AMFilter->HandOverAMClass(AlchemicalMachineObject::NONE));

			// ����������
			m_AMObject[counter]->Initialize();

			// ���݂��郉�C������
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
