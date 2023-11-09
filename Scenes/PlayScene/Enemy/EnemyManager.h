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
#include "Scenes/PlayScene/Shadow/Particle.h"
#include <list>

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void Update(SimpleMath::Vector3 basePos);
	void Render();
	void Finalize();

	// �G�l�~�[�̃f�[�^���ēx�ǂݎ��
	void ReloadEnemyData();

	EnemyObject GetEnemyStatus(ENEMY_TYPE type,int spawnNumber);

	EnemyObject GetRandomEnemy();

	void HitAMObejct(AlchemicalMachineObject* alchemicalMachines);

	int GetNockDownEnemyExp();

public:


	const int GetKnokDownEnemyFlag() const { return m_knokDownFlag; }
	const ENEMY_TYPE GetKnokDownEnemyType() const { return m_knokDownEnemyType; }
	std::list<EnemyObject>* GetEnemyData() { return m_enemyObject.get(); }

private:

	std::unique_ptr<std::list<EnemyObject>> m_enemyObject;

	//===��ŏ����e�X�g�p�ϐ�
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  ���u���l�p���f��

	std::unique_ptr<DirectX::Model> m_enemyModel;

	// �p�[�e�B�N���N���X
	std::unique_ptr<Particle> m_particle_spawn;
	std::unique_ptr<Particle> m_particle_delete;

	// ���E���h�i�s
	float m_nextEnemyTime;

	float m_timer;
	float m_totalTimer;

	// �|���ꂽ�u�Ԃ��擾����t���O(1F���ɕ����̓|���Ɛ���ȏ������s���Ȃ��ׁAint�^�ŊǗ�)
	int m_knokDownFlag;
	ENEMY_TYPE m_knokDownEnemyType;

	// 1F�Ԃɉ҂���EXP�̑���
	int m_falmeTotalEnemyExp;

	int m_enemyNums;

};