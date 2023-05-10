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

	m_playerBase = std::make_unique<PlayerBase>();
	m_playerBase ->Initialize();

	m_field		 = std::make_unique<Field>();
	m_field		 ->Initialize();

	m_moveCamera = std::make_unique<MoveCamera>();
	m_moveCamera ->Initialize();

}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();

	m_moveCamera	->Update();
	m_field			->Update();

	// �J�����𓮂���
	pSD.GetCamera()->SetViewMatrix		(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition	(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition		(m_moveCamera->GetEyePosition());

	// ���_��HP��0�ɂȂ����烊�U���g�֐؂�ւ���
	if (m_playerBase->GetHP() <= 0)
	{
		return GAME_SCENE::RESULT;
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

	m_playerBase->Draw();
	m_field->Draw();

	pSD.GetSpriteBatch()->End();

}

void PlayScene::Finalize()
{
}
