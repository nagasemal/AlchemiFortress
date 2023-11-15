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

	//m_selectionAlchemi = std::make_unique<SelectionBox>(SimpleMath::Vector2(m_data.pos.x + ALCHEMI_POS_SHIFT, m_data.pos.y + ALCHEMI_POS_SHIFT),
	//						 SimpleMath::Vector2(ALCHEMI_RAGE));

	RECT rect = RECT();
	rect.right = 120;
	rect.bottom = 120;

	//m_selectionAlchemi->SetRect(rect);
	//m_selectionAlchemi->SetRage(SimpleMath::Vector2(ALCHEMI_RAGE));
	//m_selectionAlchemi->SetLayer(1);

}

void MachineSelect::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_manufacturingFlag = false;

	m_machineBox->SetSavePos(m_data.pos);

	// �F��ς���
	m_colorChangeTime += deltaTime * 5.0f;
	m_boxColor.G(0.5f + cosf(m_colorChangeTime) / 2);


	// �B���{�^���������ꂽ
	//m_selectionAlchemi->HitMouse(true);

	// ���X�g�̒�����I�΂ꂽ
	m_onMouseFlag = m_machineBox->HitMouse(true);
	m_hitMouseFlag = m_machineBox->SelectionMouse();

	//�B���{�^���������ꂽ��B
	//if (m_selectionAlchemi->ClickMouse())
	//{
	//	m_manufacturingFlag = true;
	//
	//	// �I����ԂłȂ���ΑI����Ԃɂ���
	//	if (!m_hitMouseFlag) m_machineBox->SetSelectFlag(true);
	//
	//}

	//// �I������Ă���Ȃ�΁A�S�̂̑��x�𗎂Ƃ�
	//if (m_hitMouseFlag)
	//{
	//	DeltaTime::GetInstance().SetDoubleSpeed(0.5f);
	//}

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

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ���O�̐F
	SimpleMath::Color color = SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if (m_changeColorFlag)	color = m_boxColor;
	if (m_onMouseFlag)		color = SimpleMath::Color(0.9f, 0.95f, 0.8f, 0.9f);
	if (m_hitMouseFlag)		color = SimpleMath::Color(0.65f, 0.65f, 0.95f, 0.98f);

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	//pSB->Draw(pSL.GetSelectBoxTexture().Get(), box_Pos, &srcRect, color, 0.0f, DirectX::XMFLOAT2(IMAGE_WIGHT / 2, IMAGE_HEIGHT / 2), 1.5f);

	srcRect = SpriteCutter(200,200,m_selectMachineType - 1,0);

	pSB->Draw(pSL.GetMachineIconTexture().Get(), m_data.pos, &srcRect, color, 0.0f, DirectX::XMFLOAT2(200 / 2, 200 / 2),0.4f);
	
	pSB->End();

}

bool MachineSelect::SelectUIFlag()
{

	bool flag = false;
	
	flag = m_data.pos.x <= 1095.0f && m_data.pos.y <= 600.0f;

	return flag;

}

