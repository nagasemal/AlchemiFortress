#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#define LVUP_MAGNIFICATION_HP 1.25f
#define STANDARD_HP 30

AM_Attacker::AM_Attacker():
	m_timer(),
	m_targetPos(),
	m_saveBulletStatus(),
	m_bulletStatus()
{

}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	// 警告消し
	pSD.GetDevice();

	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

	m_hp = STANDARD_HP;
	m_maxHp = STANDARD_HP;

	for (int i = 0; i < 4; i++)
	{
		m_selectBox[i] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(80.f + ((float)i * 65.f), 560.f), DirectX::SimpleMath::Vector2(64.f, 64.f));
	}

	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);

	// 1Lv時のバレットのステータスを受け取る
	m_bulletStatus = RecalculationStatus(m_element,1);

	// 保存
	m_saveBulletStatus = m_bulletStatus;
}

void AM_Attacker::Update()
{
	// 強化前に戻す
	m_powerUPFlag = false;
	m_bulletStatus = m_saveBulletStatus;

	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = 5.f + (float)(m_lv / 2.0f);
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

		//　　外部ファイルから読み込めるようにしたい
		//　火属性取得
		if (m_selectBox[0]->ClickMouse())
		{
			// 情報更新
			m_color = DirectX::SimpleMath::Color(0.7, 0, 0, 1);

			m_element = FLAME;

			m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

		}
		//　水属性取得
		if (m_selectBox[1]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 0, 0.7, 1);

			m_element = AQUA;

			m_saveBulletStatus = RecalculationStatus(m_element, m_lv);
		}
		//　風属性取得
		if (m_selectBox[2]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0, 0.7, 0, 1);

			m_element = WIND;

			m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

		}
		//　土属性取得
		if (m_selectBox[3]->ClickMouse())
		{
			m_color = DirectX::SimpleMath::Color(0.7, 0.35f, 0, 1);

			m_element = EARTH;

			m_saveBulletStatus = RecalculationStatus(m_element, m_lv);
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
	//for (int i = 0; i < 4; i++)
	//{
	//	//m_selectBox[i]->Finalize();
	//}
}

void AM_Attacker::AllAlchemicalMachine(AlchemicalMachineObject* object)
{
		if (CircleCollider(object->GetMagicCircle(), GetCircle()))
		{
			m_powerUPFlag = true;

			//強化
			m_bulletStatus.damage	 = m_saveBulletStatus.damage  * 1.5f;
			m_bulletStatus.life		 = m_saveBulletStatus.life	  * 1.5f;
			m_bulletStatus.speed	 = m_saveBulletStatus.speed	  * 1.5f;
			m_bulletStatus.span		 = m_saveBulletStatus.span	  * 0.8f;
			m_bulletStatus.lossMp	 = m_saveBulletStatus.lossMp  * 0.35f;
		}
}

bool AM_Attacker::BulletRequest(std::list<EnemyObject>* enemys)
{
	DataManager* pDataM = DataManager::GetInstance();
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;
	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		// 自機とエネミーの当たり判定
		if (CircleCollider(it->GetCircle(), GetCircle()))
		{
			m_hp -= (int)it->GetPower();
		}

		if (CircleCollider(it->GetCircle(), m_magicCircle))
		{
			// スパン毎に生成
			if (m_timer >= m_bulletStatus.span && pDataM->GetNowMP() > 0)
			{
				m_timer = 0.0f;
				m_targetPos = it->GetPos();

				// 消費MPはLvに依存(高いほど消費大)
				pDataM->SetNowMP(pDataM->GetNowMP() - (int)m_bulletStatus.lossMp);

				return true;
			}
		}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	Bullet::BulletData data = {};
	data.damage	 = m_bulletStatus.damage;
	data.life	 = m_bulletStatus.life;
	data.speed	 = m_bulletStatus.speed;

	return Bullet(data, m_color, m_data.pos,m_targetPos);
}

void AM_Attacker::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	RECT rect_lv = SpriteCutter(64, 64, m_lv,0);
	m_selectLvUpBox->DrawUI(texture,pSL.GetNumberTexture(),rect_lv);

	m_repairBox->DrawUI(texture);

	if (m_element != NOMAL) return;

	for (int i = 0; i < 4; i++)
	{
		RECT rect_element = SpriteCutter(64, 64, 2 + i, 0);

		m_selectBox[i]->DrawUI(texture, pSL.GetElementTexture(), rect_element);
	}
}

void AM_Attacker::LvUp()
{
	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;
	// 現在レベルで再計算
	m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

	// HP強化
	m_maxHp = (int)(STANDARD_HP * LVUP_MAGNIFICATION_HP);
	// HP回復
	m_hp = m_maxHp;

}
