#include "pch.h"
#include "OperationInstructions.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"

#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/Commons/DrawVariableNum.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

#include "NecromaLib/GameData/ScreenToWorld.h"

// UI���w���ɂ�����K�v�ƂȂ�N���X
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/PlayScene/AlchemicalMachine/AM_Attacker.h"
#include "Scenes/SelectScene/MissionRender.h"
#include "Scenes/PlayScene/Shadow/Particle.h"

#define EXPLANATION_BUTTON		SimpleMath::Vector2(1220.0f,64.0f)
#define TUTORIAL_EXIT_BUTTON	SimpleMath::Vector2(1280.0f / 1.05f,720.0f / 1.9f)

#define TUTORIAL_TEXT_POS SimpleMath::Vector2(90.0f,120.0f)

#define MACHINE_EXPLANATION 5

#define TITLESCENE_BUTTON SimpleMath::Vector2(1.4f,3.3f)
#define SELECTSCENE_BUTTON SimpleMath::Vector2(1.7f,3.3f)

// ����X���W�w��
#define ARROW_POS_X 350
// �������C���̑傫��
#define LINE_RAGE SimpleMath::Vector2(50.0f,50.0f)

const std::vector<const wchar_t*> OperationInstructions::FILENAME =
{
	{L"Resources/Textures/Explanation/None.png"},			//  0.None
	{L"Resources/Textures/Explanation/Attacker.png"},		//  1.Attacker
	{L"Resources/Textures/Explanation/Defencer.png"},		//  2.Defencer
	{L"Resources/Textures/Explanation/Upper.png"},			//  3.Upper
	{L"Resources/Textures/Explanation/Recovery.png"},		//  4.Recovery
	{L"Resources/Textures/Explanation/Excavator.png"},		//  5.Excavator
	{L"Resources/Textures/Explanation/MachineList.png"},	//  6.MachineList
	{L"Resources/Textures/Explanation/RotateStop.png"},		//  7.RotateStop
	{L"Resources/Textures/Explanation/MachineMenu.png"},	//  8.MachineMenu
	{L"Resources/Textures/Explanation/Gauge_1.png"},		//  9.HP�Q�[�W
	{L"Resources/Textures/Explanation/Gauge_2.png"},		// 10.MP�Q�[�W
	{L"Resources/Textures/Explanation/Gauge_3.png"},		// 11.�N���X�^���Q�[�W
	{L"Resources/Textures/Explanation/Mission.png"},		// 12.�~�b�V����
	{L"Resources/Textures/Explanation/Alchemical.png"},		// 13.�B���{�^��
	{L"Resources/Textures/Explanation/CameraMove.png"},		// 14.�J�����ړ�
	{L"Resources/Textures/Explanation/Instructions.png"},	// 15.�`���[�g���A��(��������)
	{L"Resources/Textures/Explanation/LineSet.png"},		// 16.���C���ݒ�
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
};

OperationInstructions::OperationInstructions():
	m_explanationFlag(false),
	m_cameraFlag(false),
	m_selectNumber(1),
	m_tutorialNumber(0),
	m_maxSelectVal(),
	m_tutorialTime(0),
	m_particleTime()
{

}

OperationInstructions::~OperationInstructions()
{
}

