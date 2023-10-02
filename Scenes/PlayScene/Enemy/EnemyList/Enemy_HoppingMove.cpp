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

	lengthVec.x = cosf(m_time) / m_param.value;//Map(cosf(m_time), -1.0f, 1.0f, 0.0f, 1.0f);
	lengthVec.z = sinf(m_time) / m_param.value;//Map(sinf(m_time), -1.0f, 1.0f, 0.0f, 1.0f);
	lengthVec.Normalize();

	m_enemyPtr->SetLengthVec(m_enemyPtr->GetLengthVec() + lengthVec * m_param.value);

	// 稼働完了
	m_completion = m_time >= m_param.time;
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

	m_time = 0.0f;

}
