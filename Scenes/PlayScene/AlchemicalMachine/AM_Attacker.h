//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//　エネミーを攻撃するアルケミカルマシン
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include <list>


class AM_Attacker : public AlchemicalMachineObject
{
public:

	AM_Attacker(MACHINE_ELEMENT element);
	~AM_Attacker();

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

	void AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)			override;

	bool BulletRequest(std::list<EnemyObject>* enemys)		override;

	Bullet GetBulletData()				override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

//　アクセサ
public:


private:

	float	m_timer;
	float	m_speed;		// 攻撃の速度
	float	m_power;		// 攻撃の威力
	float	m_bulletlife;	// 弾の命

	DirectX::SimpleMath::Vector3 m_targetPos;
	
	MACHINE_ELEMENT m_element;

	//std::list<Bullet> m_bullets;

	// 選択可能オブジェクト
	std::unique_ptr<SelectionBox> m_selectBox[4];
};