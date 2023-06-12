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
	m_playerBase	= std::make_unique<PlayerBase>();
	m_playerBase	->Initialize();

	m_field			= std::make_unique<Field>();
	m_field			->Initialize();

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
	m_playerBase	->Update();
	m_field			->Update();
	m_mousePointer  ->Update();

	m_enemyManager->Update(m_playerBase->GetPos());
	m_AM_Manager	->Update(m_field->GetHitMouse(),m_playerBase->GetHitMouse(),m_mousePointer.get(),*m_enemyManager.get()->GetEnemyData());

	// エネミーにアルケミカルマシンのポインターを渡す(当たり判定に使用)
	//for (int i = 0; i < AlchemicalMachineManager::AM_MAXNUM; i++)
	//{
	//	AlchemicalMachineObject* alchemicalMachine = m_AM_Manager->GetAlchemicalMachineObject(i);
	//	if (!alchemicalMachine->GetActiv()) break;
	//	m_enemyManager->HitAMObejct(alchemicalMachine);
	//}

	// エネミーToバレット(二重for)
	// ダングリング対策
	if (!m_AM_Manager->GetBullet()->empty() && !m_enemyManager->GetEnemyData()->empty())
	{
		for (std::list<Bullet>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
		{
			for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
			{
				// 当たり判定処理
				if (CircleCollider(bulletIt->GetCircle(), enemyIt->GetCircle()))
				{

					bulletIt->SetLife(0);
					enemyIt->SetHp(enemyIt->GetHp() - bulletIt->GetDamage());

				}
			}
		}
	}

	// カメラを動かす
	pSD.GetCamera()->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition		(m_moveCamera->GetEyePosition());

	// 拠点のHPが0になったらリザルトへ切り替える
	if (m_playerBase->GetHP() <= 0)
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

	m_playerBase		->Draw();
	m_field				->Draw();
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
	m_playerBase		->Finalize();
	m_playerBase.reset();

	m_field				->Finalize();
	m_field.reset();

	m_mousePointer		->Finalize();
	m_mousePointer.reset();
	
	m_AM_Manager		->Finalize();
	m_AM_Manager.reset();

	m_enemyManager		->Finalize();
	m_enemyManager.reset();

	m_moveCamera.reset();

}
