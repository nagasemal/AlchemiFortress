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

bool GameObjct2D::HitObject_RageSet(SimpleMath::Vector2 pos, float width, float height, float percent)
{
	SimpleMath::Vector2 leftUpPos		= { m_data.pos.x - (width * percent) / 2,m_data.pos.y - (height * percent) / 2};
	SimpleMath::Vector2 rightBottomPos = { m_data.pos.x + (width * percent) / 2,m_data.pos.y + (height * percent) / 2};

	return	PointerToBox(pos,
		leftUpPos,rightBottomPos);
}

bool GameObjct2D::HitObject_RageSet(SimpleMath::Vector2 pos, float width, float height, SimpleMath::Vector2 percent)
{
	SimpleMath::Vector2 leftUpPos =	  { m_data.pos.x - (width * percent.x) / 2 ,m_data.pos.y - (height * percent.y) / 2 };
	SimpleMath::Vector2 rightBottomPos = { m_data.pos.x + (width * percent.x) / 2,m_data.pos.y +  (height * percent.y) / 2 };

	return	PointerToBox(pos,
		leftUpPos, rightBottomPos);
}
