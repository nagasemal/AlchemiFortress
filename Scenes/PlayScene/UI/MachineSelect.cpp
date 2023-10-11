#include "pch.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define SELECTBOX_RAGE		{ 2.0f,2.0f }
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

	m_selectionAlchemi = std::make_unique<SelectionBox>(SimpleMath::Vector2(m_data.pos.x + ALCHEMI_POS_SHIFT, m_data.pos.y + ALCHEMI_POS_SHIFT),
							 SimpleMath::Vector2(ALCHEMI_RAGE));

	RECT rect = RECT();
	rect.right = 120;
	rect.bottom = 120;

	m_selectionAlchemi->SetRect(rect);
	m_selectionAlchemi->SetRage(SimpleMath::Vector2(ALCHEMI_RAGE));

}

void MachineSelect::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 色を変える
	m_colorChangeTime += deltaTime * 5.0f;
	m_boxColor.G(0.5f + cosf(m_colorChangeTime) / 2);

	// リストの中から選ばれた
	m_onMouseFlag = m_machineBox->HitMouse();
	m_hitMouseFlag = m_machineBox->SelectionMouse();

	// 錬金ボタンが押された
	m_selectionAlchemi->HitMouse();

	m_manufacturingFlag = m_selectionAlchemi->ClickMouse();

	// 選択されているならば、全体の速度を落とす
	if (m_hitMouseFlag)
	{
		DeltaTime::GetInstance().SetDeltaTime(deltaTime / 2);
	}

}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
}

void MachineSelect::DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
								  DirectX::Model* model, SimpleMath::Matrix view,
								  SimpleMath::Matrix proj,
								  DirectX::Model* secondModel)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	SimpleMath::Color colour = SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if (m_changeColorFlag) colour = m_boxColor;
	if(m_onMouseFlag) colour = SimpleMath::Color(0.9f, 0.95f, 0.8f, 0.9f);
	if (m_hitMouseFlag)
	{
		colour = SimpleMath::Color(0.65f, 0.65f, 0.95f, 0.98f);
	}
	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y};

	// 内部BOX (オブジェクトを配置する箇所)
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(IMAGE_WIGHT / 2, IMAGE_HEIGHT / 2), 1.5f);

	pSB->End();

	m_selectionAlchemi->DrawUI(pSL.GetAlchemiButtonTexture().Get(),0.0f);

	//// 必要素材を表示する
	//if (m_hitMouseFlag)
	//{
	//	RECT rect[3] = {SpriteCutter(IMAGE_WIGHT, IMAGE_HEIGHT, 0, 0),						// 必要魔力量
	//					SpriteCutter(IMAGE_WIGHT, IMAGE_HEIGHT, 1, 0),						// 必要結晶数
	//					SpriteCutter(0, 0, m_selectMachineType, 1)};	// 必要魔法
	//	for (int i = 0; i < 3; i++)
	//	{
	//		m_selectionBox[i]->DrawUI(texture.Get(),pSL.GetElementTexture(),rect[i]);
	//	}
	//	m_selectionManufacturing->DrawUI(texture.Get(), pSL.GetManufacturingTexture());
	//}

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(0.35f, 0.35f, 0.35f);

	// 角度調整
	modelData *= SimpleMath::Matrix::CreateRotationX(-20);

	// ワールド座標変換
	SimpleMath::Vector3 worldPos = CalcScreenToXZN(
		(int)m_data.pos.x,
		(int)m_data.pos.y + 30,
		(int)pDR->GetOutputSize().right,
		(int)pDR->GetOutputSize().bottom,
		view,
		proj);

	modelData *= SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([](IEffect* effect)
		{
			// ライト
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, Colors::White);
			lights->SetLightDiffuseColor(1, Colors::White);
			lights->SetLightDiffuseColor(2, Colors::White);

		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, view, proj);

	// セカンドモデルが存在するのならば実行
	if (secondModel != nullptr)
	{
		secondModel->UpdateEffects([&](IEffect* effect)
			{
				// 今回はライトだけ欲しい
				auto lights = dynamic_cast<IEffectLights*>(effect);

				// 色変更
				lights->SetLightDiffuseColor(0, SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
				lights->SetLightDiffuseColor(1, SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
				lights->SetLightDiffuseColor(2, SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
			});

		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, view, proj);
	}
}

