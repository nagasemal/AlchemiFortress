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
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "AlchemicalMachineFilter.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void Initialize();

	void Update(FieldObjectManager* fieldManager,bool hitBaseToMouse,MousePointer* pMP, std::list<EnemyObject> enemys);

	void Render();

	void DrawUI();

	void Finalize();


public:

	static const int AM_MAXNUM = 256;

//アクセサ
public:

	bool GetHitMouseFlag(int number)									{ return m_AMObject[number]->GetHitMouse();}

	AlchemicalMachineObject* GetAlchemicalMachineObject()				{ return m_AMObject->get(); }
	AlchemicalMachineObject* GetAlchemicalMachineObject(int index)		{ return m_AMObject[index].get();}

	std::list<std::unique_ptr<Bullet>>* GetBullet()						{ return &m_bullets;}

private:

	// 専用の更新処理を回す
	void Update_Attacker(int index, std::list<EnemyObject> enemys);

	void Update_Mining(int index, FieldObjectManager* fieldManager);

	void Update_Recovery();

	// 回す
	void MovingMachine(int number);

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
	std::unique_ptr<AlchemicalMachineObject> m_AMObject[AM_MAXNUM];


	// アルケミカルマシン(アタックタイプ)から射出される弾のリスト
	std::list<std::unique_ptr<Bullet>>	m_bullets;

	// アルケミカルマシンモデルの受け渡し
	std::unique_ptr<AlchemicalMachineFilter> m_AMFilter;

	bool m_allHitObjectToMouse;

	// 現在選択されているオブジェクトの番号
	int m_selectNumber;

	// Mpが追加されるまでの時間
	float m_mpPulsTimer;

	// 現在保有しているアルケミカルマシンの個数
	int m_AMnums[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	//===後で消すテスト用変数
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  仮置き弾モデル

};