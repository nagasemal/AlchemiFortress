#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

#include "Scenes/PlayScene/Mouse/MousePointer.h"

class AlchemicalMachineObject : public GameObjct3D
{
public:

	// キーを列挙型として持っておけるようにする
	enum MACHINE_TYPE : int
	{
		NONE			 = 0,	// 指定なし
		ATTACKER		 = 1,	// 攻撃型
		DEFENSER		 = 2,	// 防御型
		MINING			 = 3,	// 採掘型
		RECOVERY		 = 4,	// 魔力回収型
		UPEER			 = 5,	// 範囲内強化型

		NUM
	};

	// オブジェクトの現在状況の取得
	enum STATE_NAME : int
	{
		BRING = 0,		// 運ばれている
		IDEL  = 1,		// 止まっている
		Play  = 2		// 駆動モード

	};

	AlchemicalMachineObject();
	~AlchemicalMachineObject();

	// 他のオブジェクト同士の当たり判定
	void HitToObject(MousePointer* pMP);
	
	// モデル描画系
	void ModelRender(DirectX::Model* model);

	void SummonAM(DirectX::SimpleMath::Vector3 pos);

// アクセサ
public:

	bool GetActiv()									{ return m_activ; }
	bool GetHitMouse()								{ return m_hitMouseFlag;}
	std::string GetObjectName()						{ return m_objectName; }
	MACHINE_TYPE GetModelID()							{ return m_machineID;}
	float GetMachineEffectNum()						{ return m_machineEffectNum; }
	float GetSpan()									{ return m_span;}
	int	  GetLv()									{ return m_lv; }

	void SetPos(DirectX::SimpleMath::Vector3 pos)	{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)				{ m_magicCircle = circle;}

protected:

	// マシンID　キーとしてモデルの受け取りを行う
	MACHINE_TYPE m_machineID;

	// マシンの耐久値
	int m_hp;

	// マシンのレベル
	int m_lv;

	// マシン特有のデータ(攻撃力や防御性能、採掘速度など)を決定付けるデータ
	float m_machineEffectNum;

	// 次の効果発動までの時間
	float m_span;

	// 存在しているか否か
	bool m_activ;

	// マウスが当たっているか否か
	bool m_hitMouseFlag;

	// 効果範囲
	Circle m_magicCircle;

	std::string m_objectName;

};