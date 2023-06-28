//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//　エネミーを攻撃するアルケミカルマシン
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/BulletDataBase.h"
#include <list>


class AM_Attacker : public AlchemicalMachineObject
{
public:

	AM_Attacker();
	~AM_Attacker();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 選択時に更新される処理
	void SelectUpdate()					override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	//  ===LvUp効果===
	// 攻撃力　　
    // 弾速　　　
    // 飛距離　　
    // 消費魔力　
	// 発射間隔　
	// 効果範囲　
	void LvUp() override;

	// 全てのアルケミカルマシンの情報を受け取る
	void AllAlchemicalMachine(AlchemicalMachineObject* object);

	// 射出のリクエストを出す
	bool BulletRequest(std::list<EnemyObject>* enemys);

	// 
	Bullet GetBulletData();

//　アクセサ
public:

	BulletStatus GetBulletStatus() { return m_bulletStatus;}

	int GetNextLvCrystal()			{ return (15 * m_lv) * (5 * m_lv); }

private:



private:

	float	m_timer;

	BulletStatus m_bulletStatus;		// バレットデータ
	BulletStatus m_saveBulletStatus;	// 加工元のバレットデータ

	DirectX::SimpleMath::Vector3 m_targetPos;

	//std::list<Bullet> m_bullets;

	// 選択可能オブジェクト
	std::unique_ptr<SelectionBox> m_selectBox[4];
};