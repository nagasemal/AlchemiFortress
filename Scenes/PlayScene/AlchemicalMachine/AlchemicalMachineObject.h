#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include <list>
#include <vector>

#include "Scenes/DataManager.h"

class EnemyObject;
class Bullet;

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

public:

	enum MACHINE_ELEMENT : int
	{
		NOMAL	= 0,
		FLAME	= 1,
		AQUA	= 2,
		WIND	= 3,
		EARTH	= 4,
		SUM
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

	//// 全てのマシンの情報をポインタとして受け取る(マシン同士で作用するものがあるため)
	//virtual void AllAlchemicalMachine(AlchemicalMachineObject* object) = 0;
	//// 全てのエネミーの情報を受け取る バレット射出のRequestを送る
	//virtual bool BulletRequest(std::list<EnemyObject>* enemys) = 0;
	//// フィールド上のオブジェクトの情報を受け取る(結晶、敵の巣穴等)
	//virtual void AllFieldObject(FieldObjectManager* fieldManager) = 0;
	//// アルケミカルマシンの弾情報を得る
	//virtual Bullet GetBulletData() = 0;

	// 選択時の特別なアップデート処理
	virtual void SelectUpdate() = 0;

	// 選択時、マウス選択可能なUIの表示に使用
	virtual void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) = 0;

	// LvUp専用処理
	virtual void LvUp() = 0;

	// 他のオブジェクト同士の当たり判定
	void HitToObject(MousePointer* pMP);

	// 効果範囲内に3Dオブジェクトが入った
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	// モデル描画系
	void ModelRender(DirectX::Model* model,DirectX::Model* ring = nullptr);

	void SummonAM(DirectX::SimpleMath::Vector3 pos);


// アクセサ
public:

	bool GetActiv()									{ return m_activ; }
	bool GetHitMouse()								{ return m_hitMouseFlag;}
	std::string GetObjectName()						{ return m_objectName; }
	MACHINE_TYPE GetModelID()						{ return m_machineID;}
	float GetMachineEffectNum()						{ return m_machineEffectNum; }
	float GetSpan()									{ return m_span;}
	int	  GetLv()									{ return m_lv; }
	Circle GetMagicCircle()							{ return m_magicCircle; }
	DirectX::SimpleMath::Color GetColor()			{ return m_color; }
	bool GetPowerUpFlag()							{ return m_powerUPFlag; }
	MACHINE_ELEMENT GetElement()					{ return m_element; }

	void SetPos(DirectX::SimpleMath::Vector3 pos)	{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)				{ m_magicCircle = circle;}

private:

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

	// 何かしらの影響で強化を受けているか
	bool m_powerUPFlag;

	// マウスが当たっているか否か
	bool m_hitMouseFlag;

	// Y軸回転させるための変数
	float m_rotateAnimation;

	// 効果範囲
	Circle m_magicCircle;

	std::string m_objectName;

	// オブジェクトの色
	DirectX::SimpleMath::Color m_color;

	// オブジェクトの属性
	MACHINE_ELEMENT m_element;

};