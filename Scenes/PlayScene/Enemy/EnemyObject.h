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
#include "NecromaLib/GameData/CommonStruct.h"

class EnemyObject : public GameObjct3D
{
public:
	EnemyObject(ENEMY_TYPE type, DirectX::SimpleMath::Vector3 startPos,int lv);
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

	// 進まないようにする処理
	void HitMachine(bool flag);

	// バウンドさせる
	void Bouns();

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
	// EXP
	const int GetEXP()									{ return m_exp; }
	// パワー	
	const float GetPower()								{ return m_power;}
	// 目的地までの距離
	const DirectX::SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// 加速度
	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele;}
	// 自身のエネミータイプ
	const ENEMY_TYPE GetEnemyType()						{ return m_enemyType; }

	EnemyObject* GetEnemyObject()						{ return this;}

private:

	// 攻撃力
	float m_power;
	// HP
	int m_hp;
	// 現在レベル
	int m_lv;
	// 経験値
	int m_exp;
	// 速度
	float m_speed;
	// 移動を止める
	bool m_stopFlag;
	// 自身のタイプ
	ENEMY_TYPE m_enemyType;

	// 加速度
	DirectX::SimpleMath::Vector3 m_accele;
	// 目的地までの距離
	DirectX::SimpleMath::Vector3 m_lengthVec;

};