//--------------------------------------------------------------------------------------
// File: AlchemicalMachineManager.h
//
//　プレイヤーがフィールド内に設置するオブジェクト群のマネージャー
// 
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <map>
#include <string>
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/UI/MachineExplanation.h"
#include "Scenes/PlayScene/UI/MachineSelectManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "AlchemicalMachineFilter.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include "Scenes/PlayScene/Shadow/DorpShadow.h"
#include "Scenes/PlayScene/Shadow/Particle.h"
#include "Scenes/PlayScene/Shadow/MagicCircle.h"


class AlchemicalMachineObject;
class Number;

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void Initialize();

	void Update(FieldObjectManager* fieldManager,MousePointer* pMP, EnemyManager* enemys);

	void Render();

	void DrawUI();

	void Finalize();


public:

//アクセサ
public:

	bool GetHitMouseFlag(int number)									{ return m_AMObject[number]->GetHitMouse();}

	std::vector<std::shared_ptr<AlchemicalMachineObject>> GetAlchemicalMachineObject()		{ return m_AMObject; }
	std::shared_ptr<AlchemicalMachineObject>* GetAlchemicalMachineObject(int index)			{ return &m_AMObject[index];}

	std::list<std::unique_ptr<Bullet>>* GetBullet()						{ return &m_bullets;}

	std::unique_ptr<MachineSelectManager>* GetMachineSelect()			{ return &m_selectManager; }

private:

	// 専用の更新処理を回す
	void Update_None(int baseLv);

	void Update_Attacker(int index, EnemyManager* enemyManager);

	void Update_Defenser(int index, EnemyManager* enemyManager);

	void Update_Mining(int index, FieldObjectManager* fieldManager);

	void Update_Recovery();

	// 回す
	void MovingMachine(int number);

	// 円形状にアルケミカルマシン(None)を未使用状態で並べる。
	void CreateAMMachine();

	// 並べたアルケミカルマシンをlineNumberに応じて使用可能状態に変更する
	void LvToObjectActives(int lineNumber);

private:

	// 円周上に並べる処理
	DirectX::SimpleMath::Vector3 SetVelocityCircle(int index, int max, float range)
	{

		float radian = XM_2PI / static_cast<float>(max);

		// 一つの角度が分かればもう全部わかる
		float x = range * cosf(radian * index);
		float z = range * sinf(radian * index);

		DirectX::SimpleMath::Vector3 pos = { x,0,z};

		return pos;
	}

	std::unique_ptr<MachineSelectManager> m_selectManager;

	//std::unique_ptr<DirectX::Model> m_Model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// アルケミカルマシン確保
	std::vector<std::shared_ptr<AlchemicalMachineObject>> m_AMObject;

	// アルケミカルマシン(アタックタイプ)から射出される弾のリスト
	std::list<std::unique_ptr<Bullet>>	m_bullets;

	// アルケミカルマシンモデルの受け渡し
	std::unique_ptr<AlchemicalMachineFilter> m_AMFilter;

	bool m_allHitObjectToMouse;

	// 現在選択されているオブジェクトの番号
	int m_selectNumber;

	// Mpが追加されるまでの時間
	float m_mpPulsTimer;

	// 前回のマウスホイールの数値
	int m_saveWheelValue;
	int m_scrollValue;

	// 現在保有しているアルケミカルマシンの個数
	int m_AMnums[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	// 所持数を示す
	std::unique_ptr<Number> m_machineNumRender;

	//===後で消すテスト用変数
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  仮置き弾モデル

	// 丸影
	std::unique_ptr<DorpShadow> m_dorpShadow;

	// パーティクルクラス
	std::unique_ptr<Particle> m_particle_hit;

	// 魔法陣クラス(マシン)
	std::unique_ptr<MagicCircle> m_magicCircle;

	// 魔法陣クラス(フィールド)
	std::unique_ptr<MagicCircle> m_magicCircle_Field;

};