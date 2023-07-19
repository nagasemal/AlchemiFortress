//--------------------------------------------------------------------------------------
// File: PlayerBase.h
//
// ゲーム中の拠点(防衛対象)となるオブジェクト
//  
// 
// Date: 2023.5.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Field/FieldObjectType.h"


class PlayerBase : public FieldObject
{

public:
	PlayerBase();
	~PlayerBase();

	// 初期化
	void Initialize()	override;

	// 更新
	void Update()		override;

	// 描画
	void Draw()			override;

	void Render(DirectX::Model* model) override;

	// 終了処理
	void Finalize()		override;

 /*===[　アクセサ　]===*/
public:

	int			GetBaseLv()		const { return m_baseLv;}
	float		GetHP()			const { return m_hp; }
	float		GetExp()		const { return m_exp; }
	bool		GetHitMouse()	const { return m_hitMouseFlag;}
	bool		GetLvUpTiming() const { return m_lvUpTiming; }

	void		PulsExp(float pulsExp)	{ m_exp += pulsExp;}
	void		Damage(float damage);

private:

	int			m_baseLv;				//  拠点Lv
	float		m_hp;					//  拠点Hp
	float		m_exp;					//  現在の経験値量
	bool		m_hitMouseFlag;			//  マウスが当たっているか否か
	bool		m_lvUpTiming;			//  レベルアップのタイミングフラグ
	float		m_invincibleTime;		//	無敵時間

	std::unique_ptr<DirectX::Model> m_baseModel;	//  拠点の3Dモデル

	//===後で消すテスト用変数
	std::unique_ptr<GeometricPrimitive> m_testBox;	//  仮置き四角モデル

};