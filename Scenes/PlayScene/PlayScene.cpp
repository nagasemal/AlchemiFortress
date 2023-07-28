#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"

PlayScene::PlayScene()
{

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(1);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{

	m_fieldManager = std::make_unique<FieldObjectManager>();
	m_fieldManager ->Initialize();

	m_mousePointer	= std::make_unique<MousePointer>();
	m_mousePointer	->Initialize();

	m_AM_Manager	= std::make_unique<AlchemicalMachineManager>();
	m_AM_Manager	->Initialize();

	m_moveCamera	= std::make_unique<MoveCamera>();
	m_moveCamera	->Initialize();

	m_enemyManager  = std::make_unique<EnemyManager>();
	m_enemyManager	->Initialize();

	m_gauge = std::make_unique<Gauge>();
	m_gauge->Initialize();

	m_missionManager = std::make_unique<MissionManager>();
	m_missionManager->Initialize();

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	m_skySphere->UpdateEffects([&](IEffect* effect)
	{
		// ����̓��C�g�����~����
		auto lights = dynamic_cast<IEffectLights*>(effect);

		// ���̓�������ύX
		lights->SetAmbientLightColor(DirectX::SimpleMath::Color(0.7f, 0.7f, 1.f, 0.8f));

	});
}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();

	m_moveCamera->Update(!m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), true);

	m_fieldManager->Update();
	m_mousePointer->Update();

	m_AM_Manager->Update(m_fieldManager.get(),
						 m_mousePointer.get(),
						 m_enemyManager.get(),
						 m_moveCamera.get());

	m_enemyManager->Update(m_fieldManager->GetPlayerBase()->GetPos());

	m_gauge->Update();

	m_missionManager->Update(m_AM_Manager.get(),m_enemyManager.get());

	bool enemyActivs = !m_enemyManager->GetEnemyData()->empty();
	// �G�l�~�[To�o���b�g
	// �_���O�����O�΍�
	if (!m_AM_Manager->GetBullet()->empty() && enemyActivs) EnemyToBullet();

	// �G�l�~�[To�v���C���[�x�[�X
	if (enemyActivs) EnemyToPlayerBase();

	// �J�����𓮂���
	pSD.GetCamera()->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition		(m_moveCamera->GetEyePosition());

	// ���_��HP��0�ɂȂ����烊�U���g�֐؂�ւ���
	if (m_fieldManager->GetPlayerBase()->GetHP() <= 0 || m_missionManager->MissionmFailure())
	{
		return GAME_SCENE::RESULT;
	}

	if (m_missionManager->MissionComplete())
	{
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

	/*===[ �f�o�b�O�����`�� ]===*/
	std::wostringstream oss;
	oss << "PlayScene";

	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale({ 1.8f,1.8f,1.8f });
	modelData = DirectX::SimpleMath::Matrix::CreateTranslation({0.0f,70.0f,0.0f });

	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	m_fieldManager      ->Draw();
	m_mousePointer		->Draw();

	m_enemyManager		->Render();
	m_AM_Manager		->Render();

	pSD.GetSpriteBatch()->End();
}

void PlayScene::DrawUI()
{
	m_AM_Manager		->DrawUI();
	m_gauge				->Render();
	m_missionManager->Render();
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
		bool hitEnemy;

		// �����蔻�菈��
		if (hitEnemy = CircleCollider(playerBase->GetCircle(), enemyIt->GetCircle()))
		{
			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
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
