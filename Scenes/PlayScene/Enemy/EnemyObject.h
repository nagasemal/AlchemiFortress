//--------------------------------------------------------------------------------------
// File: EnemyObject.h
//
//  エネミーの内部処理　親クラス
//  ステータスを変えることで種類を持たせる
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class EnemyObject : public GameObjct3D
{
public:

	enum EnemyType : int
	{
		NONE = 0,
		

	};

public:
	EnemyObject(EnemyType type, DirectX::SimpleMath::Vector3 startPos,int lv);
	~EnemyObject();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	void Render(GeometricPrimitive* geo);

	// 終了処理
	void Finalize()						override;

	// プレイヤー拠点へと向かう
	bool GotoTarget(DirectX::SimpleMath::Vector3 target);

// アクセサ
public:

	// 体力
	void SetHp(int hp)									{ m_hp = hp; }

	// スピード
	void SetSpeed(float speed)							{ m_speed = speed; }

	// レベル
	const int GetLv()									{ return m_lv; }
	// 体力
	const int GetHp()									{ return m_hp; }
	// パワー	
	const float GetPower()								{ return m_power;}
	// 目的地までの距離
	const DirectX::SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// 加速度
	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele;}

	EnemyObject* GetEnemyObject()						{ return this;}

private:

	float m_power;
	int m_hp;
	int m_lv;
	// 速度
	float m_speed;
	// 加速度
	DirectX::SimpleMath::Vector3 m_accele;
	// 目的地までの距離
	DirectX::SimpleMath::Vector3 m_lengthVec;

};