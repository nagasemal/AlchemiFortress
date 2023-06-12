#include "pch.h"
#include "AM_Mining.h"

AM_Mining::AM_Mining()
{
}

AM_Mining::~AM_Mining()
{
}

void AM_Mining::Initialize()
{
	m_machineID = MACHINE_TYPE::MINING;
	m_objectName = "Mining";
}

void AM_Mining::Update()
{
	m_magicCircle.p = m_data.pos;
}

void AM_Mining::SelectUpdate()
{
}

void AM_Mining::Draw()
{
}

void AM_Mining::Finalize()
{
}

void AM_Mining::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{
}

bool AM_Mining::BulletRequest(std::list<EnemyObject>* enemys)
{
	return false;
}

Bullet AM_Mining::GetBulletData()
{
	return Bullet(0.f, 0.f, 0.f, m_color, m_data.pos, DirectX::SimpleMath::Vector3());
}

void AM_Mining::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}
