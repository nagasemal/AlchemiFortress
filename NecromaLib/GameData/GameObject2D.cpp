#include "pch.h"
#include "GameObject2D.h"

GameObjct2D::GameObjct2D():
	m_data()
{
}

GameObjct2D::~GameObjct2D()
{
}

bool GameObjct2D::HitObject(SimpleMath::Vector2 pos)
{
	return	PointerToBox(pos,
		-m_data.rage / 2 + m_data.pos,
		m_data.rage / 2 + m_data.pos);
}