void OperationInstructions::Initialize(std::vector<Tutorial_Status> tutorialNumber, PlayScene* pPlayScene)
{

	m_showBox = std::make_unique<DrawBox>(SimpleMath::Vector2(), LINE_RAGE, 5.0f);
	m_showBox->SetColor(SimpleMath::Color(1.0f, 1.0f, 0.0f, 1.0f));

	// �ʒu��ǂݍ���
	RelodeTutorial(tutorialNumber,pPlayScene);

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// �擾
	m_arrowL = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) + ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2f + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) - ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2f + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),4);

	m_explanationButton  = std::make_unique<SelectionBox>(TUTORIAL_EXIT_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));
	//m_tutorialExitButton = std::make_unique<SelectionBox>(TUTORIAL_EXIT_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));

	m_titleSceneBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / TITLESCENE_BUTTON.x,height / TITLESCENE_BUTTON.y),SimpleMath::Vector2(2.0f,1.0f));
	m_selectSceneBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / SELECTSCENE_BUTTON.x, height / SELECTSCENE_BUTTON.y), SimpleMath::Vector2(2.0f, 1.0f));

	// �`���[�g���A���\���p �}�V��UI
	m_amMachine = std::make_unique<AM_Attacker>();
	m_amMachine -> Initialize();

	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	CreateInterfase();

	// �����摜�e�N�X�`���̓ǂݍ���
	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[0].type]);

	// �ΏۃI�u�W�F�N�g���w���p�[�e�B�N���N���X
	m_particle_Select = std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Select->Initialize();
	m_particle_Select->SetParticleSpawnTime(1.0f);

	//// �`���[�g���A�������擾
	//m_selectNumber = 0;
	//m_maxSelectVal = (const int)m_tutorialNumber.size() - 1;
	//LinePosSet(pPlayScene, m_tutorialNumber[m_selectNumber]);

}

void OperationInstructions::Update(PlayScene* pPlayScene, bool stopFlag)
{

	m_cameraFlag = stopFlag;

	// �p�[�e�B�N���̃A�b�v�f�[�g����
	m_particle_Select->UpdateParticle();

	// �p�[�e�B�N���o���̃J�E���g
	m_particleTime += DeltaTime::GetInstance().GetDeltaTime();

	if(!m_cameraFlag) m_showBox->ResetAnimationData();

	// ���������[�h
	if (m_explanationFlag)
	{
		m_showBox->Update();
		m_showBox->SetAnimationFlag(m_explanationFlag);

		m_arrowL->HitMouse();
		m_arrowR->HitMouse();

		m_titleSceneBox->HitMouse();
		m_selectSceneBox->HitMouse();

		m_maxSelectVal = INSTRUCTION_TYPE::NUM;

		// ���{�^����m_selectNumber����
		if (m_arrowL->ClickMouse())
		{
			m_selectNumber++;
			// ��������ݒ�
			m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// ���������ʒu�����߂�
			LinePosSet(pPlayScene,m_selectNumber);

			// �A�j���[�V�����l��0�ɖ߂�
			m_showBox->ResetAnimationData();
		}

		// �E�{�^����m_selectNumber����
		if (m_arrowR->ClickMouse())
		{

			m_selectNumber--;
			// ��������ݒ�
			m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// ���������ʒu�����߂�
			LinePosSet(pPlayScene,m_selectNumber);

			// �A�j���[�V�����l��0�ɖ߂�
			m_showBox->ResetAnimationData();
		}

	}

	//// �`���[�g���A��������
	//if (m_tutorialFlag && m_cameraFlag)
	//{		
	//
	//	m_showBox->SetAnimationFlag(true);
	//
	//	// �w���Ă���ꏊ���N���b�N���� �`���[�g���A���ł���Ƃ�
	//	if (m_showBox->ClickMouse())
	//	{
	//		NextTutorial(pPlayScene);
	//	}
	//
	//	// None�}�V����I������`���[�g���A���ł���Ƃ�
	//	if (m_tutorialNumber[m_selectNumber] == SPAWN)
	//	{
	//		// None�}�V���̂ǂꂩ���I�����ꂽ
	//		if (pPlayScene->GetAlchemicalMachineManager()->SpawnMachineNotification() != NONE)
	//		{
	//			NextTutorial(pPlayScene);
	//		}
	//
	//		// None�}�V����G��`���[�g���A���ł���Ƃ��@���@�p�[�e�B�N���̃X�|�[���^�C�����K��l�𒴂�����
	//		if (m_particleTime >= 1.0f)
	//		{
	//			NonePosSearch(pPlayScene);
	//			m_particleTime = 0.0f;
	//		}
	//	}
	//
	//	// �S�`���[�g���A�����I�������`���[�g���A�����[�h����������
	//	if (m_selectNumber >= m_maxSelectVal)
	//	{
	//		m_tutorialFlag = false;
	//		m_selectNumber = 0;
	//
	//	}
	//
	//	//m_arrowL->HitMouse();
	//	//m_arrowR->HitMouse();
	//	//
	//	//// �I���\�͈͂̍ő�l���擾
	//	//m_maxSelectVal = (const int)m_tutorialNumber.size() - 1;
	//	//
	//	//// ���{�^����m_selectNumber����
	//	//if (m_arrowL->ClickMouse())
	//	//{
	//	//	m_selectNumber++;
	//	//	// ��������ݒ�
	//	//	m_selectNumber = std::min(std::max(m_selectNumber, 0), m_maxSelectVal);
	//	//
	//	//	// �e�N�X�`����ǂݍ���
	//	//	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[m_selectNumber]]);
	//	//
	//	//	// �A�j���[�V�����l��0�ɖ߂�
	//	//	m_showBox->ResetAnimationData();
	//	//}
	//	//
	//	//// �E�{�^����m_selectNumber����
	//	//if (m_arrowR->ClickMouse())
	//	//{
	//	//
	//	//	m_selectNumber--;
	//	//	// ��������ݒ�
	//	//	m_selectNumber = std::min(std::max(m_selectNumber, 0), m_maxSelectVal);
	//	//
	//	//	// �e�N�X�`����ǂݍ���
	//	//	m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[m_selectNumber]]);
	//	//
	//	//	// �A�j���[�V�����l��0�ɖ߂�
	//	//	m_showBox->ResetAnimationData();
	//	//}
	//	//
	//	//// ���C���������ʒu�����߂�
	//	//LinePosSet(pPlayScene,m_selectNumber);
	//	//
	//	//if (m_selectNumber >= m_maxSelectVal)
	//	//{
	//	//	m_tutorialExitButton->HitMouse();
	//	//	if (m_tutorialExitButton->ClickMouse()) 			m_tutorialFlag = false;
	//	//}
	//
	//}

	// �������[�h�ڍs�{�^��
	m_explanationButton->HitMouse();
	if (m_explanationButton->ClickMouse())
	{
		m_explanationFlag = !m_explanationFlag;

		// ���������ʒu�����߂�
		LinePosSet(pPlayScene,m_selectNumber);
	}

}

