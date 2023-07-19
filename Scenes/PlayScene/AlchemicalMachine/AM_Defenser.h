//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//　エネミーからの攻撃を受け止めるアルケミカルマシン
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class EnemyManager;

class AM_Defenser : public AlchemicalMachineObject
{
public:
	AM_Defenser();
	~AM_Defenser();

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

	// エネミーとの当たり判定
	void EnemyHit(std::list<EnemyObject>* enemy);

	//　アクセサ
public:

	// 次回必要となるクリスタルの数
	const int GetNextLvCrystal() const { return (15 * m_lv) * (5 * m_lv); }

private:

	// 壊れているかどうか
	bool m_isBreak;

};