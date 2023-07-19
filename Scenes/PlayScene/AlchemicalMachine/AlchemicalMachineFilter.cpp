#include "pch.h"
#include "AlchemicalMachineFilter.h"
#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

AlchemicalMachineFilter::AlchemicalMachineFilter():
	m_model{}
{
	// モデル取得
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_model[AlchemicalMachineObject::MACHINE_TYPE::NONE] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_None.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Attacker.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Defencer.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::MINING] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Mining.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::RECOVERY] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Attacker.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::UPEER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Upper.cmo", *fx);


	// サブモデル
	m_subModel[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Ring.cmo", *fx);

	m_subModel[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Ring.cmo", *fx);
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

Model* AlchemicalMachineFilter::HandOverAMModel(AlchemicalMachineObject::MACHINE_TYPE type)
{
	return m_model[type].get();
}

std::unique_ptr<AlchemicalMachineObject> AlchemicalMachineFilter::HandOverAMClass(AlchemicalMachineObject::MACHINE_TYPE type)
{
	switch (type)
	{
	case AlchemicalMachineObject::NONE:
		return std::make_unique<AM_None>();
	case AlchemicalMachineObject::ATTACKER:
		return std::make_unique<AM_Attacker>();
	case AlchemicalMachineObject::DEFENSER:
		return std::make_unique<AM_Defenser>();
	case AlchemicalMachineObject::MINING:
		return std::make_unique<AM_Mining>();
	case AlchemicalMachineObject::RECOVERY:
		return std::make_unique<AM_Recovery>();
	case AlchemicalMachineObject::UPEER:
		return std::make_unique<AM_Upper>();
	default:
		return std::make_unique<AM_None>();
		break;
	}

}

Model* AlchemicalMachineFilter::GetRingModel(AlchemicalMachineObject::MACHINE_TYPE type)
{

	if (type != AlchemicalMachineObject::ATTACKER) return nullptr;

	return m_subModel[type].get();
}

void AlchemicalMachineFilter::Finalize()
{
	m_model[AlchemicalMachineObject::MACHINE_TYPE::NONE].reset();

	m_model[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER].reset();

	m_model[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER].reset();

	m_model[AlchemicalMachineObject::MACHINE_TYPE::MINING].reset();

	m_model[AlchemicalMachineObject::MACHINE_TYPE::RECOVERY].reset();

	m_model[AlchemicalMachineObject::MACHINE_TYPE::UPEER].reset();
	
	m_model->reset();
}
