#include "pch.h"
#include "Bullet.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

Bullet::Bullet(float speed, float damage, float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetVector)
{
	m_speed = speed;
	m_damage = damage;
	m_life = life;
	m_data.pos = pos;
	m_startPos = pos;
	m_data.rage = DirectX::SimpleMath::Vector3(1, 1, 1);

	m_targetVectol = targetVector;
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{

}

void Bullet::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_life -= deltaTime;

	DirectX::SimpleMath::Vector3 vec = m_targetVectol - m_startPos;

	vec.y = m_data.pos.y;

	m_data.pos += vec * (m_speed * deltaTime);

}

void Bullet::Draw()
{
}

void Bullet::Finalize()
{
}

void Bullet::Render(GeometricPrimitive* geo)
{

	ShareData& pSD = ShareData::GetInstance();

	DirectX::SimpleMath::Matrix textBox = DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, 1 , m_data.pos.z);

	geo->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Red);

}

bool Bullet::deleteRequest()
{
	return m_life <= 0;
}
