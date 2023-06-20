#include "pch.h"
#include "AM_Recovery.h"

AM_Recovery::AM_Recovery()
{
}

AM_Recovery::~AM_Recovery()
{
}

void AM_Recovery::Initialize()
{

	m_machineID = MACHINE_TYPE::RECOVERY;
	m_objectName = "Recovery";
}

void AM_Recovery::Update()
{
	m_magicCircle.p = m_data.pos;

}

void AM_Recovery::SelectUpdate()
{
}

void AM_Recovery::Draw()
{
}

void AM_Recovery::Finalize()
{
}

void AM_Recovery::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
}

void AM_Recovery::LvUp()
{

	m_lv++;

}

void AM_Recovery::MPPuls(DataManager* pDM)
{

	pDM->SetNowMP(pDM->GetNowMP() + m_lv);

}
