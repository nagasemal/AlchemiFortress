#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/CommonStruct.h"

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{

	// �I���{�b�N�X�̃e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/log.png",
		nullptr,
		m_selectBoxTexture.ReleaseAndGetAddressOf()
	);

	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/elementTex.png",
		nullptr,
		m_elementTextures.ReleaseAndGetAddressOf()
	);

	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MachineNames.png",
		nullptr,
		m_machineNameTextures.ReleaseAndGetAddressOf()
	);

	// �G�l�~�[�̖��O�̃e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/EnemyNames.png",
		nullptr,
		m_enemyNameTextures.ReleaseAndGetAddressOf()
	);

	// �����̕����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Seizou.png",
		nullptr,
		m_ManufacturingTexture.ReleaseAndGetAddressOf()
	);

	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/wo.png",
		nullptr,
		m_letterTexture.ReleaseAndGetAddressOf()
	);

	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionText.png",
		nullptr,
		m_missionTexture.ReleaseAndGetAddressOf()
	);

	// ���x���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionLabel.png",
		nullptr,
		m_missionLabelTexture.ReleaseAndGetAddressOf()
	);
	
	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/number_1.png",
		nullptr,
		m_NumberTexture.ReleaseAndGetAddressOf()
	);

	// �^�C�g�����S�e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/TitleLogo.png",
		nullptr,
		m_titleLogo.ReleaseAndGetAddressOf()
	);

	// UI�A�C�R���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/UIIcons.png",
		nullptr,
		m_uiIcons.ReleaseAndGetAddressOf()
	);

	// UI�A�C�R���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Arrow.png",
		nullptr,
		m_uiArrow.ReleaseAndGetAddressOf()
	);

	{
		// ���@�w�e�N�X�`��_None
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/MajicCircle.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::NONE].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Attacker
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::ATTACKER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Defenser
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::DEFENSER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Upper
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::UPPER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Recovery
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::RECOVERY].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Recovery
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::RECOVERY].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Mining
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::MINING].ReleaseAndGetAddressOf()
		);
	}
}

void SpriteLoder::Finalize()
{
	// ���
	m_selectBoxTexture.Reset();
	m_elementTextures.Reset();
	m_machineNameTextures.Reset();
	m_ManufacturingTexture.Reset();
	m_NumberTexture.Reset();

	for (int i = 0; i < MACHINE_TYPE::NUM;i++)
	{
		m_MagicCircleTexture[i].Reset();
	}
}

SpriteLoder::SpriteLoder()
{

}

void SpriteLoder::Create()
{
	if (instance == nullptr)
	{
		instance = new SpriteLoder;
	}
}

void SpriteLoder::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}