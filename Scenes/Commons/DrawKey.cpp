#include "pch.h"
#include "DrawKey.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/SpriteCutter.h"


#define SHIFT_POSITION SimpleMath::Vector2(30,30)

DrawKey::DrawKey(SelectionUI* parent):
    m_parent(parent)
{
}

DrawKey::~DrawKey()
{
}

void DrawKey::Update()
{



}

void DrawKey::Draw()
{
	//�@====================[�@�������^�[���@]
	//�@�@|=>�@���g�����݂��Ȃ�
	if (m_parent->GetKeys().size() == 0) return;

	ShareData& pSD = ShareData::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();
	SpriteLoder* pSL = &SpriteLoder::GetInstance();

	// �A���t�@�x�b�g�\�L
	int keyCord = m_parent->GetKeys()[0] - 0x41;

	//�@====================[�@�A���t�@�x�b�g�Ō���ȓ��@]
	if (keyCord <= Keyboard::Z && InputSupport::GetInstance().GetKeybordState().GetLastState().IsKeyDown(Keyboard::Tab))
	{
		DrawAlphabetKey(keyCord);

		return;
	}

}

bool DrawKey::PushKey()
{
    return false;
}

void DrawKey::DrawAlphabetKey(int cutVal)
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();
	SpriteLoder* pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	RECT rect = { 0,0,64,64 };

	SimpleMath::Color boxColor = (SimpleMath::Color)Colors::Gray;

	if (m_parent->HitMouse())
	{
		boxColor = (SimpleMath::Color)Colors::DarkGray;
	}

	// �I��BOX
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), m_parent->GetPos() + SHIFT_POSITION * m_parent->GetRage(),
		&rect, boxColor, 0.0f, DirectX::XMFLOAT2(static_cast<float>(rect.right) / 2, static_cast<float>(rect.bottom) / 2), 0.38f);

	SpriteLoder::TextureData alphabet = pSL->GetAlphabet();
	rect = SpriteCutter(alphabet.height, alphabet.height, cutVal, 0);

	// �L�[�`��
	pSB->Draw(alphabet.tex.Get(),
			  m_parent->GetPos() + SHIFT_POSITION * m_parent->GetRage(),
			  &rect,
			  Colors::Black,
			  0.0f,
			  DirectX::XMFLOAT2(alphabet.height / 2, alphabet.height / 2),
			  0.38f);

	pSB->End();
}