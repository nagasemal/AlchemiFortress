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
