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
class SelectionBox;

class AlchemicalMachineObject : public GameObjct3D
{
public:

	// キーを列挙型として持っておけるようにする
	enum MACHINE_TYPE : int
	{
		NONE			 = 0,	// 指定なし
		ATTACKER		 = 1,	// 攻撃型
		UPEER			 = 2,	// 範囲内強化型
		DEFENSER		 = 3,	// 防御型
		MINING			 = 4,	// 採掘型
		RECOVERY		 = 5,	// 魔力回収型

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
	virtual ~AlchemicalMachineObject() {};

	// 選択時の特別なアップデート処理
	virtual void SelectUpdate() = 0;

	// 選択時、マウス選択可能なUIの表示に使用
	virtual void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) = 0;

	// LvUp専用処理
	virtual void LvUp() = 0;

	// 共通してアップデートをかける処理
	void SelectUpdate_Common();

	// 他のオブジェクト同士の当たり判定
	void HitToObject(MousePointer* pMP);

	// 効果範囲内に3Dオブジェクトが入った
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	/// <summary>
	/// モデル描画系
	/// </summary>
	/// <param name="model">　　　 ベースのモデル　</param>
	/// <param name="ring">　　　　追加パーツ</param>
	void ModelRender(DirectX::Model* model,DirectX::Model* ring = nullptr);

	// 召喚
	void SummonAM(DirectX::SimpleMath::Vector3 pos);

// アクセサ
public:

	// 存在しているか否か
	const bool GetActive()									const { return m_active; }

	// マウスが触れているか否か
	const bool GetHitMouse()								const { return m_hitMouseFlag;}

	// オブジェクトの名前
	const std::string GetObjectName()						const { return m_objectName; }

	// オブジェクトのタイプ
	const MACHINE_TYPE GetModelID()							const { return m_machineID;}

	// 現在未使用
	const float GetMachineEffectNum()						const { return m_machineEffectNum; }

	// 効果発動までの時間
	const float GetSpan()									const { return m_span;}
	
	// 現在レベル
	const int	  GetLv()									const { return m_lv; }

	// 現在HP
	const int	  GetHP()									const { return m_hp; }

	// 現在MAXHP
	const int	  GetMAXHP()								const { return m_maxHp; }

	// 効果範囲
	const Circle GetMagicCircle()							const { return m_magicCircle; }

	// 保有している色情報
	// HPが0ならば黒を返します
	const DirectX::SimpleMath::Color GetColor()				const { return m_hp <= 0 ? DirectX::SimpleMath::Color(0, 0, 0, 1): m_color;}

	// 何かの要因で変化を受けている状態
	const bool GetPowerUpFlag()								const { return m_powerUPFlag; }

	// 現在の属性
	const MACHINE_ELEMENT GetElement()						const { return m_element; }

	// 現在どのライン上に存在しているか
	const int GetLine()										const { return m_line; }

	void SetPos(DirectX::SimpleMath::Vector3 pos)		{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)					{ m_magicCircle = circle;}
	void SetActive(bool flag)							{ m_active = flag; }
	void SetLine(int line)								{ m_line = line; }
	void SetPowerUpFlag(bool flag)						{ m_powerUPFlag = flag; }
	void SetSelectModeFlag(bool flag)					{ m_selectModeFlag = flag;}

private:

protected:

	// Lvの最大値
	static const int MAX_LV = 5;

	// マシンID　キーとしてモデルの受け取りを行う
	MACHINE_TYPE m_machineID;

	// マシンの耐久値
	int m_hp;

	// マシンの最大耐久値
	int m_maxHp;

	// マシンのレベル
	int m_lv;

	// マシン特有のデータ(攻撃力や防御性能、採掘速度など)を決定付けるデータ
	float m_machineEffectNum;

	// 次の効果発動までの時間
	float m_span;

	// 存在しているか否か
	bool m_active;

	// 何かしらの影響で強化を受けているか
	bool m_powerUPFlag;

	// 選択モード時のフラグ
	bool m_selectModeFlag;

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

	// オブジェクトのライン位置(どの円形線状にあるか)
	int m_line;

	// LvUp用の選択ボックス
	std::unique_ptr<SelectionBox> m_selectLvUpBox;

	// 修理用の選択ボックス
	std::unique_ptr<SelectionBox> m_repairBox;

};