#include "pch.h"
#include "Enemy_HoppingMove.h"

#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/DeltaTime.h"

Enemy_HoppingMove::Enemy_HoppingMove() :
	m_completion(),
	m_time(),
	m_enemyPtr(nullptr),
	m_param()
{
}

Enemy_HoppingMove::~Enemy_HoppingMove()
{
}

void Enemy_HoppingMove::Execute()
{
	m_time += DeltaTime::GetInstance().GetDeltaTime();

	// ディレイ時間が来るまでは処理を行わない
	if (m_time <= m_param.delay || m_enemyPtr->GetStopFlag()) return;

	// 速度の計算
	SimpleMath::Vector3 lengthVec = SimpleMath::Vector3();

	lengthVec.y = m_hoppingVal;
	//lengthVec.Normalize();

	m_hoppingVal -= m_time;

	m_enemyPtr->SetLengthVec(lengthVec * m_param.value);

	// 稼働完了
	m_completion = m_time >= m_param.time;

	if (m_hoppingVal <= 0)
	{
		m_hoppingVal = m_param.value;
	}
}

MoveParameter Enemy_HoppingMove::GetParam()
{
	return m_param;
}

bool Enemy_HoppingMove::GetCompletion()
{
	return m_completion;
}

void Enemy_HoppingMove::SetCompletion(bool flag)
{
	m_completion = flag;
}


void Enemy_HoppingMove::SetParam(MoveParameter param)
{
	m_param.delay = param.delay;
	m_param.time = param.time;
	m_param.value = param.value;

}

void Enemy_HoppingMove::SetEnemyPtr(EnemyObject& object)
{
	m_enemyPtr = &object;
}

void Enemy_HoppingMove::Reset()
{
	m_hoppingVal = m_param.value;
	m_time = 0.0f;

}
