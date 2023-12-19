#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "NecromaLib/GameData/SpriteCutter.h"

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
	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

	// Jsonから読み取ったマシンのデータを適応する
	m_maxHp = m_hp = ShareJsonData::GetInstance().GetMachineData(m_machineID).hp;

	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

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

}

void AM_Attacker::SelectUpdate()
{
}

void AM_Attacker::Draw()
{
}

void AM_Attacker::Finalize()
{
}

void AM_Attacker::AllAlchemicalMachine(AlchemicalMachineObject* object)
{
		if (CircleCollider(object->GetMagicCircle(), GetCircle()))
		{
			
			m_powerUPFlag = true;

			//強化
			m_bulletStatus.str		 = m_saveBulletStatus.str	  * 1.15f;
			m_bulletStatus.life		 = m_saveBulletStatus.life	  * 1.15f;
			m_bulletStatus.speed	 = m_saveBulletStatus.speed	  * 1.15f;
			m_bulletStatus.span		 = m_saveBulletStatus.span	  * 0.95f;
			m_bulletStatus.lossMp	 = (int)((float)m_saveBulletStatus.lossMp  / 2.0f);

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

		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		// 自機とエネミーの当たり判定
		if (hitMachine && !m_invincibleFlag)
		{
			// ダメージを受ける
			m_hp -= (int)it->GetPower();

			m_invincibleFlag = true;
		}

		if (CircleCollider(it->GetCircle(), m_magicCircle) && it->GetColliderActive())
		{
			// スパン毎に生成
			if (m_timer >= m_bulletStatus.span && pDataM->GetNowMP() > 0)
			{
				SoundData& pSound = SoundData::GetInstance();

				pSound.PlaySE(ConvertToElement(m_element));

				m_timer = 0.0f;
				m_targetPos = it->GetPos();

				// 消費MPはLvに依存(高いほど消費大)
				pDataM->SetNowMP(pDataM->GetNowMP() - (int)(m_bulletStatus.lossMp * m_lv));

				return true;
			}
		}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	Bullet::BulletData data = {};
	data.damage	 = (float)m_bulletStatus.str;
	data.life	 = (float)m_bulletStatus.life;
	data.speed	 = (float)m_bulletStatus.speed;

	return Bullet(data, m_color, m_data.pos,m_targetPos);
}

void AM_Attacker::RenderUI()
{
}

void AM_Attacker::LvUp()
{
	// クリスタルを減らす
	DataManager& pDM = *DataManager::GetInstance();

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// Lvが上限または変更後のクリスタルが0以下
	if (m_lv >= MAX_LV || pDM.GetNowCrystal() - GetNextLvCrystal() <= 0) return;

	pDM.SetNowCrystal(pDM.GetNowCrystal() - GetNextLvCrystal());

	m_lv++;
	// 現在レベルで再計算
	m_saveBulletStatus = RecalculationStatus(m_element, m_lv);

	// HP強化
	m_maxHp = (int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv));
	// HP回復
	m_hp = m_maxHp;

}
