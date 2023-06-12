//--------------------------------------------------------------------------------------
// File: EnemyManager.h
//
// �G�l�~�[�̃}�l�[�W���[
//  
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "EnemyObject.h"
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

	//===��ŏ����e�X�g�p�ϐ�
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  ���u���l�p���f��

	float m_timer;
	float m_totalTimer;

};