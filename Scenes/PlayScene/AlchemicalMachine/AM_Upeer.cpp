#include "pch.h"
#include "AM_Upper.h"

AM_Upper::AM_Upper(MACHINE_ELEMENT element):
	m_element(element)
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	m_machineID = MACHINE_TYPE::UPEER;
	m_objectName = "Upper";

	m_magicCircle.r = 2.0f;

	ShareData& pSD = ShareData::GetInstance();
	// �x������
	pSD.GetDevice();

	m_selectBox[0] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(140, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[1] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(200, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[2] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(260, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[3] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(320, 600), DirectX::SimpleMath::Vector2(64, 64));

	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);

}

void AM_Upper::Update()
{
	m_magicCircle.p = m_data.pos;

	// �I����Ԃ��m�[�}���̎��Ȃ�΁A�����I�����[�h
	if (m_element == NOMAL)
	{
		m_selectBox[0]->HitMouse();
		m_selectBox[1]->HitMouse();
		m_selectBox[2]->HitMouse();
		m_selectBox[3]->HitMouse();

		//�@�@�O���t�@�C������ǂݍ��߂�悤�ɂ�����
		//�@�Α����擾
		if (m_selectBox[0]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0, 0, 1);
			m_element = FLAME;
		}
		//�@�������擾
		if (m_selectBox[1]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 0, 1, 1);
			m_element = AQUA;
		}
		//�@�������擾
		if (m_selectBox[2]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 1, 0, 1);
			m_element = WIND;

		}
		//�@�y�����擾
		if (m_selectBox[3]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(1, 0.5f, 0, 1);
			m_element = EARTH;
		}
	}
}

void AM_Upper::SelectUpdate()
{
}

void AM_Upper::Draw()
{
}

void AM_Upper::Finalize()
{
}

void AM_Upper::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{
	for (int i = 0; i < maxNum; i++)
	{
		//	���݃`�F�b�N
		if (object[i].GetActiv())
		{

		}
	}
}

bool AM_Upper::BulletRequest(std::list<EnemyObject>* enemys)
{

	// �x������
	enemys;

	return false;
}

Bullet AM_Upper::GetBulletData()
{
	return Bullet(0.f, 0.f, 0.f, m_color, m_data.pos, DirectX::SimpleMath::Vector3());
}

void AM_Upper::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i]->DrawUI(texture);
	}
}
