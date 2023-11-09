#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "Scenes/Commons/DrawVariableNum.h"

#include "Scenes/PlayScene/Camera/MoveCamera.h"
#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

#define SPEED 0.0025f

// ���͂��񕜂���Ԋu
#define MPPLUSTIME 2.0f
// ���͂̉񕜗�
#define MPPLUSNUM  1.0f

// �~���̈�ԏ������ꍇ�ɂ�����}�V���̍ő吔
#define CIRCLE_MAX_MIN  4

// �~���̍ő僉�C��
#define CIRCLE_MAX_LINE 9

// ���C�����m�̊Ԋu
#define CIRCLE_LINE_DISTANCE 5.5f

// ���@�w�o���̍���(��)
#define MAGICCIRCLE_HEIGHT	0.3f 

AlchemicalMachineManager::AlchemicalMachineManager() :
	m_allHitObjectToMouse(),
	m_selectNumber(-1),
	m_prevSelectMachinePos(0, 0, 0),
	m_mpPulsTimer(),
	m_AMnums{ 0,0,0,0,0,0 },
	m_saveWheelValue(0),
	m_scrollValue(),
	m_rotationStop(),
	m_crystalPulsVal(),
	m_mpPulsVal(),
	m_spawnMachine		(MACHINE_TYPE::NONE),
	m_destroyMachine	(MACHINE_TYPE::NONE),
	m_recoveryMachine	(MACHINE_TYPE::NONE),
	m_lvUpMachine		(MACHINE_TYPE::NONE)
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

	m_particle_Put		= std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Put		->Initialize();

	m_particle_Gurd		= std::make_unique<Particle>(Particle::DEFENSE_EFFECT);
	m_particle_Gurd		->Initialize();

	m_particle_Mining	= std::make_unique<Particle>(Particle::MINING_EFFECT);
	m_particle_Mining	->Initialize(L"Crystal");
	m_particle_Mining	->SetParticleSpawnTime(1.0f);

	m_particle_Recovery = std::make_unique<Particle>(Particle::RECOVERY_EFFECT);
	m_particle_Recovery	->Initialize(L"MP");
	m_particle_Recovery	->SetParticleSpawnTime(1.0f);

	m_particle_Bullet	= std::make_unique<Particle>(Particle::BULLET_LINE);
	m_particle_Bullet	->Initialize();

	m_magicCircle		= std::make_unique<MagicCircle>();
	m_magicCircle		->Initialize();

	m_magicCircle_Field = std::make_unique<MagicCircle>();
	m_magicCircle_Field	->Initialize();

	m_lineVariable = std::make_unique<DrawVariableNum>(SimpleMath::Vector2(1280 / 1.38f,720 / 1.05f),SimpleMath::Vector2(1.0f,1.0f));

	CreateAMMachine();
	LvToObjectActives(1);
	JsonLoadResources();

}

