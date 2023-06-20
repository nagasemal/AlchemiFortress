#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{
	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/elementTex.png",
		nullptr,
		m_elementTextures.ReleaseAndGetAddressOf()
	);


}

void SpriteLoder::Finalize()
{
	// 解放
	m_elementTextures.Reset();

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