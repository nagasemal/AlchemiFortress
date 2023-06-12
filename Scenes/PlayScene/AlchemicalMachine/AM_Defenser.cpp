#include "pch.h"
#include "AM_Defenser.h"

AM_Defenser::AM_Defenser()
{
}

AM_Defenser::~AM_Defenser()
{
}

void AM_Defenser::Initialize()
{
	m_machineID = MACHINE_TYPE::DEFENSER;
	m_objectName = "Defenser";

}

void AM_Defenser::Update()
{
	m_magicCircle.p = m_data.pos;
}

void AM_Defenser::Draw()
{
}

void AM_Defenser::Finalize()
{
}

void AM_Defenser::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{
}

bool AM_Defenser::BulletRequest(std::list<EnemyObject>* enemys)
{
	return false;
}

Bullet AM_Defenser::GetBulletData()
{
	return Bullet(0.f,0.f,0.f,m_data.pos,DirectX::SimpleMath::Vector3());
}
