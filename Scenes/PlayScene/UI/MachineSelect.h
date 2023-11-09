//--------------------------------------------------------------------------------------
// File		: MachineSelect.h
//
// 選択された際に表示する使用リソースの表示と製造ボタンの処理を行う
// 
// 製造ボタン クリック → MachineSelectManager → AlchemicalMachineManagerのm_AMnums[]を1増やす;
// 
// Use		: MachineSelectManager
// Date		: 2023.5.18
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "SelectionBox.h"
#include "Scenes/PlayScene/UI/Number.h"

class MachineSelect : public GameObjct2D
{
public:
	MachineSelect();
	~MachineSelect();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

	// モデルを描画するための関数
	void DisplayObject();

public:

	// 製造ボタンを押した瞬間を取得
	bool GetManufacturingFlag()										{ return m_manufacturingFlag; }

	bool GetHitMouseFlag()											{ return m_hitMouseFlag; }
	void SetHitMouseFlag(bool flag)									{ m_hitMouseFlag = flag; }

	void SetPosition(SimpleMath::Vector2 pos)						{ m_data.pos = pos; }

	void SetMachineType(MACHINE_TYPE type)							{ m_selectMachineType = type;}
	MACHINE_TYPE GetMachineType()									{ return m_selectMachineType;}

	SelectionBox* GetMachineBox()									{ return m_machineBox.get();}

	void SetChangeColorFlag(bool flag)								{ m_changeColorFlag = flag;}

	// 現在UIが規定位置に存在するか否か
	bool SelectUIFlag();

private:

	bool m_onMouseFlag;
	bool m_hitMouseFlag;
	bool m_manufacturingFlag;

	MACHINE_TYPE m_selectMachineType;

	// 選択ボックス(マシン)
	std::unique_ptr<SelectionBox> m_machineBox;

	// ボックスの色味
	SimpleMath::Color m_boxColor;
	bool m_changeColorFlag;

	// 色を変化させる時間変数
	float m_colorChangeTime;
};