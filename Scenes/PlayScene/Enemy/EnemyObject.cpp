#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/Easing.h"

#define GRAVITY 0.2f

EnemyObject::EnemyObject(EnemyType type, DirectX::SimpleMath::Vector3 startPos, int lv) :
	m_power(1),
	m_hp(10),
	m_lv(lv),
	m_speed(6),
	m_accele(),
	m_lengthVec(),
	m_exp(),
	m_stopFlag()
{

	m_data.pos = startPos;
	m_data.rage = DirectX::SimpleMath::Vector3(0.25, 1, 0.25);

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
}

void EnemyObject::Draw()
{

}

void EnemyObject::Render(GeometricPrimitive* geo)
{

	ShareData& pSD = ShareData::GetInstance();

	DirectX::SimpleMath::Matrix textBox = DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	geo->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Black);

}

void EnemyObject::Finalize()
{

}

bool EnemyObject::GotoTarget(DirectX::SimpleMath::Vector3 target)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	if (m_stopFlag) return m_hp <= 0;

	// ���x�̌v�Z
	m_lengthVec = Easing::Moveing(target, m_data.pos);

	m_lengthVec.Normalize();

	// ���W�̌v�Z
	m_data.pos	+= m_lengthVec * m_speed * deltaTime;

	// �d�͌v�Z
	m_data.pos.y -= GRAVITY;

	if (m_data.pos.y <= 0.5f)	m_data.pos.y = 0.5f;

	// �}�l�[�W���[���ŏ����Ă��炤
	return m_hp <= 0;
}

void EnemyObject::HitMachine(bool flag)
{
	// true�Ȃ�Ώ������Ȃ�
	if (m_stopFlag == true) return;

	m_stopFlag = flag;

}

void EnemyObject::Bouns()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_lengthVec.Normalize();

	m_data.pos -= m_lengthVec * m_speed * deltaTime;

}
