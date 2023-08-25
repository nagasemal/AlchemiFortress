#include "pch.h"
#include "AM_Mining.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/DataManager.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

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

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_maxHp = m_hp = (int)pSJD.GetMachineData(m_machineID).hp;
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage;
	m_machineEffectValue = (float)pSJD.GetMachineData(m_machineID).effect_value;

	m_span = 1.0f;
}

void AM_Mining::Update()
{
	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_magicCircle.p = m_data.pos;
	// 効果範囲を決定する
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage + (float)(m_lv / 2.0f);

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
				pDM->SetNowCrystal(pDM->GetNowCrystal() + (int)m_machineEffectValue);
			}
		}
	}
}

void AM_Mining::RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	//SpriteLoder& pSL = SpriteLoder::GetInstance();
	//RECT rect_lv = SpriteCutter(64, 64, m_lv, 0);
	//m_selectLvUpBox->DrawUI(texture, pSL.GetNumberTexture(), rect_lv);
	//m_dismantlingBox->DrawUI(texture);
}

void AM_Mining::LvUp()
{

	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();
	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;

	// HP強化
	m_maxHp = (int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv));
	// HP回復
	m_hp = m_maxHp;

}
