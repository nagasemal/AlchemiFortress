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
	m_boxColor{1.0f,1.0f,1.0f,1.0f},
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

	// 結晶選択時色を変える
	m_colorChangeTime += deltaTime * 5.0f;
	m_boxColor.G(0.5f + cosf(m_colorChangeTime) / 2);

	// リストの中から選ばれた
	if (m_machineBox->HitMouse(true))
	{
		m_boxColor = Colors::Gray;
	}

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
	SimpleMath::Color color = SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if (m_changeColorFlag)	color = m_boxColor;
	if (m_onMouseFlag)		color = SimpleMath::Color(0.9f, 0.95f, 0.8f, 0.9f);
	if (m_hitMouseFlag)		color = SimpleMath::Color(0.65f, 0.65f, 0.95f, 0.98f);

	// 内部BOX (オブジェクトを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();

	srcRect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectMachineType - 1, 0);

	pSB->Draw(texData.tex.Get(), m_data.pos, &srcRect, color, 0.0f, 
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
						  static_cast<float>(texData.height / 2)), 0.4f);
	
	pSB->End();

}

bool MachineSelect::SelectUIFlag()
{

	bool flag = false;
	
	flag = m_data.pos.x <= 1095.0f && m_data.pos.y <= 600.0f;

	return flag;

}

