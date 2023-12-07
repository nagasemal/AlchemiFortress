#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"


#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#include "Scenes/Commons/DrawArrow.h"

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
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_moveTime = 0;

	m_camera = std::make_unique<Camera>();

	//　====================[　マシンUIの情報を取得　]
	UI_Data uiData = pSJD.GetUIData("ExplanationOffset");

	m_data.pos  = uiData.pos;
	m_data.rage = uiData.rage;

	uiData = pSJD.GetUIData("ExplanationSpawn");
	m_spawnBox			= std::make_unique<SelectionBox>(uiData.pos, uiData.rage);
	m_spawnBox			->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationLvUp");
	m_selectLvUpBox		= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_selectLvUpBox		->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationRepair");
	m_repairBox			= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_repairBox			->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationDestory");
	m_dismantlingBox	= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_dismantlingBox	->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationNext");
	m_nextMachineArrow	= std::make_unique<DrawArrow>(uiData.pos, uiData.rage,3);
	m_nextMachineArrow	->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationBack");
	m_backMachineArrow	= std::make_unique<DrawArrow>(uiData.pos, uiData.rage,1);
	m_backMachineArrow	->SetKey(uiData.key);

}

void MachineExplanation::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	//　====================[　UI表示マシンを回転させる　]
	m_moveTime += deltaTime.GetDeltaTime();

	//　====================[　選択ボックスの設定　]
	//　　|=>　設置
	m_spawnBox->HitMouse();
	m_spawnBox->SetActiveFlag(false);
	//　　|=>　LvUP
	m_selectLvUpBox->HitMouse();
	m_selectLvUpBox->SetActiveFlag(false);

	//　　|=>　修繕
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(false);

	//　　|=>　破壊
	m_dismantlingBox->HitMouse();
	m_dismantlingBox->SetActiveFlag(false);

	// 次のマシンへ
	m_nextMachineArrow->HitMouse();

	// 前のマシンへ
	m_backMachineArrow->HitMouse();

}

void MachineExplanation::Update_MachineData(AlchemicalMachineObject* object)
{

	DataManager& pDataM = *DataManager::GetInstance();
	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Noneマシンを選択しているときのみ反応する
	m_spawnBox->SetActiveFlag(object->GetModelID() == MACHINE_TYPE::NONE);

	//　====================[　早期リターン　]
	//　　|=>　選択中がNoneマシン
	if (m_spawnBox->GetActiveFlag()) return;

	// Lvが上限または変更後のクリスタルが0以下
	m_selectLvUpBox->SetActiveFlag(object->GetLv() <= 5 && pDM.GetNowCrystal() - object->GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse())
	{
		object->LvUp();
	}
	// 修繕用の選択ボックスの設定
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - object->GetRepairCrystal() >= 0 && object->GetHP() < object->GetMAXHP());

	// 修繕選択ボックスを押す　現在のCrystal量から修繕に掛かるCrystal量が0以上ならば実行
	if (m_repairBox->ClickMouse())
	{
		object->SetHP(object->GetMAXHP());
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - object->GetRepairCrystal());
	}

	// 破壊用の選択ボックス
	m_dismantlingBox->SetActiveFlag(pDataM.GetNowCrystal() + object->GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX());

	// 破壊選択ボックスを押す　現在のCrystal量から増加するCrystal量が最大値以下ならば実行
	if (m_dismantlingBox->ClickMouse())
	{
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + object->GetDismantlingCrystal());
	}

}

void MachineExplanation::Draw()
{
	// 設置＆ディスプレイ用UI
	m_spawnBox		->DrawUI();

	//　====================[　マシン干渉計UIの表示　]
	//　　|=>　設置＆ディスプレイUIのアクティブがTrueであるときはNoneであるため、それ以外であれば表示を行う
	if (!m_spawnBox->GetActiveFlag())
	{
		// LVUP用UI
		m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

		// 修繕用UI
		m_repairBox->DrawUI(SpriteLoder::REPAIR);

		// 解体用UI
		m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);
	}

	m_nextMachineArrow->Draw();
	m_backMachineArrow->Draw();

}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(0.3f,0.3f, 0.3f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ワールド座標変換
	SimpleMath::Vector3 worldPos = CalcScreenToXZN((int)m_data.pos.x,
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

	case 3:
		//　　|=>　召喚
		return m_spawnBox.get();
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

bool MachineExplanation::GetSpawnFlag()
{
	return m_spawnBox->ClickMouse();
}

bool MachineExplanation::GetNextMachineFlag()
{
	return m_nextMachineArrow->ClickMouse();
}

bool MachineExplanation::GetBackMachineFlag()
{
	return m_backMachineArrow->ClickMouse();;
}
