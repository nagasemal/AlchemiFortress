#include "pch.h"
#include "AlchemicalMachineFilter.h"
#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

#include "NecromaLib/GameData/CommonStruct.h"

AlchemicalMachineFilter::AlchemicalMachineFilter():
	m_model{}
{
	// モデル取得
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_model[MACHINE_TYPE::NONE] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_None.cmo", *fx);

	m_model[MACHINE_TYPE::ATTACKER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Attacker.cmo", *fx);

	m_model[MACHINE_TYPE::DEFENSER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Defencer.cmo", *fx);

	m_model[MACHINE_TYPE::MINING] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Mining.cmo", *fx);

	m_model[MACHINE_TYPE::RECOVERY] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Excavator.cmo", *fx);

	m_model[MACHINE_TYPE::UPPER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_UpperCore.cmo", *fx);


	// サブモデル
	m_subModel[MACHINE_TYPE::ATTACKER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Ring.cmo", *fx);

	m_subModel[MACHINE_TYPE::UPPER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Upper.cmo", *fx);

	// エフェクトの設定
	m_subModel[MACHINE_TYPE::UPPER]->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
			lights->SetLightDiffuseColor(1, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
			lights->SetLightDiffuseColor(2, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

		});



}

AlchemicalMachineFilter::~AlchemicalMachineFilter()
{

}

//AlchemicalMachineObject* alchemicalMachine[AlchemicalMachineObject::MACHINE_TYPE::NUM]
//{
//	new AM_None,
//	new AM_Attacker,
//	new AM_Defenser,
//	new AM_Mining,
//	new AM_Recovery,
//	new AM_Upper,
//};

Model* AlchemicalMachineFilter::HandOverAMModel(MACHINE_TYPE type)
{
	return m_model[type].get();
}

std::unique_ptr<AlchemicalMachineObject> AlchemicalMachineFilter::HandOverAMClass(MACHINE_TYPE type)
{
	switch (type)
	{
	case MACHINE_TYPE::NONE:
		return std::make_unique<AM_None>();
	case MACHINE_TYPE::ATTACKER:
		return std::make_unique<AM_Attacker>();
	case MACHINE_TYPE::DEFENSER:
		return std::make_unique<AM_Defenser>();
	case MACHINE_TYPE::MINING:
		return std::make_unique<AM_Mining>();
	case MACHINE_TYPE::RECOVERY:
		return std::make_unique<AM_Recovery>();
	case MACHINE_TYPE::UPPER:
		return std::make_unique<AM_Upper>();
	default:
		return std::make_unique<AM_None>();
		break;
	}

}

Model* AlchemicalMachineFilter::GetRingModel(MACHINE_TYPE type)
{

	//if (type != MACHINE_TYPE::ATTACKER) return nullptr;

	return m_subModel[type].get();
}

void AlchemicalMachineFilter::Finalize()
{
	m_model[MACHINE_TYPE::NONE].reset();

	m_model[MACHINE_TYPE::ATTACKER].reset();

	m_model[MACHINE_TYPE::DEFENSER].reset();

	m_model[MACHINE_TYPE::MINING].reset();

	m_model[MACHINE_TYPE::RECOVERY].reset();

	m_model[MACHINE_TYPE::UPPER].reset();
	
	m_model->reset();
}
