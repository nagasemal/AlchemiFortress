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
	m_magicCircle.p = m_data.pos;
}

void AM_None::SelectUpdate()
{
}

void AM_None::Draw()
{
}

void AM_None::Finalize()
{
}

void AM_None::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}

void AM_None::LvUp()
{
}
