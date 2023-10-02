#include "pch.h"
#include "Tutorial.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

// UI���w���ɂ�����K�v�ƂȂ�N���X
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/AM_Attacker.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/SelectScene/MissionRender.h"

#define EXPLANATION_BUTTON		SimpleMath::Vector2(1220.0f,64.0f)
#define TUTORIAL_EXIT_BUTTON	SimpleMath::Vector2(1180.0f,380.0f)

#define TUTORIAL_TEXT_POS SimpleMath::Vector2(200.0f,200.0f)

#define MACHINE_EXPLANATION 5

// ����X���W�w��
#define ARROW_POS_X 350
// �������C���̑傫��
#define LINE_RAGE SimpleMath::Vector2(50.0f,50.0f)

const std::vector<const wchar_t*> Tutorial::FILENAME =
{
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/Attacker.png"},
	{L"Resources/Textures/Explanation/Defencer.png"},
	{L"Resources/Textures/Explanation/Upper.png"},
	{L"Resources/Textures/Explanation/Recovery.png"},
	{L"Resources/Textures/Explanation/Excavator.png"},
	{L"Resources/Textures/Explanation/Instructions_1.png"},
	{L"Resources/Textures/Explanation/Instructions_2.png"},
	{L"Resources/Textures/Explanation/Instructions_3.png"},
	{L"Resources/Textures/Explanation/Gauge_1.png"},
	{L"Resources/Textures/Explanation/Gauge_2.png"},
	{L"Resources/Textures/Explanation/Gauge_3.png"},
	{L"Resources/Textures/Explanation/Mission.png"},
	{L"Resources/Textures/Explanation/Instructions_4.png"},
	{L"Resources/Textures/Explanation/Instructions_5.png"},
	{L"Resources/Textures/Explanation/Instructions_6.png"},
	{L"Resources/Textures/Explanation/None.png"},
	{L"Resources/Textures/Explanation/None.png"},
};

Tutorial::Tutorial():
	m_explanationFlag(false),
	m_tutorialFlag(false),
	m_cameraFlag(false),
	m_selectNumber(1),
	m_tutorialNumber(0)
{

}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(int tutorialNumber)
{

	m_selectNumber = tutorialNumber;
	m_tutorialFlag = (bool)m_selectNumber;

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// �擾
	m_arrowL = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) + ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) - ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2 + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),4);

	m_explanationButton = std::make_unique<SelectionBox>(EXPLANATION_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));

	m_tutorialExitButton = std::make_unique<SelectionBox>(TUTORIAL_EXIT_BUTTON, SimpleMath::Vector2(1.0f, 1.0f));

	m_showBox = std::make_unique<DrawBox>(SimpleMath::Vector2(), LINE_RAGE, 5.0f);
	m_showBox->SetColor(SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f));

	// �`���[�g���A���\���p �}�V��UI
	m_amMachine = std::make_unique<AM_Attacker>();
	m_amMachine->Initialize();

	m_machineExplanation = std::make_unique<MachineExplanation>();
	m_machineExplanation->Initialize();

	CreateInterfase();

	m_textTexture->LoadTexture(FILENAME[m_selectNumber]);
}

void Tutorial::Update(AlchemicalMachineManager* machineManager, Gauge* gauge, MissionRender* missionRender, bool stopFlag)
{

	m_cameraFlag = stopFlag;

	m_showBox->Update();
	m_showBox->SetHitMouseFlag( GetTutorialFlag() );


	if(!m_cameraFlag) m_showBox->ResetAnimationData();

	if (m_explanationFlag)
	{
		m_arrowL->HitMouse();
		m_arrowR->HitMouse();

		// ���{�^����m_selectNumber����
		if (m_arrowL->ClickMouse())
		{
			m_selectNumber++;
			// ��������ݒ�
			m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
			m_textTexture->LoadTexture(FILENAME[m_selectNumber]);

			// ���������ʒu�����߂�
			LinePosSet(machineManager, gauge, missionRender, m_selectNumber);

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
			LinePosSet(machineManager, gauge, missionRender, m_selectNumber);

			// �A�j���[�V�����l��0�ɖ߂�
			m_showBox->ResetAnimationData();
		}

	}

	if (m_tutorialFlag && m_cameraFlag)
	{
		// �e�N�X�`���ǂݍ���

		// ���������ʒu�����߂�
		LinePosSet(machineManager, gauge, missionRender, m_selectNumber);

		m_tutorialExitButton->HitMouse();
		if (m_tutorialExitButton->ClickMouse()) 			m_tutorialFlag = false;

	}

	m_explanationButton->HitMouse();
	if (m_explanationButton->ClickMouse())
	{
		m_explanationFlag = !m_explanationFlag;

		// ���������ʒu�����߂�
		LinePosSet(machineManager, gauge, missionRender,m_selectNumber);
	}

}

