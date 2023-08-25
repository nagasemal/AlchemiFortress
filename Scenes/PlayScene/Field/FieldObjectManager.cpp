#include "pch.h"
#include "FieldObjectManager.h"
#include "NecromaLib/Singleton/ShareData.h"

#include <algorithm>
#include <random>

FieldObjectManager::FieldObjectManager():
	m_crystalTomouseFlag()
{
}

FieldObjectManager::~FieldObjectManager()
{
}

void FieldObjectManager::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	// モデルの読み込み
	m_floorModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
	m_crystalModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Crystal.cmo", *fx);
	m_baseModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower.cmo", *fx);

	// 結晶モデルのエフェクトの設定
	m_crystalModel->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 光の当たり方変更
			lights->SetAmbientLightColor(DirectX::SimpleMath::Color(0.4f, 0.4f, 1.f, 0.2f));

		});

	// 生成
	m_field = std::make_unique<Field>();
	m_playerBase = std::make_unique<PlayerBase>();
	m_crystals = std::make_unique<std::list<Crystal>>();

	m_field->Initialize();
	m_playerBase->Initialize();

	CrystalSpawn(30);

	//m_crystals->push_back(*std::make_unique<Crystal>(DirectX::SimpleMath::Vector3(1, -1.0f,5), cos(5)));

	// クリスタルの初期化処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Initialize();
	}

}

void FieldObjectManager::Update()
{
	// 床の更新処理
	m_field->Update();

	// プレイヤー拠点の更新処理
	m_playerBase->Update();

	m_crystalTomouseFlag = false;

	// クリスタルの更新処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Update();

		// クリスタルがマウスに触れている
		if (!m_crystalTomouseFlag)
		{
			m_crystalTomouseFlag = it->GetHitMouse();
		}
	}

}

void FieldObjectManager::Draw()
{
	// 床の描画処理
	m_field->Render(m_floorModel.get());

	// プレイヤー拠点の描画処理
	m_playerBase->Render(m_baseModel.get());

	// クリスタルの描画処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Render(m_crystalModel.get());
	}


}

void FieldObjectManager::Finalize()
{
	// 床の削除処理
	m_field->Finalize();
	m_field.reset();

	// プレイヤー拠点の更新処理
	m_playerBase->Finalize();
	m_playerBase.reset();

	// クリスタルの描画処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Finalize();
	}

	m_crystals.reset();

	delete m_crystals.get();

}

void FieldObjectManager::CrystalSpawn(int num)
{
	int minRage = 2;

	for (int i = 0; i < num; i++)
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<> dist(0, XM_2PI);

		std::uniform_int_distribution<> dist2(minRage,m_field->GetCircle().r);
		std::random_device rd;
		std::mt19937 gen(rd());

		float rand = static_cast<float>(dist(engine));

		float rand2 = static_cast<float>(dist2(gen));

		m_crystals->push_back(*std::make_unique<Crystal>(DirectX::SimpleMath::Vector3(rand2 * cosf(rand), -1.0f, rand2 * sinf(rand)),cos(rand2)));
	}
}
