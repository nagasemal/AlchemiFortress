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

#define TUTORIAL_TEXT_POS SimpleMath::Vector2(200.0f,400.0f)

#define MACHINE_EXPLANATION 5

#define TITLESCENE_BUTTON SimpleMath::Vector2(1.4f,3.8f)
#define SELECTSCENE_BUTTON SimpleMath::Vector2(1.7f,3.8f)

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
	m_maxSelectVal(),
	m_tutorialTime(0)
{

}

OperationInstructions::~OperationInstructions()
{
}

void OperationInstructions::Initialize(std::vector<Tutorial_Status> tutorialNumber, PlayScene* pPlayScene)
{
	pPlayScene;
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// �擾
	m_arrowL = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) + ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2f + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),2);
	m_arrowR = std::make_unique<DrawArrow>(SimpleMath::Vector2((width / 2) - ARROW_POS_X + TUTORIAL_TEXT_POS.x, height / 2.2f + TUTORIAL_TEXT_POS.y), SimpleMath::Vector2(1.0f, 1.0f),4);

	UI_Data uiData = ShareJsonData::GetInstance().GetUIData("OptionExplanation");
	m_explanationButton  = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);

	m_titleSceneBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / TITLESCENE_BUTTON.x,height / TITLESCENE_BUTTON.y),SimpleMath::Vector2(2.0f,1.0f));
	m_selectSceneBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / SELECTSCENE_BUTTON.x, height / SELECTSCENE_BUTTON.y), SimpleMath::Vector2(2.0f, 1.0f));


	CreateInterfase();

	// �����摜�e�N�X�`���̓ǂݍ���
	m_textTexture->LoadTexture(FILENAME[0]);

}

void OperationInstructions::Update(PlayScene* pPlayScene, bool stopFlag)
{
	pPlayScene;
	m_cameraFlag = stopFlag;

	// �������[�h�ڍs�{�^��
	m_explanationButton->HitMouse();

	// �I�����[�h����/�N��
	if (m_explanationButton->ClickMouse())
	{
		m_explanationFlag = !m_explanationFlag;
	}


	// ���������[�h
	if (!m_explanationFlag) return;

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
	}

	// �E�{�^����m_selectNumber����
	if (m_arrowR->ClickMouse())
	{
		m_selectNumber--;
		// ��������ݒ�
		m_selectNumber = std::min(std::max(m_selectNumber, 1), (const int)INSTRUCTION_TYPE::NUM);
		m_textTexture->LoadTexture(FILENAME[m_selectNumber]);
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

	if (m_explanationFlag)
	{

		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		RECT rect = { 0,0,800,600};

		// �w�i���Â�����
		pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{800.0f / 2.0f,600.0f / 2.0f}, 2.0f);
	
		rect = SpriteCutter(64, 64, SpriteLoder::UIICON_TYPE::STOP, 0);
	
		// UI�̃A�C�R��(��U��~�p)��`��
		pSB->Draw(pSL.GetUIIcons().Get(), SimpleMath::Vector2(width / 2.0f, height / 2.0f),
			&rect,
			SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.8f),
			0.0f,
			DirectX::XMFLOAT2{ 64.0f / 2.0f,64.0f / 2.0f }, 10.0f);
	
		pSD.GetSpriteBatch()->End();

		m_titleSceneBox		->DrawUI();
		m_selectSceneBox	->DrawUI();

	}
	
	// �������[�h�{�^���`��
	m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::EXPRANATION);

}

void OperationInstructions::Render_Layer2()
{	
	if ( m_explanationFlag )
	{
		// �e�L�X�g�`��
		m_backFlame		->Render();
		m_textTexture	->Render();
	
		// �I���ړ����̕`��@(��������ɒB������`���؂�)
		if (m_selectNumber < m_maxSelectVal)	m_arrowL->Draw();
		if (m_selectNumber > 0)					m_arrowR->Draw();

	}
}

void OperationInstructions::Finalize()
{
}

void OperationInstructions::CreateInterfase()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	// �e�N�X�`����`�悷��ۂ̈ʒu���Ƒ傫��
	SimpleMath::Vector2 pos{ TUTORIAL_TEXT_POS.x,TUTORIAL_TEXT_POS.y};
	SimpleMath::Vector2 rage{ 0.25f,0.25f};

	m_textTexture	= std::make_unique<UserInterface>();
	m_backFlame		= std::make_unique<UserInterface>();

	m_textTexture->Create(device, L"Resources/Textures/Explanation/Attacker.png", pos, rage,UserInterface::MIDDLE_CENTER);
	m_textTexture->SetWindowSize((const int)width,(const int)height);

	//pos		= { TUTORIAL_TEXT_POS.x,TUTORIAL_TEXT_POS.y};
	//rage	= { 0.35f,0.3f };

	m_backFlame->Create(device, L"Resources/Textures/Explanation/BackFlame.png", pos, rage, UserInterface::MIDDLE_CENTER);
	m_backFlame->SetWindowSize((const int)width, (const int)height);
	m_backFlame->SetColor(SimpleMath::Color(0.8f, 0.8f, 1.0f, 0.95f));
}