void AlchemicalMachineManager::Update(
	FieldObjectManager* fieldManager,
	MousePointer* pMP,
	EnemyManager* enemys,
	MoveCamera* moveCamera)
{

	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();

	// ���͊֌W
	auto mouse		= pINP.GetMouseState();
	auto keyboard	= pINP.GetKeybordState();

	bool leftRelease	= pINP.LeftButton_Release();

	// ��]���~�߂�t���O
	if (pINP.RightButton_Press())
	{
		m_rotationStop = !m_rotationStop;
	}

	// �v���C���[���_
	auto pPlayerBase = fieldManager->GetPlayerBase();

	// �I�����C����ύX����
	m_lineVariable->Update(1,pPlayerBase->GetBaseLv());
	LvToObjectActives(m_lineVariable->GetNumber());

	// ���ݏo�Ă���A���P�~�J���}�V���̐�
	int amNum = 0;

	// None�̏o�Ă��鐔
	int amNum_Nomal = 0;

	// MP���񕜂����ʂ����Z�b�g����
	m_mpPulsVal = 0;

	// �N���X�^�����񕜂����ʂ����Z�b�g����
	m_crystalPulsVal = 0;

	// �Z���N�g�}�l�[�W���[�̃A�b�v�f�[�g����
	m_selectManager->Update(fieldManager);
 

	// Mp�ǉ��܂ł̎��Ԍv��
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	// �S�ẴA���P�~�J���}�V��To�}�E�X�𔻒肷��
	m_allHitObjectToMouse = false;

	// �ʒm�n�ϐ��͑S��None��Ԃ�
	m_spawnMachine = m_destroyMachine = m_recoveryMachine = m_lvUpMachine = MACHINE_TYPE::NONE;

	// �p�[�e�B�N���̍X�V
	Update_Particle();

	// �����Ȃ��Ƃ���ō��N���b�N������ƑI����Ԃ����������
	if (leftRelease && !m_machineExplanation->OnMouse())
	{

		moveCamera->ResetTargetChangeTimer();

		// �I�����ꂽ�I�u�W�F�N�g���Ȃ��ꍇ�̏���
		if (m_selectNumber != -1)
		{
			m_prevSelectMachinePos = m_AMObject[m_selectNumber]->GetPos();
			moveCamera->SetSaveTargetProsition(m_AMObject[m_selectNumber]->GetPos());
		}
		else
		{
			m_prevSelectMachinePos = SimpleMath::Vector3();
			moveCamera->SetSaveTargetProsition(SimpleMath::Vector3());
		}

		// �I����Ԃ̉���
		m_selectNumber = -1;

	}

	// �I����Ԃ̃I�u�W�F�N�g������
	if (m_selectNumber != -1)
	{

		// �����_�ړ�
		moveCamera->TargetChange(m_prevSelectMachinePos, m_AMObject[m_selectNumber]->GetData().pos);

		// �������̃A�b�v�f�[�g��������
		m_machineExplanation->Update();
		m_machineExplanation->Update_MachineData(m_AMObject[m_selectNumber].get());

		// �I���ς݂̃I�u�W�F�N�g�̑I�����A�b�v�f�[�g����
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

	// �ۉe�𗎂Ƃ�����
	m_dorpShadow->DeleteShadow();

	//�@���ݑ��݂���}�V���𓮂������߂̏���
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// �A���P�~�J���}�V���𒆐S�_�����ɉ�
		MovingMachine(i);
		m_AMObject[i]->SetSelectModeFlag(m_selectManager->GetHitMouseToSelectBoxEven());
		m_AMObject[i]->Update_Common();

		// �I�u�W�F�N�g�Ƀ}�E�X�������Ă��邩�ǂ���
		if (m_AMObject[i]->GetHitMouse())
		{
			// �}�V���ɓ������Ă��锻��𐥂ɂ���
			m_allHitObjectToMouse = true;

			// �N���b�N�őI����ԂɈڍs
			if (leftRelease && m_selectNumber != i)
			{
				// �I������Ă���}�V���̃C���f�b�N�X�ԍ���n��
				m_selectNumber = i;
				m_machineExplanation->ResetMoveTime();
			}
		}

		//�@���݂��Ă���Ώ����𑱂���
		if (!m_AMObject[i]->GetActive()) continue;

		// �ۉe�̕\���ʒu���߂�
		m_dorpShadow->CreateShadow(m_AMObject[i]->GetData().pos);

		// �����ɑ��݂��Ă���}�V���̑����𒲂ׂ� (None)
		if (m_AMObject[i]->GetModelID() == MACHINE_TYPE::NONE) amNum_Nomal++;
		// �S�̐�
		amNum++;

		// �A���P�~�J���}�V���̍X�V����
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToMouse(pMP);

		// ��̂��ꂽ���Ƃ��󂯎��
		if (m_AMObject[i]->GetDismantlingFlag())
		{
			Dismantling(i);
		}

		// �C�U���ꂽ���Ƃ��󂯎��
		if (m_AMObject[i]->GetRepairFlag())
		{
			m_recoveryMachine	= m_AMObject[i]->GetModelID();
		}

		// LvUp���ꂽ���Ƃ��󂯎��
		if (m_AMObject[i]->GetLvUpFlag())
		{
			m_lvUpMachine		= m_AMObject[i]->GetModelID();
		}

		// MP�ǉ����� (���J�o���[�̎��̂݋@�\)
		if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop && m_AMObject[i]->GetModelID() == MACHINE_TYPE::RECOVERY)
		{
			m_mpPulsVal += (int)m_AMObject[i]->GetMachineEffectValue();
		}

		// �A���P�~�J���}�V���̌ʍX�V����
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager,enemys);
		Update_Upper(i, enemys);
		Update_Recovery(i,enemys);
	}

	// �}�V�����������鏈��
	SpawnAMMachine(leftRelease);

	// ���̓��\�[�X�𑝂₷
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer = 0;
		m_mpPulsVal += ((int)MPPLUSNUM * ((amNum - amNum_Nomal) / 4));
		pDM.SetNowMP(pDM.GetNowMP() + m_mpPulsVal);
	}

	// �N���X�^�����\�[�X�𑝂₷
	pDM.SetNowCrystal(pDM.GetNowCrystal() + m_crystalPulsVal);


	// �B���{�^���������ꂽ�珊�����𑝂₷
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;
	}

	// �������̂Ń}�E�X�̓����蔻������̑傫���ɖ߂�
	if(leftRelease)  pMP->ReleaseLeftButtom();

	// �o���b�g�̍X�V����
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		m_particle_Bullet->Update(it->get()->GetPos(),true,it->get()->GetColor());

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
		SimpleMath::Vector3{ 0,MAGICCIRCLE_HEIGHT,0 },
		pPlayerBase->GetBaseLv() * CIRCLE_LINE_DISTANCE);

	if(!m_rotationStop) m_magicCircle_Field->CreateWorld();
}

