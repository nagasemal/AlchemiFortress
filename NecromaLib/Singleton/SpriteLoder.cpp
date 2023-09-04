#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/CommonStruct.h"

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{

	// 選択ボックスのテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/log.png",
		nullptr,
		m_selectBoxTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/elementTex.png",
		nullptr,
		m_elementTextures.ReleaseAndGetAddressOf()
	);

	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MachineNames.png",
		nullptr,
		m_machineNameTextures.ReleaseAndGetAddressOf()
	);

	// エネミーの名前のテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/EnemyNames.png",
		nullptr,
		m_enemyNameTextures.ReleaseAndGetAddressOf()
	);

	// 製造の文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Seizou.png",
		nullptr,
		m_ManufacturingTexture.ReleaseAndGetAddressOf()
	);

	// 文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/wo.png",
		nullptr,
		m_letterTexture.ReleaseAndGetAddressOf()
	);

	// 文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionText.png",
		nullptr,
		m_missionTexture.ReleaseAndGetAddressOf()
	);

	// ラベルテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionLabel.png",
		nullptr,
		m_missionLabelTexture.ReleaseAndGetAddressOf()
	);
	
	// 数字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/number_1.png",
		nullptr,
		m_NumberTexture.ReleaseAndGetAddressOf()
	);

	// タイトルロゴテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/TitleLogo.png",
		nullptr,
		m_titleLogo.ReleaseAndGetAddressOf()
	);

	// UIアイコンテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/UIIcons.png",
		nullptr,
		m_uiIcons.ReleaseAndGetAddressOf()
	);

	// UIアイコンテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Arrow.png",
		nullptr,
		m_uiArrow.ReleaseAndGetAddressOf()
	);

	{
		// 魔法陣テクスチャ_None
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/MajicCircle.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::NONE].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Attacker
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::ATTACKER].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Defenser
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::DEFENSER].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Upper
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::UPPER].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Recovery
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::RECOVERY].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Recovery
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::RECOVERY].ReleaseAndGetAddressOf()
		);

		// 魔法陣テクスチャ_Mining
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
	// 解放
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