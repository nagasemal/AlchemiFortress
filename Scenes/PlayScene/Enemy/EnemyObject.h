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

class EnemyCommander;
class Enemy_StanderMove;
class Enemy_MeanderingMove;
class ICommand_Enemy;
struct Enemy_Data;

class EnemyObject : public GameObjct3D
{
public:
	EnemyObject(ENEMY_TYPE type, SimpleMath::Vector3 startPos,int lv);
	~EnemyObject();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	void Render(Model* model);

	// 終了処理
	void Finalize()						override;

	// プレイヤー拠点へと向かう
	bool GotoTarget(SimpleMath::Vector3 target);

	// 進まないようにする処理
	void HitMachine(bool flag);

	// バウンドさせる
	void Bouns();

// アクセサ
public:

	// 体力
	void SetHp(int hp)									{ m_hp = hp; }

	// ターゲット
	void SetTargetPos(SimpleMath::Vector3 targetPos) { m_targetPos = targetPos; }

	// 目的地までの距離
	void SetLengthVec(SimpleMath::Vector3 lengthVec) { m_lengthVec = lengthVec; }

	// 止まる処理
	void SetStopFlag(bool flag)							{ m_stopFlag = flag;}

	// ステータスを設定する
	void SetEnemyData(Enemy_Data data);

	// レベル
	const int GetLv()									{ return m_lv; }
	// 体力
	const int GetHp()									{ return m_hp; }
	// EXP
	const int GetEXP()									{ return m_exp; }
	// パワー	
	const float GetPower()								{ return m_power;}

	// とまっているフラグ
	const bool GetStopFlag()							{ return m_stopFlag; }

	const SimpleMath::Vector3 GetTargetPos()	{ return m_targetPos;}
	// 目的地までの距離
	const SimpleMath::Vector3 GetLengthVec()	{ return m_lengthVec;}
	// 加速度
	const SimpleMath::Vector3 GetAccele()		{ return m_accele;}
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
	// 生存している時間
	float m_aliveTimer;
	// 移動を止める
	bool m_stopFlag;
	// 動きの種類　ALL 記述されている動きを全て同時に動かす　ONE 一つずつ動きを動かす
	std::string m_moveType;
	// 自身のタイプ
	ENEMY_TYPE m_enemyType;

	// 加速度
	SimpleMath::Vector3 m_accele;

	SimpleMath::Vector3 m_moveVec;

	SimpleMath::Quaternion m_rotation;

	// 目的地までの距離
	SimpleMath::Vector3 m_lengthVec;

	// 目標の位置
	SimpleMath::Vector3 m_targetPos;

	std::vector<ICommand_Enemy*> m_moveCommands;
	// 動きのデータが入るコマンド
	std::shared_ptr<EnemyCommander> m_commander;



};