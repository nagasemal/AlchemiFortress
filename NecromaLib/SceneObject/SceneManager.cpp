#include "pch.h"
#include "DeviceResources.h"
#include "SceneManager.h"

#include "imaseLib/DebugCamera.h"

#include "Scenes/TitleScene/TitleScene.h"
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/ResultScene/ResultScene.h"

// ���O��Ԃ̗��p
using namespace DirectX;

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
SceneManager::SceneManager():
	m_NextScene(GAME_SCENE::TITLE),		// �����V�[���̐ݒ�
	m_Scene(nullptr)
{

	Fade::Create();
	m_Fade = Fade::GetInstance();

	DataManager::Create();
	m_DataManager = DataManager::GetInstance();

}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
SceneManager::~SceneManager()
{
	Fade::Destroy();

	DataManager::Destroy();
}

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
void SceneManager::Initialize()
{
	m_Fade->Initialize(1280, 720);
	m_Fade->StartFadeIn();

	// �V�[���쐬
	CreateScene();
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void SceneManager::Update(const DX::StepTimer& timer)
{
	InputSupport* pINP = &InputSupport::GetInstance();

	m_Fade->Update(timer);

	// ESC�L�[�ŏI��
	if (pINP->GetKeybordState().IsKeyPressed(Keyboard::Escape)) PostQuitMessage(0);


	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_NextScene != GAME_SCENE::NONE)
	{
		//  ��x�����t�F�[�h�C���̃t���O�𗧂ĂāA����ȍ~�A�t�F�[�h�C���Ȃ��
		//�@�V�[���J�ڂ͕ۗ��Ƃ���
		if (!m_Fade->ProcessCheckFadeNow())	m_Fade->StartFadeIn();
		if (m_Fade->ProcessCheckFadeIn())	return;

		// �V�[���폜
		DeleteScene();

		// �V�[���쐬
		CreateScene();
	}

	//	�t�F�[�h���I������܂ŏ������s���Ȃ��悤�ɂ���
	if (m_Fade->ProcessCheckFadeNow()) return;

	// ���Ԃ�����΍X�V
	if (m_Scene != nullptr)
	{
		m_NextScene = m_Scene->Update();
	}

	//�@�f�o�b�O�@�\
	{
		// �^�C�g���V�[����
		if (pINP->GetKeybordState().IsKeyReleased(Keyboard::T))		m_NextScene = GAME_SCENE::TITLE;

		//�@�v���C�V�[����
		if (pINP->GetKeybordState().IsKeyReleased(Keyboard::P))		m_NextScene = GAME_SCENE::PLAY;

		//�@���U���g�V�[��
		if (pINP->GetKeybordState().IsKeyReleased(Keyboard::R))		m_NextScene = GAME_SCENE::RESULT;
	}
}

//-------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------
void SceneManager::Render()
{
	// ���Ԃ�����Ε`��
	if (m_Scene != nullptr)
	{
		m_Scene->Draw();
	}

	// ���Ԃ�����Ε`��
	if (m_Scene != nullptr)
	{
		m_Scene->DrawUI();
	}

	// �t�F�[�h�pRender
	m_Fade->Render();

}

//-------------------------------------------------------------------
// ��n��
//-------------------------------------------------------------------
void SceneManager::Finalize()
{
	m_Fade->Finalize();
	DeleteScene();
}

/*--------------------------------------------------
�V�[���̍쐬
--------------------------------------------------*/
void SceneManager::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_Scene != nullptr)				return;

	// ���V�[���̍쐬
	switch (m_NextScene)
	{

	case GAME_SCENE::TITLE:
	{
		m_Scene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		m_Scene = new PlayScene();
		break;
	}
	case GAME_SCENE::RESULT:
	{
		m_Scene = new ResultScene();
		break;
	}
	default:
	{
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}

	}

	// �쐬�����V�[����������
	m_Scene->Initialize();

	// ���ւ̃V�[������������
	m_NextScene = GAME_SCENE::NONE;
}

/*--------------------------------------------------
�V�[���̍폜
--------------------------------------------------*/
void SceneManager::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_Scene == nullptr)
	{
		return;
	}

	// ���V�[���̏I������
	m_Scene->Finalize();

	// ���V�[���̍폜
	delete m_Scene;
	m_Scene = nullptr;
}