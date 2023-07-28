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

	EnemyObject GetEnemyStatus(ENEMY_TYPE type,int spawnNumber);

	void HitAMObejct(AlchemicalMachineObject* alchemicalMachines);

public:


	const bool GetKnokDownEnemyFlag() const { return m_knokDownFlag; }
	const ENEMY_TYPE GetKnokDownEnemyType() const { return m_knokDownEnemyType; }

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

	// 倒された瞬間を取得するフラグ
	bool m_knokDownFlag;
	ENEMY_TYPE m_knokDownEnemyType;

	int m_enemyNums;

};