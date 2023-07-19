#include "pch.h"
#include "AM_Mining.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/DataManager.h"

#define LVUP_MAGNIFICATION_HP 1.25f
#define STANDARD_HP 100

AM_Mining::AM_Mining():
	m_timer()
{
}

AM_Mining::~AM_Mining()
{
}

void AM_Mining::Initialize()
{
	m_machineID = MACHINE_TYPE::MINING;
	m_objectName = "Mining";

	m_magicCircle.r = 3.5f;

	m_machineEffectNum = 20.0f;
	m_span = 1.0f;

	m_hp = STANDARD_HP;
	m_maxHp = STANDARD_HP;
}

void AM_Mining::Update()
{
	m_magicCircle.p = m_data.pos;
	m_timer += DeltaTime::GetInstance().GetDeltaTime();
	m_color = DirectX::SimpleMath::Color(1, 1, 1, 1);
}

void AM_Mining::SelectUpdate()
{

}

void AM_Mining::Draw()
{
}

void AM_Mining::Finalize()
{
}

void AM_Mining::AllFieldObject(FieldObjectManager* fieldManager)
{
	DataManager* pDM = DataManager::GetInstance();

	for (std::list<Crystal>::iterator it = fieldManager->GetCrystalObject()->begin(); it != fieldManager->GetCrystalObject()->end(); it++)
	{
		if (CircleCollider(it->GetCircle(), m_magicCircle))
		{
			m_color = DirectX::SimpleMath::Color(1, 0, 1, 1);

			// 回収開始
			if (m_timer >= m_span)
			{
				m_timer = 0.0f;
				pDM->SetNowCrystal(pDM->GetNowCrystal() + (int)m_machineEffectNum);
			}
		}
	}
}

void AM_Mining::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	RECT rect_lv = SpriteCutter(64, 64, m_lv, 0);
	m_selectLvUpBox->DrawUI(texture, pSL.GetNumberTexture(), rect_lv);
}

void AM_Mining::LvUp()
{

	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;

	// HP強化
	m_maxHp = (int)(STANDARD_HP * LVUP_MAGNIFICATION_HP);
	// HP回復
	m_hp = m_maxHp;

}
