//--------------------------------------------------------------------------------------
// File: AlchemicalMachineFilter.h
//
//　受け取った引数に応じてモデルやクラスを渡すクラス
// 
// Date: 2023.6.5
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

class AlchemicalMachineFilter
{
public:
	AlchemicalMachineFilter();
	~AlchemicalMachineFilter();

	/// <summary>
    /// マシンタイプに応じてモデルを渡す
    /// </summary>
    /// <param name="type">アルケミカルマシンのタイプ</param>
    /// <returns></returns>
	DirectX::Model* HandOverAMModel(AlchemicalMachineObject::MACHINE_TYPE type);

	/// <summary>
	/// マシンタイプに応じてAlchemicalMachineObjectを継承したクラスを渡す
	/// </summary>
	/// <param name="type">アルケミカルマシンのタイプ</param>
	/// <returns></returns>
	AlchemicalMachineObject* HandOverAMClass(AlchemicalMachineObject::MACHINE_TYPE type);

private:

	std::unique_ptr<DirectX::Model> m_model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

};