void OperationInstructions::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	m_particle_Select->Render();

	if (m_explanationFlag)
	{
		// �w�i��������
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		RECT rect = { 0,0,800,600};
	
		pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);
	
		rect = SpriteCutter(64, 64, SpriteLoder::UIICON_TYPE::STOP, 0);
	
		// UI�̃A�C�R��(��U��~�p)��n���܂�
		pSB->Draw(pSL.GetUIIcons().Get(), SimpleMath::Vector2(width / 2.0f, height / 2.0f),
			&rect,
			SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.8f),
			0.0f,
			DirectX::XMFLOAT2{ 64.0f / 2.0f,64.0f / 2.0f }, 10.0f);
	
		pSD.GetSpriteBatch()->End();
		
		// �}�V��UI�̐����̍ۂɕ`��
		if (m_selectNumber == MACHINE_UI)
		{
			m_machineExplanation->Draw();
			m_amMachine->SetSelectModeFlag(true);
			m_amMachine->SelectRenderUI_Common();
			m_amMachine->RenderUI();
		}

		m_titleSceneBox->DrawUI();
		m_selectSceneBox->DrawUI();

	}
	
	m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::EXPRANATION);

}

void OperationInstructions::Render_Layer2()
{
	ShareData& pSD = ShareData::GetInstance();
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	m_showBox->Draw();
	pSD.GetSpriteBatch()->End();
	
	if ( m_explanationFlag )
	{
		// �e�L�X�g�`��
		m_backFlame->Render();
		m_textTexture->Render();
	
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		// ������̐��Ɩ��UI�`��
		// �Ώۂ𖾊m�ɂ���
		m_showBox->Draw();
	
		// �I���ړ����̕`��@(��������ɒB������`���؂�)
		if (m_selectNumber < m_maxSelectVal)	m_arrowL->Draw();
		if (m_selectNumber > 0)					m_arrowR->Draw();
	
		pSD.GetSpriteBatch()->End();
	
		//// �`���[�g���A�����ɕ`�悷��
		//if ( m_tutorialFlag && m_selectNumber >= (const int)m_tutorialNumber.size() - 1)
		//{
		//	m_tutorialExitButton->DrawUI(SpriteLoder::UIICON_TYPE::CANCEL);
		//}
	
	}

}

