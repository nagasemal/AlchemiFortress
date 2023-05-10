#include <pch.h>
#include "DeltaTime.h"

DeltaTime* DeltaTime::instance = nullptr;

DeltaTime::DeltaTime():
	m_deltaTime(0)
{

}

void DeltaTime::Create()
{
	if (instance == nullptr)
	{
		instance = new DeltaTime;
	}
}

void DeltaTime::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}