void Tutorial::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	if ( GetTutorialFlag() )
	{
		// �w�i��������
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		RECT rect = { 0,0,800,600};

		// �w�i����������
		pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);

		pSD.GetSpriteBatch()->End();
		
		// �}�V��UI�̐����̍ۂɕ`��
		if ( m_selectNumber == MACHINE_UI )
		{
			m_machineExplanation		->Draw();
			m_amMachine					->SelectRenderUI_Common();
			m_amMachine					->RenderUI(pSL.GetSelectBoxTexture());
		}

	}

	m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::EXPRANATION);
}

void Tutorial::Render_Layer2()
{

	ShareData& pSD = ShareData::GetInstance();

	if ( GetTutorialFlag() )
	{
		// �e�L�X�g�`��
		m_backFlame->Render();
		m_textTexture->Render();

		// ������̐��Ɩ��UI�`��
		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		//�@�Ώۂ𖾊m�ɂ���
		m_showBox->Draw();

		// �`���[�g���A�����ɂ͕`�悵�Ȃ�
		if ( !m_tutorialFlag )
		{
			// �I���ړ����̕`��@(��������ɒB������`���؂�)
			if (m_selectNumber < INSTRUCTION_TYPE::NUM)m_arrowL->Draw();
			if (m_selectNumber > 0)					   m_arrowR->Draw();
		}

		pSD.GetSpriteBatch()->End();

		// �`���[�g���A�����ɕ`�悷��
		if ( m_tutorialFlag )
		{
			m_tutorialExitButton->DrawUI(SpriteLoder::UIICON_TYPE::CANCEL);
		}

	}

}

void Tutorial::Finalize()
{
}

void Tutorial::CreateInterfase()
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

	pos = { width / 2.0f,height / 2.0f};
	rage = { 100.0f,100.0f };

}

void Tutorial::LinePosSet(AlchemicalMachineManager* machineManager, Gauge* gauge, MissionRender* missionRender, int number)
{

	SimpleMath::Vector2 linePos = SimpleMath::Vector2();
	SimpleMath::Vector2 lineRage = LINE_RAGE;

	MachineSelect* machineSelect = machineManager->GetMachineSelect()->get()->GetMachineSelect(MACHINE_TYPE::ATTACKER);
	machineSelect->SetHitMouseFlag(false);

	// �`���[�g���A����ʎ��͏�ɖ��I���Ƃ���B
	machineManager->SetSelectMachineNumber(-1);

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
		machineSelect->SetHitMouseFlag(true);
		linePos = machineSelect->GetManufacturingBox()->GetPos();
		lineRage = SimpleMath::Vector2(35.0f, 35.0f);

		break;
	}
	case INSTRUCTION_TYPE::MACHINE_UI:
	{
		linePos = m_machineExplanation->GetPos();
		lineRage = m_machineExplanation->GetRage();
		break;
	}
	case INSTRUCTION_TYPE::GAUGE_HP:
	{
		linePos = gauge->GetHPGaugePos();
		lineRage = SimpleMath::Vector2(280.0f, 19.0f);

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
		break;
	case INSTRUCTION_TYPE::SPAWN:
		break;
	default:
		break;
	}

	m_showBox->SetPos(linePos);
	m_showBox->SetRage(lineRage);

}