void AlchemicalMachineManager::Render()
{

	// �u�����ۂɏo���p�[�e�B�N��
	m_particle_Put->Render();

	// �V���G�b�g�`��p�h���[�R�[��
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActive())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()),true);

		}
	}

	// �ʏ�`��p�h���[�R�[��
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// ���݂��Ă��邩�`�F�b�N
		if (m_AMObject[i]->GetActive())
		{
			// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
				m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()), false);
		
			m_AMObject[i]->Draw();
		
		}

	}

	if (m_selectNumber != -1)
	{
		m_magicCircle->CreateWorld();
		m_magicCircle->Render(m_AMObject[m_selectNumber]->GetModelID());
	}

	// �ۉe�\��
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	m_particle_hit->Render();
	m_particle_Gurd->Render();
	m_particle_Mining->Render();
	m_particle_Recovery->Render();
	m_particle_Bullet->Render();

	// �o���b�g�̕`�揈��
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}

	m_magicCircle_Field->Render(0);

}

void AlchemicalMachineManager::DrawUI()
{
	m_selectManager->MagicCircleRender();

	m_selectManager->Render();

	// �I�u�W�F�N�g�Z���N�g��render���Ăяo�� None���Ȃ����߂�1�X�^�[�g
	for (int i = 1; i < (int)MACHINE_TYPE::NUM; i++)
	{
		m_selectManager->ModelRender(i);
	}

	m_selectManager->RenderUI(m_AMnums);

	// UI�̕\�� m_selectNumber��-1 = �I������Ă��Ȃ�
	if (m_selectNumber != -1)
	{
		// �I���������f����ID��None�Ȃ�\�����Ȃ�
		if (m_AMObject[m_selectNumber]->GetModelID() == MACHINE_TYPE::NONE) return;

		/*===[ �m�F�p���f���̕\�� ]===*/
		m_machineExplanation->Draw();
		m_machineExplanation->DisplayObject(m_AMFilter->HandOverAMModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMFilter->GetRingModel(m_AMObject[m_selectNumber]->GetModelID()),
											m_AMObject[m_selectNumber].get());

		m_AMObject[m_selectNumber]->RenderUI();
		m_AMObject[m_selectNumber]->SelectRenderUI_Common();

	}

	m_lineVariable->Render();

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

Model* AlchemicalMachineManager::GetSelectModel()
{
	return m_AMFilter->HandOverAMModel(m_selectManager->GetSelectMachineType());
}

void AlchemicalMachineManager::ReloadResource()
{

	auto pSJD = &ShareJsonData::GetInstance();

	Stage_Resource resource = pSJD->GetStageData().resource;

	m_AMnums[MACHINE_TYPE::ATTACKER]	+= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	+= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		+= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	+= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		+= resource.mining;

}

void AlchemicalMachineManager::Update_Particle()
{
	// �p�[�e�B�N���Q�̃A�b�v�f�[�g����
	m_particle_hit			->UpdateParticle();
	m_particle_Put			->UpdateParticle();
	m_particle_Gurd			->UpdateParticle();
	m_particle_Mining		->UpdateParticle();
	m_particle_Recovery		->UpdateParticle();
	m_particle_Bullet		->UpdateParticle();
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

}

void AlchemicalMachineManager::Update_Attacker(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::ATTACKER) return;


	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	// ��������}�V��(Upper)�̏���n��
	for (int j = 0; j < m_AMObject.size(); j++)
	{
		int upperMachineLine	= m_AMObject[j]->GetLine();
		int attackerMachineLine = m_AMObject[index]->GetLine();

		// �����������(���C�����}1�̑���������Upper�Ŋ��������Ă���)
		bool flag = (upperMachineLine + 1 >= attackerMachineLine ||
					 upperMachineLine - 1 >= attackerMachineLine) &&
					 m_AMObject[j]->GetModelID() == MACHINE_TYPE::UPPER &&
					 m_AMObject[j]->GetElement() == m_AMObject[index]->GetElement() &&
					 m_AMObject[j]->GetHP() >= 0;

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
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::DEFENSER) return;

	AM_Defenser* defenser = dynamic_cast<AM_Defenser*>(m_AMObject[index].get());

	defenser->EnemyHit(enemys->GetEnemyData());

	m_particle_Gurd->Update(defenser->GetPos(), defenser->CounterAttack(), defenser->GetColor());

}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	m_crystalPulsVal += mining->AllFieldObject(fieldManager);
	mining->HitEnemy(enemys->GetEnemyData());

	m_particle_Mining->Update(mining->GetPos(), !mining->GetCrystalFlag(),mining->GetColor());

}

