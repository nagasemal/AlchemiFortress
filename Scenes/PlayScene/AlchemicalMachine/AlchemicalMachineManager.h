//--------------------------------------------------------------------------------------
// File: AlchemicalMachineManager.h
//
//　プレイヤーがフィールド内に設置するオブジェクト群のマネージャー
//  モデルのロードや値の受け渡しを担当
// 
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <string>
#include "AM_Attacker.h"
#include "AM_None.h"
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/UI/MachineExplanation.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void ModeLoader();

	void Initialize();

	void Update(bool hitFiledToMouse,bool hitBaseToMouse,MousePointer* pMP);

	void Render();

	void DrawUI();

	void Finalize();


public:

	static const int AM_MAXNUM = 256;

//アクセサ
public:

	bool GetHitMouseFlag(int number) { return m_AMObject[number]->GetHitMouse();}

private:

	// 回す
	void MovingMachine(int number);

private:

	std::unique_ptr<MachineSelectManager> m_selectManager;

	std::unique_ptr<DirectX::Model> m_Model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// アルケミカルマシン確保
	std::unique_ptr<AlchemicalMachineObject> m_AMObject[AM_MAXNUM];

	bool m_allHitObjectToMouse;

	// 現在選択されているオブジェクトの番号
	int m_selectNumber;

};