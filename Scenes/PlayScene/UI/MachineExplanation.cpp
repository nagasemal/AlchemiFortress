#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#define MAX_RAGE DirectX::SimpleMath::Vector2(450,300)

#define MINI_BOX_POS DirectX::SimpleMath::Vector2(-135,-135)

MachineExplanation::MachineExplanation():
	m_moveTime(),
	m_modelPos(),
	m_hitFlag()
{
}

MachineExplanation::~MachineExplanation()
{
}

void MachineExplanation::Initialize()
{
	m_moveTime = 0;

	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Log.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	m_camera = std::make_unique<Camera>();

	m_data.pos  = { 180,550};
	m_data.rage = { 300,300};
}

void MachineExplanation::Update()
{
	InputSupport& pIS = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	m_moveTime += deltaTime.GetDeltaTime();

	m_hitFlag = HitObject(pIS.GetMousePosScreen());

}

void MachineExplanation::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = {0, 0, 64, 64 };

	// ログの色
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	// BOX描画
	pSB->Draw(m_texture.Get(),m_data.pos,&srcRect,colour,0.0f,XMFLOAT2(64 / 2, 64 / 2),5.0f);

	DirectX::SimpleMath::Vector2 miniBox_pos = { m_data.pos.x + MINI_BOX_POS.x ,m_data.pos.y + MINI_BOX_POS.y };

	// 内部BOX (オブジェクトを配置する箇所)
	pSB->Draw(m_texture.Get(), miniBox_pos, &srcRect, colour, 0.0f, XMFLOAT2(), 1.5f);

	pSB->End();
}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// モデル情報(位置,大きさ)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f,0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ワールド座標変換
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN(m_data.pos.x - 74.f,
															m_data.pos.y - 64.f,
															pDR->GetOutputSize().right,
															pDR->GetOutputSize().bottom,
															m_camera->GetViewMatrix(),
															m_camera->GetProjectionMatrix());

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ライト
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, object->GetColor());
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());

	// セカンドモデルが存在するならば実行
	if (secondModel != nullptr)
	{

		secondModel->UpdateEffects([&](IEffect* effect)
			{
				// 今回はライトだけ欲しい
				auto lights = dynamic_cast<IEffectLights*>(effect);

				// 色変更
				lights->SetLightDiffuseColor(0, DirectX::SimpleMath::Color((float)object->GetPowerUpFlag(),(float)object->GetPowerUpFlag(), 0.0f, 1.0f));
			});

		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}

}

bool MachineExplanation::OnMouse()
{
	return m_hitFlag;
}

void MachineExplanation::Finalize()
{
	m_texture.Reset();
	m_camera.reset();
}