void AlchemicalMachineManager::Update_Recovery(int index,EnemyManager* enemys)
{
	DataManager& pDM = *DataManager::GetInstance();

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::RECOVERY) return;

	AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[index].get());

	recovery->HitEnemy(enemys->GetEnemyData());

	// �����Ԃ��p�[�e�B�N���Ŏ���
	m_particle_Recovery->OnShot(recovery->GetPos(), m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop, recovery->GetColor());
}

void AlchemicalMachineManager::Update_Upper(int index,EnemyManager* enemyManager)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::UPPER) return;

	AM_Upper* upper = dynamic_cast<AM_Upper*>(m_AMObject[index].get());

	upper->HitEnemy(enemyManager->GetEnemyData());

}

void AlchemicalMachineManager::MovingMachine(int number)
{

	if (m_rotationStop) return;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0�𒆐S�ɉ�]�ړ�
	SimpleMath::Matrix matrix = SimpleMath::Matrix::Identity;

	matrix *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(10.0f * deltaTime));

	// ��]��̍��W����
	m_AMObject[number]->SetPos(SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

// Lv�ɉ����Ēu����ꏊ��������ׁA�ݒ������
void AlchemicalMachineManager::CreateAMMachine()
{
	int counter = 0;

	m_dorpShadow->DeleteShadow();

	int circle_max_line = ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX;

	for (int i = 1; i < circle_max_line; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{

			// None�}�V����ǉ�
			m_AMObject.push_back(m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE));

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

		if (m_AMObject[i]->GetModelID() != MACHINE_TYPE::NONE) continue;

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

void AlchemicalMachineManager::SpawnAMMachine(bool leftButtom)
{

	// �I���{�b�N�XUI�ɐG��Ă��Ȃ�
	// �ΏۃI�u�W�F�N�g�ɐG��Ă���
	// ����UI�ɐG��Ă��Ȃ�
	// ���{�^���𗣂��ƃI�u�W�F�N�g�����ւ���
	if (m_allHitObjectToMouse &&
		!m_machineExplanation->OnMouse() &&
		leftButtom)
	{
		// MachineType::None��I�����Ă���ꍇ�Ɍ��菈����ʂ�
		if (m_AMObject[m_selectNumber]->GetModelID() != MACHINE_TYPE::NONE) return;

		// �����ł���I�u�W�F�N�g�̐�������Ȃ��ꍇ�͏������Ȃ�
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0) return;

		// �ʒu�����擾
		SimpleMath::Vector3 savePos = m_AMObject[m_selectNumber]->GetData().pos;

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

		m_spawnMachine = m_AMObject[m_selectNumber]->GetModelID();

		m_particle_Put->OnShot(m_AMObject[m_selectNumber]->GetPos(),true, m_AMObject[m_selectNumber]->GetColor());

		SoundData::GetInstance().PlaySystemSE(XACT_WAVEBANK_SYSTEMSE_MACHINESPAWN);


	}

}

void AlchemicalMachineManager::Dismantling(int index)
{

	// �ʒu�����擾
	SimpleMath::Vector3 savePos = m_AMObject[index]->GetData().pos;

	// ���C�������擾
	int saveLine = m_AMObject[index]->GetLine();

	// �ʒm���s�����߁A�j�󎞒ʒm�p�ϐ���ID������
	m_destroyMachine = m_AMObject[index]->GetModelID();

	// �{�擾
	m_AMObject[index] = m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE);

	// ������
	m_AMObject[index]->Initialize();

	// None�ƑΏۂ̃I�u�W�F�N�g�����ւ���
	m_AMObject[index]->SummonAM(savePos);

	// ���C�����������p��
	m_AMObject[index]->SetLine(saveLine);

}

void AlchemicalMachineManager::JsonLoadResources()
{
	auto pSJD = &ShareJsonData::GetInstance();

	Stage_Resource resource = pSJD->GetStageData().resource;

	m_AMnums[MACHINE_TYPE::ATTACKER]	= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		= resource.mining;

	for (int i = 0; i < pSJD->GetStageData().machine.size(); i++)
	{
		// �ʒu�����擾
		SimpleMath::Vector3 savePos = m_AMObject[i]->GetData().pos;

		// ���C�������擾
		int saveLine = m_AMObject[i]->GetLine();

		// �{�擾
		m_AMObject[i] = m_AMFilter->HandOverAMClass(pSJD->GetStageData().machine[i].type);

		// ������
		m_AMObject[i]->Initialize();

		// �����ݒ�
		m_AMObject[i]->SetElement(pSJD->GetStageData().machine[i].element);
		m_AMObject[i]->SetColor(Json::ChangeColor(pSJD->GetStageData().machine[i].element));

		// �����ݒ�
		m_AMObject[i]->SetLv(pSJD->GetStageData().machine[i].lv);

		// None�ƑΏۂ̃I�u�W�F�N�g�����ւ���
		m_AMObject[i]->SummonAM(savePos);

		// ���C�����������p��
		m_AMObject[i]->SetLine(saveLine);

	}
}
