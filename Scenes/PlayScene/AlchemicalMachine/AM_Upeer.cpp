#include "pch.h"
#include "AM_Upper.h"

AM_Upper::AM_Upper()
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	m_machineID = MACHINE_TYPE::UPEER;
	m_objectName = "Upper";

	m_magicCircle.r = 2.0f;

}

void AM_Upper::Update()
{
	m_magicCircle.p = m_data.pos;
}

void AM_Upper::SelectUpdate()
{
}

void AM_Upper::Draw()
{
}

void AM_Upper::Finalize()
{
}

void AM_Upper::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{
	for (int i = 0; i < maxNum; i++)
	{
		//	存在チェック
		if (object[i].GetActiv())
		{

		}
	}
}

bool AM_Upper::BulletRequest(std::list<EnemyObject>* enemys)
{
	return false;
}

Bullet AM_Upper::GetBulletData()
{
	return Bullet(0.f, 0.f, 0.f, m_color, m_data.pos, DirectX::SimpleMath::Vector3());
}

void AM_Upper::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}
