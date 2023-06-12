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

	AM_Attacker();
	~AM_Attacker();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)			override;

	bool BulletRequest(std::list<EnemyObject>* enemys)		override;

	Bullet GetBulletData()				override;

//　アクセサ
public:


private:

	float	m_timer;
	float	m_speed;		// 攻撃の速度

	DirectX::SimpleMath::Vector3 m_targetPos;

	//std::list<Bullet> m_bullets;


};