#include "pch.h"
#include "AM_None.h"

AM_None::AM_None()
{
}

AM_None::~AM_None()
{
}

void AM_None::Initialize()
{
	m_machineID = MACHINE_TYPE::NONE;
	m_objectName = "None";

}

void AM_None::Update()
{
}

void AM_None::Draw()
{
}

void AM_None::Finalize()
{
}
