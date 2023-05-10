#include "pch.h"
#include "PlayerBase.h"

#define RAGE DirectX::SimpleMath::Vector3(3, 3, 3)

PlayerBase::PlayerBase():
	m_baseLv(),
	m_hp(1),
	m_exp(),
	m_testBox()
{
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::Initialize()
{

	ShareData& pSD = ShareData::GetInstance();

	m_baseLv = 0;
	m_exp = 0;

	m_data.pos  = DirectX::SimpleMath::Vector3::Zero;
	m_data.rage = RAGE;

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), m_data.rage);
}

void PlayerBase::Update()
{
}

void PlayerBase::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	DirectX::SimpleMath::Matrix textBox = DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Chocolate);

}

void PlayerBase::Finalize()
{
}
