#include "pch.h"
#include "TitleScene.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SoundData.h"

#define COLOR SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.55f)
#define MAGIC_CIRCLE_RAGE 30

TitleScene::TitleScene():
	m_nextType(ButtonType::Num),
	m_rotateNowFlag(),
	m_rotateYSpeed()
{
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	ShareJsonData::GetInstance().LoadingJsonFile_GameParameter();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	//�@====================[�@�J�����̐����@]
	m_titleCamera = std::make_unique<TitleCamera>();
	m_titleCamera->Initialize();

	//�@====================[�@���@�w�`��N���X�̐����@]
	m_magicCircle = std::make_unique<MagicCircle>();
	m_magicCircle->Initialize();

	//�@====================[�@�V�����f���𐶐��@]
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	//�@�@|=>�@�G�t�F�N�g��������
	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// ���C�g�̎擾
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// ��������̐F��ύX
			lights->SetAmbientLightColor(SimpleMath::Color(0.5f, 0.7f, 1.f, 0.8f));

		});

	//�@====================[�@�^�C�g�����S�̐ݒ�@]
	m_titleLogo = std::make_unique<TitleLogo>();
	m_titleLogo->Create(pSL.GetTitleLogoPath());
	m_titleLogo->SetWindowSize(width, height);
	m_titleLogo->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.6f, 1.0f));
	m_titleLogo->SetPosition(SimpleMath::Vector2(width / 1.3f, height / 1.2f));

	//�@====================[�@��ʉE���̖��̐ݒ�@]
	m_veil = std::make_unique<Veil>(0);
	m_veil->Create(L"Resources/Textures/TitleText.png");
	m_veil->LoadShaderFile(L"Veil");
	m_veil->SetWindowSize(width, height);
	m_veil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_veil->SetScale(SimpleMath::Vector2(width / 1.5f, (float)height));
	m_veil->SetPosition(SimpleMath::Vector2(width / 1.95f, 0.0f));

	//�@====================[�@�L�[�{�[�h���UI�𑀍삷��N���X�̐����@]
	m_uiKeyControl = std::make_unique<UIKeyControl>();

	//�@====================[�@�Q�[�����ݒ�����߂�N���X�̐����@]
	m_renderOption = std::make_unique<RenderOption>();


	//�@====================[�@UI�𐶐�,�o�^�@]
	for (int i = 0; i < ButtonType::Num; i++)
	{
		m_selectionButton[i] = std::make_unique<DrawLine>(SimpleMath::Vector2(width / 1.4f, 30 + (i * 120.0f)),
														SimpleMath::Vector2(150,60));

		m_uiKeyControl->AddUI(m_selectionButton[i].get());
	}

	//�@====================[�@�}�V���`��N���X�̐����@]
	m_titleCall = std::make_unique<DrawMachine>();
	m_titleCall->Initialize();

	//�@====================[�@�ϐ��̏������@]
	m_animationData = AnimationData();
	m_rotateNowFlag = false;

}

