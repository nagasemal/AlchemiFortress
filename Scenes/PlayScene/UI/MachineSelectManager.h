//--------------------------------------------------------------------------------------
// File		: MachineSelectManager.h
//
// アルケミカルマシン関連のUIに関わるクラスを内包
// 
// Use		: AlchemicalMachineManager
// Date		: 2023.5.18
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineSelect.h"
#include "NecromaLib/GameData/Camera.h"

#include "NecromaLib/GameData/CommonStruct.h"

class FieldObjectManager;
class DisplayMagicCircle;
class UIKeyControl;
class Number;

class MachineSelectManager
{
public:
	MachineSelectManager();
	~MachineSelectManager();

	void TextuerLoader();

	void Initialize();

	// fieldObjecytManager = UIに変化を与えるオブジェクトの検出(クリスタル)
	void Update(FieldObjectManager* fieldObjectManager);

	void Render();

	void RenderUI(int machineNum[]);

	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="index">インデックス番号(マシンタイプID)</param>
	void ModelRender(int index);

	void MagicCircleRender();

	void Finalize();

	// マシンアイコンを回す
	void RotationMachineList();

public:

	void SetSelectNoneFlag(bool flag) { m_selectNoneFlag = flag; }

	// 選択中のマシンのタイプを返す
	MACHINE_TYPE GetSelectMachineType()	{ return m_selectMachineType; }


	int GetMachineUISelectNumber() { return m_selectNumber; }

	// 選択ボックスが押されているかをTypeに応じて返す
	bool GetHitMouseToSelectBox(int index)							{ return m_machineSelect[index]->GetHitMouseFlag();}

	// 選択ボックスをTypeに応じて返す
	MachineSelect* GetMachineSelect(int index)						{ return m_machineSelect[index].get();}

	bool GetManufacturingFlag()										{ return m_manufacturingFlag;}

	// 一つの要素でもtrueなら
	bool GetHitMouseToSelectBoxEven()								{ return m_selectBoxAll;}

private:

	// リソース群を減らす
	void ReduceResource(MACHINE_TYPE type);

private:

	std::unique_ptr<MachineSelect> m_machineSelect[MACHINE_TYPE::NUM];

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

	// 所持数を示す
	std::unique_ptr<Number> m_machineNumRender;

	MACHINE_TYPE m_selectMachineType;

	bool m_selectBoxAll;
	bool m_manufacturingFlag;

	// フィールド上のNoneを選択した際に、全てのMachineSelectの選択Boxの色を変更する
	bool m_selectNoneFlag;

	// マシンの名前表示
	std::unique_ptr<SelectionBox> m_machineName;

	// 選択ボックス(錬金ボタン用)
	std::unique_ptr<SelectionBox> m_selectionAlchemi;

	// 選択ボックス(UI選択用)
	std::unique_ptr<SelectionBox> m_selectionLeft,m_selectionRight;

	//画面に大きく出現させる魔法陣
	std::unique_ptr<DisplayMagicCircle>			m_displayMagicCircle;

	// アイコンの回転を行う時間変数
	float m_rotateTime;

	// 選択しているマシンの番号
	int m_selectNumber;

	// 前Frameのマシンの番号
	int m_prevSelectNumber;

	// カーソルにあっているマシンの番号
	int m_cursorMachineNumber;

	// UI回転が順転か否か
	bool m_forwardOrderFlag;

};