#include "pch.h"
#include "AM_None.h"

#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

AM_None::AM_None():
	m_selectModeTime()
{
}

AM_None::~AM_None()
{

}

void AM_None::Initialize()
{
	m_machineID = MACHINE_TYPE::NONE;
	m_objectName = "None";

	ShareData& pSD = ShareData::GetInstance();
	m_testBox = GeometricPrimitive::CreateGeoSphere(pSD.GetContext());

}

void AM_None::Update()
{
	m_magicCircle.p = m_data.pos;

	m_color = SimpleMath::Color(0.0f,0.0f,0.0f,1.0f);

	// 選択モード時、色を変える
	if (m_selectModeFlag)
	{
		m_selectModeTime += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;

		float transitionVal = sinf(m_selectModeTime);

		m_color = SimpleMath::Color(transitionVal, transitionVal, transitionVal / 1.25f, 1.0f);
	}
	else
	{
		m_selectModeTime = 0;
	}

	// マウスが触れている間、色を変える
	if (m_hitMouseFlag)
	{
		m_color = SimpleMath::Color(1, 1, 1, 1);
	}
}

void AM_None::SelectUpdate()
{
}

void AM_None::Draw()
{
	if (m_selectModeFlag) LightLine();
}

void AM_None::Finalize()
{
}

void AM_None::RenderUI()
{
}

void AM_None::LvUp()
{
}

void AM_None::LightLine()
{

	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Matrix textBox = SimpleMath::Matrix::CreateScale(m_data.rage * 3.0f);
	textBox *= SimpleMath::Matrix::CreateTranslation(m_data.pos);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), m_color, nullptr,false,[&]()
		{
			//ModelShader::GetInstance().ToransparentShader();

			ModelShader::GetInstance().ModelEffectShader(SimpleMath::Color(0.6f,0.8f,0.0f,1.0f),
			SimpleMath::Vector4(0.0f,0.0f,0.0f,m_selectModeTime),
			SpriteLoder::GetInstance().GetAuraBase());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetAuraMask().GetAddressOf());

		});

}
