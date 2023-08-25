#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/Easing.h"

#define GRAVITY 0.2f

EnemyObject::EnemyObject(ENEMY_TYPE type, DirectX::SimpleMath::Vector3 startPos, int lv) :
	m_power(1),
	m_hp(10),
	m_lv(lv),
	m_speed(1),
	m_accele(),
	m_lengthVec(),
	m_exp(),
	m_stopFlag(),
	m_enemyType(type),
	m_rotation(),
	m_moveVec(),
	m_aliveTimer()
{

	m_data.pos = startPos;
	m_data.rage = DirectX::SimpleMath::Vector3(0.25f, 0.25f, 0.25f);

}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::Initialize()
{

}

void EnemyObject::Update()
{
	m_stopFlag = false;

	m_aliveTimer += DeltaTime::GetInstance().GetDeltaTime();

	m_data.rage.y = Easing::EaseInCirc(0.25f, 0.35f, sinf(m_aliveTimer));
	m_data.rage.z = Easing::EaseInCirc(0.25f, 0.35f, cosf(m_aliveTimer));

}

void EnemyObject::Draw()
{

}

void EnemyObject::Render(Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	DirectX::SimpleMath::Matrix modelMatrix = 
		SimpleMath::Matrix::CreateFromQuaternion(m_rotation)
		* DirectX::SimpleMath::Matrix::CreateScale(m_data.rage * 5.0f)
		* DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelMatrix, pSD.GetView(), pSD.GetProjection());

}

void EnemyObject::Finalize()
{

}

bool EnemyObject::GotoTarget(DirectX::SimpleMath::Vector3 target)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	if (m_stopFlag) return m_hp <= 0;

	DirectX::SimpleMath::Vector3 targetDiff = target - m_data.pos;
	targetDiff.y = 0;
	targetDiff.Normalize();

	// 速度の計算
	m_lengthVec = Easing::Moveing(target, m_data.pos);
	m_lengthVec.Normalize();

	// 対象に向ける処理
	m_rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::UnitX, targetDiff);

	// 座標の計算
	m_data.pos	+= m_lengthVec * m_speed * deltaTime;

	// 重力計算
	m_data.pos.y -= GRAVITY;

	if (m_data.pos.y <= 0.0f)	m_data.pos.y = 0.0f;

	// マネージャー側で消してもらう
	return m_hp <= 0;
}

void EnemyObject::HitMachine(bool flag)
{
	// trueならば処理しない
	if (m_stopFlag == true) return;

	m_stopFlag = flag;

}

void EnemyObject::Bouns()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_lengthVec.Normalize();

	m_data.pos -= m_lengthVec * m_speed * deltaTime;

}
