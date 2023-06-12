#include "pch.h"
#include "AM_Recovery.h"

AM_Recovery::AM_Recovery()
{
}

AM_Recovery::~AM_Recovery()
{
}

void AM_Recovery::Initialize()
{

	m_machineID = MACHINE_TYPE::RECOVERY;
	m_objectName = "Recovery";
}

void AM_Recovery::Update()
{
	m_magicCircle.p = m_data.pos;
}

void AM_Recovery::SelectUpdate()
{
}

void AM_Recovery::Draw()
{
}

void AM_Recovery::Finalize()
{
}

void AM_Recovery::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{
	// åxçêè¡Çµ
	object;
	maxNum;
}

bool AM_Recovery::BulletRequest(std::list<EnemyObject>* enemys)
{

	// åxçêè¡Çµ
	enemys;
	return false;
}

Bullet AM_Recovery::GetBulletData()
{
	return Bullet(0.f, 0.f, 0.f, m_color, m_data.pos, DirectX::SimpleMath::Vector3());
}

void AM_Recovery::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}
