#include "pch.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define SELECTBOX_RAGE		{ 1.5f,1.5f }
#define ALCHEMI_RAGE		(0.5f,0.5f)
#define ALCHEMI_POS_SHIFT	40

#define IMAGE_WIGHT			64
#define IMAGE_HEIGHT		64

#define BOX_DISTANS_X		68
#define BOX_DISTANS_Y		80

MachineSelect::MachineSelect():
	m_hitMouseFlag(),
	m_onMouseFlag(),
	m_manufacturingFlag(),
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_changeColorFlag(),
	m_colorChangeTime()
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{
	m_data.rage = SELECTBOX_RAGE;

	m_machineBox = std::make_unique<SelectionBox>(m_data.pos, m_data.rage);
	m_machineBox->Initialize();
	m_machineBox->SetColor((SimpleMath::Color)Colors::White);

	m_colorChangeTime = 0;

	RECT rect = RECT();
	rect.right = 120;
	rect.bottom = 120;

}

void MachineSelect::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_manufacturingFlag = false;

	m_machineBox->SetSavePos(m_data.pos);

	// リストの中から選ばれた
	m_machineBox->HitMouse(true);

	if (m_machineBox->ClickMouse())
	{
		m_onMouseFlag = !m_onMouseFlag;
	}

}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
}

void MachineSelect::DisplayObject()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	SimpleMath::Color color = m_machineBox->GetColor();

	// 内部BOX (マシンUIを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();

	srcRect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectMachineType - 1, 0);

	pSB->Draw(texData.tex.Get(), m_data.pos, &srcRect, color, 0.0f, 
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
						  static_cast<float>(texData.height / 2)), 0.4f);
	
	pSB->End();

}

bool MachineSelect::SelectUIFlag()
{
	return m_onMouseFlag;
}

