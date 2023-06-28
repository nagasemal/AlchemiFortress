#include "pch.h"
#include "AM_Defenser.h"

AM_Defenser::AM_Defenser():
	m_isBreak()
{
}

AM_Defenser::~AM_Defenser()
{
}

void AM_Defenser::Initialize()
{
	m_machineID = MACHINE_TYPE::DEFENSER;
	m_objectName = "Defenser";

}

void AM_Defenser::Update()
{
	m_magicCircle.p = m_data.pos;
}

void AM_Defenser::SelectUpdate()
{
}

void AM_Defenser::Draw()
{
}

void AM_Defenser::Finalize()
{
}

void AM_Defenser::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}

void AM_Defenser::LvUp()
{

	m_lv++;

}
