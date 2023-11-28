#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"


#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#define MAX_RAGE SimpleMath::Vector2(450,300)

#define MINI_BOX_POS SimpleMath::Vector2(-85,-95)

#define BIG_BOX_RAGEPERCENT SimpleMath::Vector2(5.0f,0.5f)


// アイコンを設置する箇所
#define ICON_POS SimpleMath::Vector2(100,600)

// アイコンを並べる間隔
#define ICON_OFFSET_X 60.0f

struct position2D
{
	double x;
	double y;
};

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

	m_data.pos  = { 150,680};
	m_data.rage = { 150,150};

	//m_machineGauge = std::make_unique<MachineGauge>();
	//m_machineGauge->AddHPGauge({m_data.pos.x + 60,m_data.pos.y - 130}, { 0.20,0.20 }, UserInterface::MIDDLE_CENTER);

	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X * 2, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X * 3, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

}

void MachineExplanation::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	m_moveTime += deltaTime.GetDeltaTime();

	m_hitFlag = HitObject_RageSet(pINP.GetMousePosScreen(),64,64, BIG_BOX_RAGEPERCENT);






}

void MachineExplanation::Update_MachineData(AlchemicalMachineObject* object)
{

	DataManager& pDataM = *DataManager::GetInstance();

	// LvUp用の選択ボックスの設定
	m_selectLvUpBox->HitMouse();

	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	m_selectLvUpBox->SetActiveFlag(object->GetLv() <= 5 && pDM.GetNowCrystal() - object->GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse()) 		object->LvUp();

	// 修繕用の選択ボックスの設定
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - object->GetRepairCrystal() >= 0 && object->GetHP() < object->GetMAXHP());

	// 修繕選択ボックスを押す　現在のCrystal量から修繕に掛かるCrystal量が0以上ならば実行
	if (m_repairBox->ClickMouse())
	{
		object->SetHP(object->GetMAXHP());
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - object->GetRepairCrystal());
	}

	// 破壊用の選択ボックス
	m_dismantlingBox->HitMouse();
	m_dismantlingBox->SetActiveFlag(pDataM.GetNowCrystal() + object->GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX());

	// 破壊選択ボックスを押す　現在のCrystal量から増加するCrystal量が最大値以下ならば実行
	if (m_dismantlingBox->ClickMouse())
	{
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + object->GetDismantlingCrystal());
	}


}

void MachineExplanation::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = {0, 0, 64, 64 };

	// ログの色
	SimpleMath::Color colour = SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	// BOX描画

	SimpleMath::Vector2 miniBox_pos = { m_data.pos.x + MINI_BOX_POS.x ,m_data.pos.y + MINI_BOX_POS.y };

	// 内部BOX (オブジェクトを配置する箇所)
	pSB->Draw(m_texture.Get(), miniBox_pos, &srcRect, colour, 0.0f, XMFLOAT2(64 / 2,64 / 2), 1.5f);

	pSB->End();

	// LVUP用UI
	m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

	// 修繕用UI
	m_repairBox->DrawUI(SpriteLoder::REPAIR);

	// 解体用UI
	m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);
}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(0.35f,0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ワールド座標変換
	SimpleMath::Vector3 worldPos = CalcScreenToXZN((int)m_data.pos.x - 86,
															(int)m_data.pos.y,
															pDR->GetOutputSize().right,
															pDR->GetOutputSize().bottom,
															m_camera->GetViewMatrix(),
															m_camera->GetProjectionMatrix());

	worldPos.z = 4.25f;

	modelData *= SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ライト
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, object->GetColor());
			lights->SetLightDiffuseColor(1, object->GetColor());
			lights->SetLightDiffuseColor(2, object->GetColor());
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), false,[&]
		{

			ModelShader::GetInstance().MachineDrawShader(object->GetColor(), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(object->GetElement()).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(object->GetElement()).GetAddressOf());

		});

	// セカンドモデルが存在するならば実行
	if (secondModel != nullptr)
	{
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

SelectionBox* MachineExplanation::GetMenuButton(int buttonType)
{
	//　====================[　番号に則した選択ボックスを返す　]
	switch (buttonType)
	{
	case 0:
		//　　|=>　修繕
		return m_repairBox.get();

	case 1:
		//　　|=>　破壊
		return m_dismantlingBox.get();

	case 2:
		//　　|=>　強化
		return m_selectLvUpBox.get();
	}

	// それ以外の場合は修繕を選択する
	return m_repairBox.get();

}

bool MachineExplanation::GetDismantlingFlag()
{
	return m_dismantlingBox->ClickMouse();
}


bool MachineExplanation::GetRepairFlag()
{
	return m_repairBox->ClickMouse();
}

bool MachineExplanation::GetLvUpFlag()
{
	return m_selectLvUpBox->ClickMouse();
}
