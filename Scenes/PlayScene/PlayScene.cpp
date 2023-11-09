#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/DataManager.h"

#define MAX_SPEED 4

PlayScene::PlayScene()
{
	m_stageNumber = DataManager::GetInstance()->GetStageNum();

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	//�@�X�e�[�W�i���o�[��Wave1��ǂݍ���
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber,1);

	// ���{���ɐݒ�
	m_doubleSpeedNum = 1;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	// ��ʂ̏c���l�̎擾
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// �|�X�g�v���Z�X�̐���
	m_postProcess = std::make_unique<BasicPostProcess>(ShareData::GetInstance().GetDevice());


	// �t�B�[���h�}�l�[�W���[�N���X�̐���
	m_fieldManager = std::make_unique<FieldObjectManager>();
	m_fieldManager ->Initialize();
	// �}�E�X�|�C���^�[�N���X�̐���
	m_mousePointer	= std::make_unique<MousePointer>();
	m_mousePointer	->Initialize();
	// ���j�b�g(�}�V��)�}�l�[�W���[�N���X�̐���
	m_AM_Manager	= std::make_unique<AlchemicalMachineManager>();
	m_AM_Manager	->Initialize();
	// �J�����𓮂����N���X�̐���
	m_moveCamera	= std::make_unique<MoveCamera>();
	m_moveCamera	->Initialize();
	// �G�l�~�[�}�l�[�W���[�N���X�̐���
	m_enemyManager  = std::make_unique<EnemyManager>();
	m_enemyManager	->Initialize();
	// ���\�[�X�������Q�[�W�N���X�̐���
	m_resourceGauge = std::make_unique<Gauge>();
	m_resourceGauge	->Initialize();
	// ���_��Lv�������N���X�̐���
	m_baseLv = std::make_unique<BaseLv>();
	m_baseLv->SetPosition(SimpleMath::Vector2(40.0f, 40.0f));
	m_baseLv->SetScale(SimpleMath::Vector2(0.1f, 0.1f));
	// �~�b�V�����}�l�[�W���[�N���X�̐���
	m_missionManager = std::make_unique<MissionManager>();
	m_missionManager->Initialize();

	// ������@�N���X�̐���
	m_explanation = std::make_unique<Explanation>();

	// �{���{�^���̐���
	m_doubleSpeedButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(width / 1.05f, height / 1.6f), SimpleMath::Vector2(1.0f, 1.0f));

	// �V�����f���̃��[�h
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	// �V�����f���̃��[�h
	m_skySphere->UpdateEffects([&](IEffect* effect)
	{
		// ����̓��C�g�����~����
		auto lights = dynamic_cast<IEffectLights*>(effect);

		// ���̓�������ύX
		lights->SetAmbientLightColor(SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));
	});

	// �`���[�g���A���N���X�̐���
	m_tutorial = std::make_unique<Tutorial>();
	m_tutorial->Initialize(ShareJsonData::GetInstance().GetStageData().tutorial, this);


	// �����_�[�e�N�X�`���̍쐬�i�V�[���S�́j
	m_offscreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_offscreenRT->SetDevice(pSD.GetDevice());
	RECT rect = pSD.GetDeviceResources()->GetOutputSize();
	m_offscreenRT->SetWindow(rect);

}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DeltaTime& pDelta = DeltaTime::GetInstance();
	bool tutorialFlag = m_tutorial->GetExplanationFlag();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_PLAY, false);

	m_tutorial			->Update(this,m_moveCamera->GetStopCameraFlag());

	m_explanation		->Update();

	m_missionManager	->Update(m_AM_Manager.get(), m_enemyManager.get(), m_fieldManager.get());

	//�@����Wave�ɐi�񂾂��Ƃ�m�点��
	if (m_missionManager->NextWaveFlag())
	{
		DataManager* pDM = DataManager::GetInstance();
		ShareJsonData& pSJD = ShareJsonData::GetInstance();
		Stage_Data stageData = pSJD.GetStageData();

		m_AM_Manager->ReloadResource();
		m_enemyManager->ReloadEnemyData();
		m_tutorial->RelodeTutorial(stageData.tutorial, this);
		m_missionManager->ReloadWave();

		// ���\�[�X�Q��ǉ�����
		pDM->SetNowBaseHP(pDM->GetNowBaseHP() + stageData.resource.hp);
		pDM->SetNowCrystal(pDM->GetNowCrystal() + stageData.resource.crystal);
		pDM->SetNowMP(pDM->GetNowMP() + stageData.resource.mp);

	}

	// �^�C�g���ڍs�{�^���������ꂽ��^�C�g���V�[���Ɍ�����
	if (m_tutorial->GetTitleSceneButton()->ClickMouse()) return GAME_SCENE::TITLE;
	// �Z���N�g�ڍs�{�^���������ꂽ��Z���N�g�V�[���Ɍ�����
	if (m_tutorial->GetSelectSceneButton()->ClickMouse()) return GAME_SCENE::SELECT;

	//// �`���[�g���A�����Ȃ�Έȉ��̏�����ʂ��Ȃ�
	if (tutorialFlag) 		return GAME_SCENE();

	// �{���{�^���̃A�b�v�f�[�g
	m_doubleSpeedButton->HitMouse();
	m_doubleSpeedNum += m_doubleSpeedButton->ClickMouse();
	if (m_doubleSpeedNum > MAX_SPEED) m_doubleSpeedNum = 1;
	// �{���ɂ���
	pDelta.SetDoubleSpeed((float)m_doubleSpeedNum);

	m_moveCamera		->Update(!m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), true);

	m_fieldManager		->Update(m_enemyManager.get());
	m_mousePointer		->Update();

	// ���j�b�g(�}�V��)�}�l�[�W���[�̃A�b�v�f�[�g
	m_AM_Manager		->Update(m_fieldManager.get(),
						 m_mousePointer.get(),
						 m_enemyManager.get(),
						 m_moveCamera.get());

	m_missionManager	->TimerUpdate();

	m_enemyManager		->Update(m_fieldManager->GetPlayerBase()->GetPos());

	m_resourceGauge		->Update();

	m_baseLv			->Update(m_fieldManager.get());

	bool enemyActivs	= !m_enemyManager->GetEnemyData()->empty();

	// �G�l�~�[To�o���b�g
	// �_���O�����O�΍�
	if (!m_AM_Manager	->GetBullet()->empty() && enemyActivs) EnemyToBullet();

	// �G�l�~�[To�v���C���[�x�[�X(�����蔻��)
	if (enemyActivs)	  EnemyToPlayerBase();

	// �J�����𓮂���
	pSD.GetCamera()		->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()		->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()		->SetEyePosition	(m_moveCamera->GetEyePosition());

	// ���_��HP��0�ɂȂ�A�~�b�V�����̓��e�I�ɃN���A���s�\�ɂȂ����ꍇ���U���g�֐؂�ւ���
	if (m_missionManager->MissionmFailure())		return GAME_SCENE::RESULT;

	// �~�b�V������S�ĒB�������烊�U���g�֐؂�ւ���
	if (m_missionManager->MissionComplete())
	{
		// �X�e�[�W�U�����𓾂�
		Json::WritingJsonFile_ClearData(m_stageNumber,
										m_AM_Manager->GetAlchemicalMachineObject(),
										m_missionManager->GetStartTimer());

		return GAME_SCENE::RESULT;
	}

	InputSupport* pINP = &InputSupport::GetInstance();

	// �}�V���̃f�[�^�ēǂݍ���
	if (pINP->GetKeybordState().IsKeyReleased(Keyboard::M))
	{
		ModelShader::GetInstance().CreateModelShader();
		ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	auto context = pSD.GetContext();

	auto renderTarget = pSD.GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = pSD.GetDeviceResources()->GetDepthStencilView();
	auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
	auto offscreenSRV = m_offscreenRT->GetShaderResourceView();

	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g��ύX�isceneRT�j
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(m_offscreenRT->GetRenderTargetView(), Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &offscreenRTV, depthStencil);
	// -------------------------------------------------------------------------- //


	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.8f,1.8f,1.8f });
	modelData = SimpleMath::Matrix::CreateTranslation({0.0f,70.0f,0.0f });

	// �V���`��
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	// �t�B�[���h�I�u�W�F�N�g�`��
	m_fieldManager	->Draw();
	// �G�l�~�[�`��
	m_enemyManager	->Render();
	// �}�V���̕`��
	m_AM_Manager	->Render();

	// �}�E�X�|�C���^�[���f�����o��
	m_mousePointer->Draw();
	m_mousePointer->ModelDraw(m_AM_Manager->GetSelectModel());

	pSD.GetSpriteBatch()->End();

	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = pSD.GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	// -------------------------------------------------------------------------- //

	// �Z�s�A���ɐF��ς���|�X�g�v���Z�X�����s����
	//m_postProcess->SetEffect(BasicPostProcess::Sepia);
	m_postProcess->SetSourceTexture(offscreenSRV);
	//m_postProcess->SetSourceTexture(SpriteLoder::GetInstance().GetAuraBase().Get());
	m_postProcess->Process(context);


}

