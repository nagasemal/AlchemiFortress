#include "pch.h"
#include "AlchemicalMachineFilter.h"

AlchemicalMachineFilter::AlchemicalMachineFilter():
	m_model{}
{
	// ƒ‚ƒfƒ‹Žæ“¾
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_model[AlchemicalMachineObject::MACHINE_TYPE::NONE] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Siroma.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::ATTACKER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::DEFENSER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::MINING] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::RECOVERY] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);

	m_model[AlchemicalMachineObject::MACHINE_TYPE::UPEER] = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *fx);
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

AlchemicalMachineObject* AlchemicalMachineFilter::HandOverAMClass(AlchemicalMachineObject::MACHINE_TYPE type)
{
	switch (type)
	{
	case AlchemicalMachineObject::NONE:
		return new AM_None;
	case AlchemicalMachineObject::ATTACKER:
		return new AM_Attacker;
	case AlchemicalMachineObject::DEFENSER:
		return new AM_Defenser;
	case AlchemicalMachineObject::MINING:
		return new AM_Mining;
	case AlchemicalMachineObject::RECOVERY:
		return new AM_Recovery;
	case AlchemicalMachineObject::UPEER:
		return new AM_Upper;
	default:
		return new AM_None;
		break;
	}

}
