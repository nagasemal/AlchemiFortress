#include "pch.h"
#include "PlayScene.h"

PlayScene::PlayScene()
{
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

}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();

	m_moveCamera	->Update();
	m_fieldManager  ->Update();
	m_mousePointer  ->Update();

	m_enemyManager	->Update(m_fieldManager->GetPlayerBase()->GetPos());

	m_AM_Manager	->Update(m_fieldManager.get(),
							 m_mousePointer.get(),
							*m_enemyManager.get()->GetEnemyData());

	// エネミーToバレット(二重for)
	// ダングリング対策
	if (!m_AM_Manager->GetBullet()->empty() && !m_enemyManager->GetEnemyData()->empty())
	{
		for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
		{
			for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
			{
				// 当たり判定処理
				if (CircleCollider(bulletIt->get()->GetCircle(), enemyIt->GetCircle()))
				{
					bulletIt->get()->SetLife(0);
					enemyIt->SetHp(enemyIt->GetHp() - (int)bulletIt->get()->GetDamage());
				}
			}
		}
	}

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition		(m_moveCamera->GetEyePosition());

	// 拠点のHPが0になったらリザルトへ切り替える
	if (m_fieldManager->GetPlayerBase()->GetHP() <= 0)
	{
		return GAME_SCENE::RESULT;
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{

	ShareData& pSD = ShareData::GetInstance();

	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "PlayScene";

	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 60.f));

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	m_fieldManager      ->Draw();
	m_mousePointer		->Draw();

	m_AM_Manager		->Render();
	m_enemyManager		->Render();

	pSD.GetSpriteBatch()->End();

}

void PlayScene::DrawUI()
{

	m_AM_Manager		->DrawUI();

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
