#include "pch.h"
#include "Enemy_MeanderingMove.h"

#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/DeltaTime.h"

Enemy_MeanderingMove::Enemy_MeanderingMove() :
	m_completion(),
	m_time(),
	m_enemyPtr(nullptr),
	m_param()
{
}

Enemy_MeanderingMove::~Enemy_MeanderingMove()
{
}

float Map(float value, float start1, float stop1, float start2, float stop2)
{
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

void Enemy_MeanderingMove::Execute()
{
	m_time += DeltaTime::GetInstance().GetDeltaTime();

	// ディレイ時間が来るまでは処理を行わない
	if (m_time <= m_param.delay || m_enemyPtr->GetStopFlag()) return;

	// 速度の計算
	SimpleMath::Vector3 lengthVec = SimpleMath::Vector3();

	lengthVec.x = /*cosf(m_time) / m_param.value*/ Map(cosf(m_time), -1.0f, 1.0f, 0.0f, 1.0f);
	lengthVec.z = /*sinf(m_time) / m_param.value*/ Map(sinf(m_time), -1.0f, 1.0f, 0.0f, 1.0f);
	lengthVec.Normalize();

	m_enemyPtr->SetLengthVec(m_enemyPtr->GetLengthVec() + lengthVec * m_param.value);

	// 稼働完了
	m_completion = m_time >= m_param.time;
}

MoveParameter Enemy_MeanderingMove::GetParam()
{
	return m_param;
}

bool Enemy_MeanderingMove::GetCompletion()
{
	return m_completion;
}

void Enemy_MeanderingMove::SetCompletion(bool flag)
{
	m_completion = flag;
}


void Enemy_MeanderingMove::SetParam(MoveParameter param)
{
	m_param.delay = param.delay;
	m_param.time = param.time;
	m_param.value = param.value;
}

void Enemy_MeanderingMove::SetEnemyPtr(EnemyObject& object)
{
	m_enemyPtr = &object;
}

void Enemy_MeanderingMove::Reset()
{

	m_time = 0.0f;

}
