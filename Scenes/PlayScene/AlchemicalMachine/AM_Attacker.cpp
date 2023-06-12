#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

AM_Attacker::AM_Attacker(MACHINE_ELEMENT element):
	m_speed(1.0f),
	m_bulletlife(2.0f),
	m_power(5.0f),
	m_timer(),
	m_targetPos(),
	m_element(element)
{
}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

	m_selectBox[0] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(140, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[1] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(200, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[2] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(260, 600), DirectX::SimpleMath::Vector2(64, 64));
	m_selectBox[3] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(320, 600), DirectX::SimpleMath::Vector2(64, 64));

	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);

}

void AM_Attacker::Update()
{
	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = 5.f;
}

void AM_Attacker::SelectUpdate()
{
	// 選択状態がノーマルの時ならば、属性選択モード
	if (m_element == NOMAL)
	{
		m_selectBox[0]->HitMouse();
		m_selectBox[1]->HitMouse();
		m_selectBox[2]->HitMouse();
		m_selectBox[3]->HitMouse();

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

void AM_Attacker::Draw()
{

	ShareData& pSD = ShareData::GetInstance();

	/*===[ データの表示 ]===*/
	std::wostringstream oss;
	oss << "Element - " << m_element;
	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(120.f, 360.f));
}

void AM_Attacker::Finalize()
{
	m_selectBox->reset();
}

void AM_Attacker::AllAlchemicalMachine(AlchemicalMachineObject* object, int maxNum)
{

	for (int i = 0; i < maxNum; i++)
	{
		//	存在チェック
		if(object[i].GetActiv())
		{

		}
	}

}

bool AM_Attacker::BulletRequest(std::list<EnemyObject>* enemys)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		if (CircleCollider(it->GetCircle(), m_magicCircle))
		{
			// 5秒毎に生成
			if (m_timer >= 0.5f)
			{

				m_timer = 0.0f;
				m_targetPos = it->GetPos();
				return true;
			}
		}
		//// balletToEnemy
		////　ToDO::balletクラス内で何ともならなかった為。
		//for (std::list<Bullet>::iterator bulletIt = m_bullets.begin(); bulletIt != m_bullets.end(); bulletIt++)
		//{
		//	if (CircleCollider(it->GetCircle(), bulletIt->GetCircle()))
		//	{
		//		bulletIt->SetLife(0);
		//		it->SetHp(it->GetHp() - (int)bulletIt->GetDamage());
		//	}
		//}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	return Bullet(m_speed,m_power,m_bulletlife, m_color, m_data.pos,m_targetPos);
}

void AM_Attacker::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i]->DrawUI(texture);
	}
}
