#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"

AM_Attacker::AM_Attacker():
	 m_speed()
{
}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

}

void AM_Attacker::Update()
{

}

void AM_Attacker::Draw()
{

}

void AM_Attacker::Finalize()
{

}
