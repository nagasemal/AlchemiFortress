#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

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

	// 製造の文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Seizou.png",
		nullptr,
		m_ManufacturingTexture.ReleaseAndGetAddressOf()
	);

	// 数字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/number_1.png",
		nullptr,
		m_NumberTexture.ReleaseAndGetAddressOf()
	);

	// 魔法陣テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MajicCircle.png",
		nullptr,
		m_MagicCircleTexture[0].ReleaseAndGetAddressOf()
	);

	// 魔法陣テクスチャ2
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MagicCircle_2.png",
		nullptr,
		m_MagicCircleTexture[1].ReleaseAndGetAddressOf()
	);
}

void SpriteLoder::Finalize()
{
	// 解放
	m_elementTextures.Reset();
	m_ManufacturingTexture.Reset();

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