void PlayScene::DrawUI()
{

	m_tutorial->Render();

	m_AM_Manager		->DrawUI();
	m_resourceGauge		->Render();

	m_baseLv			->Render();

	m_missionManager	->Render();

	// �{���{�^��
	m_doubleSpeedButton->DrawUI(10 + m_doubleSpeedNum);

	// ��������`��
	m_explanation->Render(m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), m_AM_Manager->GetRotateStopFlag());

	m_tutorial->Render_Layer2();

	//SimpleMath::Vector2 origin = SimpleMath::Vector2(1280, 1280);
	//RECT rect = { 0,0,origin.x,origin.y };
	//pSB->Begin();
	//pSB->Draw(pSL->GetMagicCircleTexture(0).Get(), SimpleMath::Vector2(), &rect, Colors::White, 0.0f, origin / 2, 0.3f);
	//pSB->End();

}

void PlayScene::Finalize()
{
	m_fieldManager      ->Finalize();
	m_fieldManager.reset();

	m_mousePointer		->Finalize();
	m_mousePointer.reset();
	
	m_AM_Manager		->Finalize();
	m_AM_Manager.reset();

	m_enemyManager		->Finalize();
	m_enemyManager.reset();

	m_moveCamera.reset();

}

void PlayScene::EnemyToAMMachine()
{
	//for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	//{
	//	for (int i = 0; i < m_AM_Manager->GetAlchemicalMachineObject().size();i++)
	//	{
	//		AlchemicalMachineObject* object = m_AM_Manager->GetAlchemicalMachineObject(i)->get();
	//		bool hitEnemy = false;
	//		// �����蔻�菈��
	//		if (hitEnemy = CircleCollider(object->GetCircle(), enemyIt->GetCircle())
	//		&&  object->GetModelID() == AlchemicalMachineObject::DEFENSER)
	//		{
	//			enemyIt->HitMachine(hitEnemy);
	//		}
	//	}
	//}
}

void PlayScene::EnemyToPlayerBase()
{
	PlayerBase* playerBase = m_fieldManager->GetPlayerBase();

	for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	{
		// �_�E���L���X�g���s���AGameObject3D�^�ɕϊ�������̏����𓾂�
		bool hitEnemy = CircleCollider(playerBase->GetObject3D(),enemyIt->GetObject3D());

		// �����蔻�菈��
		if (hitEnemy)
		{
			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
			enemyIt->SetStopFlag(hitEnemy);
			// ����������
			enemyIt->SetAccele(-enemyIt->GetAccele());

		}
	}
}

void PlayScene::EnemyToBullet()
{

	for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
	{
		for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
		{

			// �����蔻�����邩�ۂ�
			if (!enemyIt->GetColliderActive()) continue;

			// �����蔻�菈��
			if (CircleCollider(bulletIt->get()->GetObject3D(), enemyIt->GetObject3D()))
			{

				SimpleMath::Vector3 hitVectol = bulletIt->get()->GetPos() - enemyIt->GetPos();

				bulletIt->get()->SetEnemyHit(true);
				bulletIt->get()->SetLife(0);

				enemyIt->HitBullet(hitVectol, (int)bulletIt->get()->GetDamage());

			}
		}
	}

}
