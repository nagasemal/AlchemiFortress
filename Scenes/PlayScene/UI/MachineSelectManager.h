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

	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="model">ベースとなるモデル</param>
	/// <param name="index">インデックス番号(マシンタイプID)</param>
	/// <param name="secondModel">第二のモデル</param>
	void ModelRender(DirectX::Model* model,int index,DirectX::Model* secondModel = nullptr);

	void MagicCircleRender();

	void Finalize();

	void SetSelectNoneFlag(bool flag)								{ m_selectNoneFlag = flag; }

public:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextuer()	{ return m_boxTextuer;}

	// 選択中のマシンのタイプを返す
	MACHINE_TYPE GetSelectMachineType()	{ return m_selectMachineType; }

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

	// モデルを入れる箱のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_boxTextuer;

	std::unique_ptr<MachineSelect> m_machineSelect[MACHINE_TYPE::NUM];

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

	MACHINE_TYPE m_selectMachineType;

	bool m_selectBoxAll;
	bool m_manufacturingFlag;

	// フィールド上のNoneを選択した際に、全てのMachineSelectの選択Boxの色を変更する
	bool m_selectNoneFlag;

	std::unique_ptr<Camera> m_camera;

	DirectX::SimpleMath::Matrix m_dispView;
	DirectX::SimpleMath::Matrix m_dispProj;

	//画面に大きく出現させる魔法陣
	std::unique_ptr<DisplayMagicCircle>			m_displayMagicCircle;

};