#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

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

	// ���@�w�e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MajicCircle.png",
		nullptr,
		m_MagicCircleTexture[0].ReleaseAndGetAddressOf()
	);

	// ���@�w�e�N�X�`��2
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MagicCircle_2.png",
		nullptr,
		m_MagicCircleTexture[1].ReleaseAndGetAddressOf()
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
}

void SpriteLoder::Finalize()
{
	// ���
	m_selectBoxTexture.Reset();
	m_elementTextures.Reset();
	m_machineNameTextures.Reset();
	m_ManufacturingTexture.Reset();
	m_NumberTexture.Reset();

	m_MagicCircleTexture->Reset();

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