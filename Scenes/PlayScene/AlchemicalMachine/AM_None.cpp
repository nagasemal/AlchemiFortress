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

	m_color = DirectX::SimpleMath::Color(0,0,0, 1);

	// 選択モード時、色を変える
	if (m_selectModeFlag)
	{
		m_selectModeTime += 0.05f;

		float transitionVal = sinf(m_selectModeTime);

		m_color = DirectX::SimpleMath::Color(transitionVal, transitionVal, transitionVal / 1.25f, 1.0f);
	}
	else
	{
		m_selectModeTime = 0;
	}

	// マウスが触れている間、色を変える
	if (m_hitMouseFlag)
	{
		m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);
	}
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

void AM_None::LightLine()
{
}