void OperationInstructions::Finalize()
{
}

void OperationInstructions::RelodeTutorial(std::vector<Tutorial_Status> tutorialNumber, PlayScene* pPlayScene)
{
	m_selectNumber = 0;
	m_tutorialNumber = tutorialNumber;
	
	// �I���\�͈͂̍ő�l���擾
	m_maxSelectVal = (const int)m_tutorialNumber.size();
	
	//UserInterFace����������Ă���Ȃ�΍ēx�摜��ǂݍ���
	if(m_textTexture) m_textTexture->LoadTexture(FILENAME[m_tutorialNumber[0].type]);
	
	// ���������ʒu�����߂�
	LinePosSet(pPlayScene, m_tutorialNumber[0].type);

}

void OperationInstructions::CreateInterfase()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	// �e�N�X�`����`�悷��ۂ̈ʒu���Ƒ傫��
	SimpleMath::Vector2 pos{ width / 4.0f + TUTORIAL_TEXT_POS.x,100.0f + TUTORIAL_TEXT_POS.y};
	SimpleMath::Vector2 rage{ 0.5f,0.5f};

	m_textTexture = std::make_unique<UserInterface>();
	m_backFlame = std::make_unique<UserInterface>();

	m_textTexture->Create(device, L"Resources/Textures/Explanation/Attacker.png", pos, rage,UserInterface::TOP_LEFT);
	m_textTexture->SetWindowSize((const int)width,(const int)height);

	pos = { width / 2 + TUTORIAL_TEXT_POS.x,height / 3 + TUTORIAL_TEXT_POS.y};
	rage = { 0.75f,0.6f };

	m_backFlame->Create(device, L"Resources/Textures/Explanation/BackFlame.png", pos, rage, UserInterface::MIDDLE_CENTER);
	m_backFlame->SetWindowSize((const int)width, (const int)height);
	m_backFlame->SetColor(SimpleMath::Color(0.8f, 0.8f, 1.0f, 0.95f));

	pos = { width / 2.0f,height / 2.4f};
	rage = { 100.0f,100.0f };

}

