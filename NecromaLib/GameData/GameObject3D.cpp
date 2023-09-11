#include "pch.h"
#include "GameObject3D.h"

GameObjct3D::GameObjct3D() :
	m_data()
{
}

GameObjct3D::~GameObjct3D()
{
}

int GameObjct3D::HitObject(ObjectData3D dataA)
{
	return	AABB_3D(-dataA.rage  / 2 + dataA.pos,
					 dataA.rage  / 2 + dataA.pos,
					-m_data.rage / 2 + m_data.pos,
					 m_data.rage / 2 + m_data.pos);
}

SimpleMath::Quaternion GameObjct3D::LookAt(SimpleMath::Vector3 target)
{

    SimpleMath::Vector3 z = (target - m_data.pos);
    z.Normalize();

    SimpleMath::Vector3 x = DirectX::XMVector3Dot(z, SimpleMath::Vector3::Up);
    x.Normalize();

    SimpleMath::Vector3 y = DirectX::XMVector3Dot(x,z);
    y.Normalize();

    SimpleMath::Matrix m = SimpleMath::Matrix::Identity;

    m._11 = x.x; m._12 = y.x; m._13 = z.x;
    m._21 = x.y; m._22 = y.y; m._23 = z.y;
    m._31 = x.z; m._32 = y.z; m._33 = z.z;


    SimpleMath::Quaternion rot = SimpleMath::Quaternion::CreateFromRotationMatrix(m);

    return rot;

}
