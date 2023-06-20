#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/Easing.h"

#define GRAVITY 0.2f

EnemyObject::EnemyObject(EnemyType type, DirectX::SimpleMath::Vector3 startPos, int lv) :
	m_power(0),
	m_hp(60),
	m_lv(lv),
	m_speed(2),
	m_accele(),
	m_lengthVec()
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

}

void EnemyObject::Draw()
{

	//ShareData& pSD = ShareData::GetInstance();

	//DirectX::SimpleMath::Matrix textBox = DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	//m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Bisque);
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

	// ���x�̌v�Z
	m_lengthVec = Easing::Moveing(target, m_data.pos) * m_speed;

	// ���W�̌v�Z
	m_data.pos	+= m_lengthVec * deltaTime;

	m_data.pos.y -= GRAVITY;

	if (m_data.pos.y <= 0.5f)m_data.pos.y = 0.5f;

	// �}�l�[�W���[���ŏ����Ă��炤
	return m_hp <= 0;
}
