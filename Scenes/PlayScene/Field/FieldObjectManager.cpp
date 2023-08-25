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

	// ���f���̓ǂݍ���
	m_floorModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
	m_crystalModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Crystal.cmo", *fx);
	m_baseModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower.cmo", *fx);

	// �������f���̃G�t�F�N�g�̐ݒ�
	m_crystalModel->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// ���̓�������ύX
			lights->SetAmbientLightColor(DirectX::SimpleMath::Color(0.4f, 0.4f, 1.f, 0.2f));

		});

	// ����
	m_field = std::make_unique<Field>();
	m_playerBase = std::make_unique<PlayerBase>();
	m_crystals = std::make_unique<std::list<Crystal>>();

	m_field->Initialize();
	m_playerBase->Initialize();

	CrystalSpawn(30);

	//m_crystals->push_back(*std::make_unique<Crystal>(DirectX::SimpleMath::Vector3(1, -1.0f,5), cos(5)));

	// �N���X�^���̏���������
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Initialize();
	}

}

void FieldObjectManager::Update()
{
	// ���̍X�V����
	m_field->Update();

	// �v���C���[���_�̍X�V����
	m_playerBase->Update();

	m_crystalTomouseFlag = false;

	// �N���X�^���̍X�V����
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Update();

		// �N���X�^�����}�E�X�ɐG��Ă���
		if (!m_crystalTomouseFlag)
		{
			m_crystalTomouseFlag = it->GetHitMouse();
		}
	}

}

void FieldObjectManager::Draw()
{
	// ���̕`�揈��
	m_field->Render(m_floorModel.get());

	// �v���C���[���_�̕`�揈��
	m_playerBase->Render(m_baseModel.get());

	// �N���X�^���̕`�揈��
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Render(m_crystalModel.get());
	}


}

void FieldObjectManager::Finalize()
{
	// ���̍폜����
	m_field->Finalize();
	m_field.reset();

	// �v���C���[���_�̍X�V����
	m_playerBase->Finalize();
	m_playerBase.reset();

	// �N���X�^���̕`�揈��
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