GAME_SCENE TitleScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	InputSupport* pINP = &InputSupport::GetInstance();
	SoundData& pSound = SoundData::GetInstance();

	//�@====================[�@BGM���Đ��@]
	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_TITLE, false);

	//�@====================[�@�^�C�g�����S�̕\���@]
	m_titleCall->Update();

	//�@====================[�@�J�����̍X�V�@]
	m_titleCamera->Update();
	
	//�@====================[�@���@�w�̍X�V�@]
	m_magicCircle->Update();

	//�@====================[�@��莞�Ԍo�߂ŏ����̒ǉ��@]
	if (m_titleCamera->GetAnimTime() >= 0.75f)
	{
		// �^�C�g�����S�̍X�V
		m_titleLogo->Update();

		// �E�����猻��鍕���̍X�V
		m_veil->Update();


		// �{�^���̃A�b�v�f�[�g
		for (int i = 0; i < ButtonType::Num; i++)
		{
			m_selectionButton[i]->Update();
		}

		// UI�̃L�[�{�[�h����N���X�̍X�V
		m_uiKeyControl->Update();
	}

	//�@====================[�@�J�����𓮂����@]
	pSD.GetCamera()->SetViewMatrix(m_titleCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_titleCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_titleCamera->GetEyePosition());

	//�@====================[�@���@�w�W�J�@]
	m_magicCircle->CreateMagicCircle(SimpleMath::Vector3{ 0,0,0 }, MAGIC_CIRCLE_RAGE);

	//�@====================[�@���N���b�N�������ۂ̏����@]
	//�@�@|=>�@�ϐ��̃��Z�b�g
	if (pINP->LeftButton_Press() && !m_rotateNowFlag)
	{
		m_animationData = 0.0f;
		m_rotateNowFlag = true;
	}

	//�@====================[�@�}�V���𓮂����@]
	if (m_rotateNowFlag)
	{
		m_animationData += 0.05f;
		m_animationData.anim = Easing::EaseInOutCubic(0.0f, XMConvertToRadians(90.0f), m_animationData);

		// �E��̖��@�w�̉�]���I�������V�[���J�ڂ��J�n���邽��
		// �A�j���[�V�����ϐ����ő�l�Ɏ����Ă��Ȃ����True�Ƃ���
		m_rotateNowFlag = !m_animationData.MaxCheck();

	}

	//�@====================[�@�I�v�V������������߂�@]
	if (m_renderOption->GetHitCancelButton())
	{
		m_nextType = ButtonType::Num;
	}

	//�@====================[�@�I������Ă��鍀�ڂ��I�v�V�����ł���]
	if (m_nextType == ButtonType::Option)
	{
		m_renderOption->Update();
	}
	//�@====================[�@�I������Ă��鍀�ڂ��I�v�V�����łȂ��@]
	else
	{
		// �e����(�͂��߂���,�Â�����,�I�v�V����,�Q�[���I��)�̍X�V����
		for (int i = 0; i < ButtonType::Num; i++)
		{
			if (m_selectionButton[i]->ClickMouse()) m_nextType = (ButtonType)i;
		}
	}

	//�@====================[�@�V�[���J�ڂ��s��Ȃ��@]
	if (m_rotateNowFlag) 	return GAME_SCENE();

	//�@====================[�@�X�e�[�W�Z���N�g�V�[���ɑJ�ځ@]
	//�@�@|=>�@���߂���
	if (m_nextType == ButtonType::NewGame)
	{
		Json::InitializationClearStageData();

		return GAME_SCENE::SELECT;
	}

	//�@====================[�@�X�e�[�W�Z���N�g�V�[���ɑJ�ځ@]
	//�@�@|=>�@��������
	if (m_nextType == ButtonType::Restert) 	return GAME_SCENE::SELECT;

	//�@====================[�@�Q�[�����I������@]
	if (m_nextType == ButtonType::Exit)		PostQuitMessage(0);

	return GAME_SCENE();
}

void TitleScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	//�@====================[�@�V���̍s��ݒ�@]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateTranslation({ 0.0f,70.0f,0.0f });
	modelData *= SimpleMath::Matrix::CreateScale(2.0f, 2.0f, 2.0f);
	modelData *= SimpleMath::Matrix::CreateFromYawPitchRoll(8.0f, 7.0f, 90.0f);

	//�@====================[�@�V���̕`��@]
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	//�@====================[�@�}�V���̕`��@]
	m_titleCall->Render();

	//�@====================[�@���@�w�̕`��@]
	m_magicCircle->CreateWorld();
	m_magicCircle->Render(0);
}

void TitleScene::DrawUI()
{
	// �K�v�ȃ��\�[�X�̊m��
	ShareData& pSD		= ShareData::GetInstance();
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture(1);
	auto pSB			= pSD.GetSpriteBatch();
	auto device			= pSD.GetDeviceResources();

	int width			= device->GetOutputSize().right;
	int height			= device->GetOutputSize().bottom;

	height;

	//�@====================[�@�������т̕`��@]
	m_veil->Render();

	//�@====================[�@UI�`��̊J�n�@]
	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	//�@�@|=> �摜�̃T�C�Y
	RECT rect_circle	= { 0, 0, texData.width, texData.height};

	//�@�@|=> �摜�̈ʒu�ݒ�
	SimpleMath::Vector2 box_Pos = { (float)width,0.0f};

	//�@�@|=>  �E��ɃN���b�N����]���閂�@�w��`��
	pSB->Draw(texData.tex.Get(),
			  box_Pos,
			  &rect_circle,
			  COLOR,
			  m_animationData.anim,
			  DirectX::XMFLOAT2(static_cast<float>(texData.width / 2), static_cast<float>(texData.height / 2)),
			  0.5f);

	//�@�@|=>  �{�^���̕`��
	for (int i = 0; i < ButtonType::Num; i++)
	{
		m_selectionButton[i]->Draw();
	}

	pSB->End();

	//�@====================[�@�Q�[�����S�̕`��@]
	m_titleLogo->Render();

	//�@====================[�@�I�v�V������ʂ̕`��@]
	if (m_nextType == ButtonType::Option)
	{
		m_renderOption->Render();
	}
}

void TitleScene::Finalize()
{
}
