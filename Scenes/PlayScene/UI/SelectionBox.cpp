#include "pch.h"
#include "SelectionBox.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

SelectionBox::SelectionBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{

	m_saveData.pos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;

	m_hitMouseFlag = false;
	m_selectFlag = false;

	m_boxColor = { 0.0f,0.0f,0.0f,0.0f };
	m_luminousFlag = false;

	m_rect = { 0, 0, 64, 64 };

	m_popUITextTimer = 0.0f;

}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::Update()
{
	m_keySelectFlag = false;

	if (m_selectFlag) SoundData::GetInstance().PlaySystemSE(XACT_WAVEBANK_SYSTEMSE_BUTTONSELECT);
}

void SelectionBox::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	//RECT srcRect = { 0, 0, 64, 64 };

		// ���O�̐F
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour =  m_boxColor;

	if (m_hitMouseFlag) colour = { 0.9f,0.9f,0.9f,1.0f };

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// �I��BOX
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), box_Pos, &m_rect, colour, 0.0f, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2, static_cast<float>(m_rect.bottom) / 2), m_data.rage);

	pSB->End();

}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture,
						  RECT rect,SimpleMath::Color pulsTextureCol)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// ���O�̐F
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour =  m_boxColor;

	if (m_hitMouseFlag) colour = { 0.9f,0.9f,0.9f,1.0f };

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// �I��BOX
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), box_Pos, &m_rect, colour, 0.0f, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2, static_cast<float>(m_rect.bottom) / 2), m_data.rage);

	// ���ɕ\������e�N�X�`��������ꍇ
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(), box_Pos, &rect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);
	}

	pSB->End();

}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, float rotate)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// ���O�̐F
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour = m_boxColor;

	if (m_hitMouseFlag) colour = { 0.9f,0.9f,0.9f,1.0f };

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// �I��BOX
	pSB->Draw(texture.Get(), box_Pos, &m_rect, colour, rotate, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2.0f, static_cast<float>(m_rect.bottom) / 2.0f), m_data.rage);

	pSB->End();
}

void SelectionBox::DrawUI(int UInumber)
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		// ���O�̐F
	SimpleMath::Color colour = { 1.0f,1.0f,1.0f,1.0f };

	if (m_luminousFlag) colour = m_boxColor;

	if (m_hitMouseFlag)
	{
		colour = { 0.9f,0.9f,0.9f,1.0f };

		m_popUITextTimer += DeltaTime::GetInstance().GetNomalDeltaTime();

	}
	else
	{
		m_popUITextTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();
	}

	if (HoldMouse()) colour = { 0.7f, 0.7f, 0.7f, 1.0f };


	m_popUITextTimer = std::min(std::max(0.0f, m_popUITextTimer),1.0f);

	// �G�ꂽ�ۂɏ㕔�ɏo����������p�e�L�X�g�̕`��
	SpriteLoder::TextureData texData = pSL->GetUIText();
	int texWidth = 48;

	RECT textRect = SpriteCutter(texWidth, texData.height, UInumber, 0);
	pSB->Draw(texData.tex.Get(), m_data.pos - SimpleMath::Vector2(0.0f, Easing::EaseInOutQuint(0.0f, texWidth * m_data.rage.y, m_popUITextTimer)),
		&textRect, SimpleMath::Color(0.0f, 0.0f, 0.0f,m_popUITextTimer),
		1.0f - Easing::EaseOutCirc(0.0f, 1.0f, m_popUITextTimer), DirectX::XMFLOAT2(texWidth / 2, texData.height / 2), 1.0f);

	// �I��BOX�`��
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), m_data.pos, &m_rect, colour, 0.0f, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2, static_cast<float>(m_rect.bottom) / 2), m_data.rage);


	// �摜�̃T�C�Y
	RECT srcRect = SpriteCutter(64, 64, UInumber, 0);
	// ����UI�\��
	pSB->Draw(pSL->GetUIIcons().Get(), m_data.pos, &srcRect, SimpleMath::Color(0.0f,0.0f,0.0f,1.0f), 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);


	pSB->End();


}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
						  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture,
						  RECT rect, 
						  SimpleMath::Color pulsTextureCol,
						  SimpleMath::Color nomalColor,
						  SimpleMath::Color onColor,
						  SimpleMath::Color pressdColor)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, 64, 64 };

	// ���O�̐F
	SimpleMath::Color colour = nomalColor;

	if (m_luminousFlag) colour = nomalColor + m_boxColor;

	if (m_hitMouseFlag) colour = onColor;

	if (HoldMouse()) colour = pressdColor;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// �I��BOX
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), m_data.rage);

	// ���ɕ\������e�N�X�`��������ꍇ
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(),box_Pos, &rect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);
	}

	pSB->End();
}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, RECT rect, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture, RECT pulsTexRect, SimpleMath::Color pulsTextureCol, SimpleMath::Color nomalColor, SimpleMath::Color onColor, SimpleMath::Color pressdColor)
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	// ���O�̐F
	SimpleMath::Color colour	= nomalColor;

	if (m_luminousFlag) colour	= nomalColor + m_boxColor;

	if (m_hitMouseFlag) colour	= onColor;

	if (HoldMouse()) colour		= pressdColor;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// �I��BOX
	pSB->Draw(texture.Get(), box_Pos, &rect, colour, 0.0f, DirectX::XMFLOAT2((float)rect.right / 2.0f, (float)rect.bottom / 2.0f), m_data.rage);

	// ���ɕ\������e�N�X�`��������ꍇ
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(), box_Pos, &pulsTexRect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);
	}

	pSB->End();

}

