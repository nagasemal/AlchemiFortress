#include "pch.h"
#include "AM_Recovery.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#define LVUP_MAGNIFICATION_HP 1.25f
#define STANDARD_HP 30

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
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	RECT rect_lv = SpriteCutter(64, 64,m_lv, 0);
	m_selectLvUpBox->DrawUI(texture, pSL.GetNumberTexture(), rect_lv);
}

void AM_Recovery::LvUp()
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

void AM_Recovery::MPPuls(DataManager* pDM)
{

	pDM->SetNowMP(pDM->GetNowMP() + m_lv);

}