void OperationInstructions::LinePosSet(PlayScene* pPlayScene, int number)
{

	SimpleMath::Vector2 linePos = SimpleMath::Vector2(-100,-100);
	SimpleMath::Vector2 lineRage = LINE_RAGE;

	// �}�V���}�l�[�W���[�N���X�𓾂�
	AlchemicalMachineManager* machineManager = pPlayScene->GetAlchemicalMachineManager();
	// �}�V���Z���N�g�N���X�𓾂�
	MachineSelect* machineSelect = machineManager->GetMachineSelect()->get()->GetMachineSelect(MACHINE_TYPE::ATTACKER);
	machineSelect->SetHitMouseFlag(false);
	// �Q�[�W�}�l�[�W���[�N���X�𓾂�
	Gauge* gauge = pPlayScene->GetGauge();
	// �~�b�V�����}�l�[�W���[�N���X�𓾂�
	MissionRender* missionRender = pPlayScene->GetMissionManager()->GetMissionRender()->get();
	// ����⏕�N���X�𓾂�
	Explanation* explanation = pPlayScene->GetExplanation();
	explanation;
	//// �`���[�g���A����ʎ��͏�ɖ��I���Ƃ���B
	//machineManager->SetSelectMachineNumber(-1);

	// �{�^����������1F�̊Ԃɂ̂ݔ�������ׁA�����ł͕s��Ƃ���
	switch ((INSTRUCTION_TYPE)number)
	{
	case INSTRUCTION_TYPE::NONE:
		break;

	case INSTRUCTION_TYPE::ATTACKER:
	case INSTRUCTION_TYPE::DEFENSER:
	case INSTRUCTION_TYPE::UPPER:
	case INSTRUCTION_TYPE::RECOVERY:
	case INSTRUCTION_TYPE::MINING:
	{

		linePos = machineManager->GetMachineSelect()->get()->GetMachineSelect(number)->GetData().pos;
		lineRage = LINE_RAGE;

		break;
	}
	case INSTRUCTION_TYPE::OPERATION:
	{
		linePos = SimpleMath::Vector2(892.0f, 64.0f);
		lineRage = SimpleMath::Vector2(300.0f, 50.0f);

		break;
	}
	case INSTRUCTION_TYPE::OPERATION_MACHINE:
	{
		//linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);

		break;
	}
	case INSTRUCTION_TYPE::MACHINE_UI:
	{
		m_amMachine->SetSelectModeFlag(true);
		linePos = m_machineExplanation->GetPos();
		lineRage = m_machineExplanation->GetRage();
		break;
	}
	case INSTRUCTION_TYPE::GAUGE_HP:
	{
		linePos = gauge->GetHPGaugePos();
		lineRage = SimpleMath::Vector2(240.0f, 19.0f);

		break;
	}
	case INSTRUCTION_TYPE::GAUGE_MP:
	{
		linePos = gauge->GetMPGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 14.0f);

		break;
	}
	case INSTRUCTION_TYPE::GAUGE_CRYSTAL:
	{
		linePos = gauge->GetCrystalGaugePos();
		lineRage = SimpleMath::Vector2(180.0f, 14.0f);

		break;
	}
	case INSTRUCTION_TYPE::MISSION:
	{

		linePos = missionRender->GetPos();
		lineRage = missionRender->GetRage();

		break;
	}
	case INSTRUCTION_TYPE::ALCHEMI:
	{
		//linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);
		break;
	}
	case INSTRUCTION_TYPE::SPAWN:
	{
		// NoneMachine�̈ʒu��T���o���A�G�t�F�N�g�𔭐������܂�
		NonePosSearch(pPlayScene);
		break;
	}
	case INSTRUCTION_TYPE::LINE_SELECT:
	{
		linePos = machineManager->GetVariableNum()->get()->GetSelectionBox()->GetPos();
		lineRage = SimpleMath::Vector2(100.0f, 36.0f);
		break;
	}
	default:
		break;
	}

	m_showBox->ResetAnimationData();
	m_showBox->SetPosRage(linePos,lineRage);
}

void OperationInstructions::NextTutorial(PlayScene* pPlayScene)
{
	// �Ώە����N���b�N���ꂽ�玟�̃`���[�g���A���Ɍ�����
	m_selectNumber++;
	// ����ݒ�
	m_selectNumber = std::min(std::max(m_selectNumber, 0), (int)m_tutorialNumber.size() - 1);
	LinePosSet(pPlayScene, m_tutorialNumber[m_selectNumber].type);
}

void OperationInstructions::NonePosSearch(PlayScene* pPlayScene)
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);
	SimpleMath::Vector3 none_pos;

	width;
	height;

	for (const auto& none : pPlayScene->GetAlchemicalMachineManager()->GetAlchemicalMachineObject())
	{

		// ��ɏo�Ă��Ȃ��Ȃ�Δ�΂�
		if (!none->GetActive()) continue;

		// ���ꂪNone�Ŗ�����Δ�΂�
		if (none->GetModelID() != MACHINE_TYPE::NONE) continue;

		// �G�t�F�N�g���o��������
		m_particle_Select->OnShot(none->GetPos(), true, SimpleMath::Color(1.0f, 1.0, 0.0f, 0.6f));
	}

}

