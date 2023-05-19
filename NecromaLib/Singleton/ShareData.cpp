#include <pch.h>
#include "ShareData.h"

ShareData* ShareData::instance = nullptr;

ShareData::ShareData():
	m_debugFont{nullptr},
	m_commonStates(),
	m_deviceResources(),
	m_spriteBatch(),
	m_camera(),
	m_effectFactory()
{

}

void ShareData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareData;
	}
}

void ShareData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}