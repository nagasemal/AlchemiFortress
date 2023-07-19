//--------------------------------------------------------------------------------------
// File: AM_Upper.h
//
//　範囲内のアルケミカルマシンに対して
//  強化効果を発揮する
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Upper : public AlchemicalMachineObject
{
public:
	AM_Upper();
	~AM_Upper();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 選択時に更新される処理
	void SelectUpdate()					override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	void LvUp() override;

public:

	// 次回必要となるクリスタルの数
	const int GetNextLvCrystal() const { return (15 * m_lv) * (5 * m_lv); }

private:

	// 選択可能オブジェクト
	std::unique_ptr<SelectionBox> m_selectBox[4];

};