//--------------------------------------------------------------------------------------
// File: AM_Mining.h
//
//　範囲内の鉱物に対して
//  効果を発揮する
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Mining : public AlchemicalMachineObject
{
public:
	AM_Mining();
	~AM_Mining();

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

	void AllAlchemicalMachine(AlchemicalMachineObject* object)				override;

	bool BulletRequest(std::list<EnemyObject>* enemys)						override;

	void AllFieldObject(FieldObjectManager* fieldManager)					override;

	Bullet GetBulletData() override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	void LvUp() override;

public:

	float m_timer;

private:

};