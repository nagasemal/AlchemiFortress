#include "pch.h"
#include "AM_Upper.h"

AM_Upper::AM_Upper()
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	m_machineID = MACHINE_TYPE::UPEER;
	m_objectName = "Upper";

	m_magicCircle.r = 3.0f;

	ShareData& pSD = ShareData::GetInstance();
	// 警告消し
	pSD.GetDevice();


	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(80.f + ((float)i * 65.f), 560.f), DirectX::SimpleMath::Vector2(64.f, 64.f));
	}

	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);

}

void AM_Upper::Update()
{
	m_magicCircle.p = m_data.pos;

}

void AM_Upper::SelectUpdate()
{
	// 選択状態がノーマルの時ならば、属性選択モード
	if (m_element == NOMAL)
	{
		m_selectBox[0]->HitMouse();
		m_selectBox[1]->HitMouse();
		m_selectBox[2]->HitMouse();
		m_selectBox[3]->HitMouse();

		//　　外部ファイルから読み込めるようにしたい
		//　火属性取得
		if (m_selectBox[0]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0, 0, 1);

			m_element = FLAME;

		}
		//　水属性取得
		if (m_selectBox[1]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 0, 1, 1);

			m_element = AQUA;
		}
		//　風属性取得
		if (m_selectBox[2]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 1, 0, 1);

			m_element = WIND;

		}
		//　土属性取得
		if (m_selectBox[3]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0.5f, 0, 1);

			m_element = EARTH;
		}
	}
}

void AM_Upper::Draw()
{
}

void AM_Upper::Finalize()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	m_selectBox[i]->Finalize();
	//}
}

void AM_Upper::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	if (m_element != NOMAL) return;

	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i]->DrawUI(texture);
	}
}

void AM_Upper::LvUp()
{
}
