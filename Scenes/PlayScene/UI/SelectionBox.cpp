#include "pch.h"
#include "SelectionBox.h"
#include "NecromaLib/Singleton/InputSupport.h"

SelectionBox::SelectionBox(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 rage)
{

	m_data.pos = pos;
	m_data.rage = rage;

	m_hitMouseFlag = false;

}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::Initialize()
{
}

void SelectionBox::Update()
{

}

void SelectionBox::Draw()
{

}

void SelectionBox::Finalize()
{
	delete this;
}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
						  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture,
						  RECT rect)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, 64, 64 };

	// ログの色
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if (m_hitMouseFlag) colour = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.8f, 0.9f);

	DirectX::SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 1.0f);

	// 中に表示するテクスチャがある場合
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(), box_Pos, &rect, colour, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);
	}

	pSB->End();
}

bool SelectionBox::HitMouse()
{

	InputSupport& pIS = InputSupport::GetInstance();

	return m_hitMouseFlag = HitObject(pIS.GetMousePosScreen());
}

bool SelectionBox::ClickMouse()
{
	InputSupport& pIS = InputSupport::GetInstance();
	bool leftFlag = pIS.GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED;

	return m_hitMouseFlag && leftFlag;
}

bool SelectionBox::ExitMouse()
{
	InputSupport& pIS = InputSupport::GetInstance();
	bool exitFlag = false;

	if (m_hitMouseFlag)
	{
		exitFlag = !HitObject(pIS.GetMousePosScreen());
	}

	return exitFlag;
}
