//--------------------------------------------------------------------------------------
// File: EnemyObject.h
//
//  �G�l�~�[�̓��������@�e�N���X
//  �X�e�[�^�X��ς��邱�ƂŎ�ނ���������
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "NecromaLib/GameData/CommonStruct.h"

class EnemyCommander;
class Enemy_StanderMove;
class Enemy_MeanderingMove;
class ICommand_Enemy;
struct Enemy_Data;

class EnemyObject : public GameObjct3D
{
public:
	EnemyObject(ENEMY_TYPE type, SimpleMath::Vector3 startPos,int lv);
	~EnemyObject();

	// ������
	void Initialize()					override;

	// �X�V
	void Update()						override;

	// �`��@(UI�֌W)
	void Draw()							override;

	void Render(Model* model);

	// �I������
	void Finalize()						override;

	// �v���C���[���_�ւƌ�����
	bool GotoTarget(SimpleMath::Vector3 target);

	// �i�܂Ȃ��悤�ɂ��鏈��
	void HitMachine(bool flag);

	// �o�E���h������
	void Bouns();

// �A�N�Z�T
public:

	// �̗�
	void SetHp(int hp)									{ m_hp = hp; }

	// �^�[�Q�b�g
	void SetTargetPos(SimpleMath::Vector3 targetPos) { m_targetPos = targetPos; }

	// �ړI�n�܂ł̋���
	void SetLengthVec(SimpleMath::Vector3 lengthVec) { m_lengthVec = lengthVec; }

	// �~�܂鏈��
	void SetStopFlag(bool flag)							{ m_stopFlag = flag;}

	// �X�e�[�^�X��ݒ肷��
	void SetEnemyData(Enemy_Data data);

	// ���x��
	const int GetLv()									{ return m_lv; }
	// �̗�
	const int GetHp()									{ return m_hp; }
	// EXP
	const int GetEXP()									{ return m_exp; }
	// �p���[	
	const float GetPower()								{ return m_power;}

	// �Ƃ܂��Ă���t���O
	const bool GetStopFlag()							{ return m_stopFlag; }

	const SimpleMath::Vector3 GetTargetPos()	{ return m_targetPos;}
	// �ړI�n�܂ł̋���
	const SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// �����x
	const SimpleMath::Vector3 GetAccele()		{ return m_accele;}
	// ���g�̃G�l�~�[�^�C�v
	const ENEMY_TYPE GetEnemyType()						{ return m_enemyType; }

	EnemyObject* GetEnemyObject()						{ return this;}

private:

	// �U����
	float m_power;
	// HP
	int m_hp;
	// ���݃��x��
	int m_lv;
	// �o���l
	int m_exp;
	// �������Ă��鎞��
	float m_aliveTimer;
	// �ړ����~�߂�
	bool m_stopFlag;
	// �����̎�ށ@ALL �L�q����Ă��铮����S�ē����ɓ������@ONE ��������𓮂���
	std::string m_moveType;
	// ���g�̃^�C�v
	ENEMY_TYPE m_enemyType;

	// �����x
	SimpleMath::Vector3 m_accele;

	SimpleMath::Vector3 m_moveVec;

	SimpleMath::Quaternion m_rotation;

	// �ړI�n�܂ł̋���
	SimpleMath::Vector3 m_lengthVec;

	// �ڕW�̈ʒu
	SimpleMath::Vector3 m_targetPos;

	std::vector<ICommand_Enemy*> m_moveCommands;
	// �����̃f�[�^������R�}���h
	std::shared_ptr<EnemyCommander> m_commander;



};