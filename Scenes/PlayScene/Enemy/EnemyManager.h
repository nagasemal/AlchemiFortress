//--------------------------------------------------------------------------------------
// File: EnemyManager.h
//
// エネミーのマネージャー
//  
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "EnemyObject.h"
#include "Scenes/PlayScene/Shadow/Particle.h"
#include <list>

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void Update(DirectX::SimpleMath::Vector3 basePos);
	void Render();
	void Finalize();

	EnemyObject GetEnemyStatus(EnemyObject::EnemyType type);

	void HitAMObejct(AlchemicalMachineObject* alchemicalMachines);

public:

	std::list<EnemyObject>* GetEnemyData() { return m_enemyObject.get(); }

private:

	std::unique_ptr<std::list<EnemyObject>> m_enemyObject;

	//===後で消すテスト用変数
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  仮置き四角モデル

	// パーティクルクラス
	std::unique_ptr<Particle> m_particle_spawn;
	std::unique_ptr<Particle> m_particle_delete;

	// ラウンド進行
	float m_nextEnemyTime;

	float m_timer;
	float m_totalTimer;

};