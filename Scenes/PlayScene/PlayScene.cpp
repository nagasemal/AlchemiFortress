#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/DataManager.h"

#define MAX_SPEED 4

PlayScene::PlayScene()
{
	m_stageNumber = DataManager::GetInstance()->GetStageNum();

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber);

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
	// �~�b�V�����}�l�[�W���[�N���X�̐���
	m_missionManager = std::make_unique<MissionManager>();
	m_missionManager->Initialize();

	// �`���[�g���A���N���X�̐���
	m_tutorial = std::make_unique<Tutorial>();
	m_tutorial->Initialize(ShareJsonData::GetInstance().GetStageData().tutorial);

	// ������@�N���X�̐���
	m_explanation = std::make_unique<Explanation>();

	// �{���{�^���̐���
	m_doubleSpeedButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(width - 100.0f, height - 100.0f), SimpleMath::Vector2(1.0f, 1.0f));

	// �V�����f���̃��[�h
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	m_skySphere->UpdateEffects([&](IEffect* effect)
	{
		// ����̓��C�g�����~����
		auto lights = dynamic_cast<IEffectLights*>(effect);

		// ���̓�������ύX
		lights->SetAmbientLightColor(SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));

	});
}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DeltaTime& pDelta = DeltaTime::GetInstance();
	bool tutorialFlag = m_tutorial->GetTutorialFlag();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_PLAY, false);

	m_tutorial			->Update(m_AM_Manager.get(),
								 m_resourceGauge.get(),
								 m_missionManager->GetMissionRender()->get(),
								 m_moveCamera->GetStopCameraFlag());

	m_explanation->Update();

	// �`���[�g���A�����Ȃ�Έȉ��̏�����ʂ��Ȃ�
	if (tutorialFlag) 		return GAME_SCENE();

	// �{���{�^���̃A�b�v�f�[�g
	m_doubleSpeedButton->HitMouse();
	m_doubleSpeedNum += m_doubleSpeedButton->ClickMouse();
	if (m_doubleSpeedNum > MAX_SPEED) m_doubleSpeedNum = 1;
	// �{���ɂ���
	pDelta.SetDeltaTime(pDelta.GetDeltaTime() * m_doubleSpeedNum);

	m_moveCamera		->Update(!m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), true);

	m_fieldManager		->Update();
	m_mousePointer		->Update();

	// ���j�b�g(�}�V��)�}�l�[�W���[�̃A�b�v�f�[�g
	m_AM_Manager		->Update(m_fieldManager.get(),
						 m_mousePointer.get(),
						 m_enemyManager.get(),
						 m_moveCamera.get());

	m_enemyManager		->Update(m_fieldManager->GetPlayerBase()->GetPos());

	m_resourceGauge		->Update();

	m_missionManager	->Update(m_AM_Manager.get(),m_enemyManager.get(),m_fieldManager.get());


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
		ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	///*===[ �f�o�b�O�����`�� ]===*/
	//std::wostringstream oss;
	//oss << "PlayScene";

	//pSD.GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 60.f));

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.8f,1.8f,1.8f });
	modelData = SimpleMath::Matrix::CreateTranslation({0.0f,70.0f,0.0f });

	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	m_fieldManager      ->Draw();
	m_mousePointer		->Draw();


	//D3D11_DEPTH_STENCIL_DESC desc =
	//{
	//	TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
	//	FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
	//	D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, D3D11_COMPARISON_ALWAYS
	//};
	//ID3D11DepthStencilState* stencilState = nullptr;
	//pSD.GetDevice()->CreateDepthStencilState(&desc, &stencilState);


	m_enemyManager		->Render();
	m_AM_Manager		->Render();

	pSD.GetSpriteBatch()->End();
}

void PlayScene::DrawUI()
{
	m_tutorial->Render();

	m_AM_Manager		->DrawUI();
	m_resourceGauge		->Render();
	m_missionManager	->Render();

	m_tutorial->Render_Layer2();

	// �{���{�^��
	m_doubleSpeedButton->DrawUI(10 + m_doubleSpeedNum);

	m_explanation->Render(m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), m_AM_Manager->GetRotateStopFlag());

	// ��������`��

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

	ShareData& pSD = ShareData::GetInstance();


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
		bool hitEnemy;

		hitEnemy = CircleCollider(playerBase->GetCircle(), enemyIt->GetCircle());

		// �����蔻�菈��
		if (hitEnemy)
		{
			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
			enemyIt->SetStopFlag(hitEnemy);
		}
	}
}

void PlayScene::EnemyToBullet()
{

	for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
	{
		for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
		{
			// �����蔻�菈��
			if (CircleCollider(bulletIt->get()->GetCircle(), enemyIt->GetCircle()))
			{
				bulletIt->get()->SetEnemyHit(true);
				bulletIt->get()->SetLife(0);
				enemyIt->SetHp(enemyIt->GetHp() - (int)bulletIt->get()->GetDamage());
			}
		}
	}

}
