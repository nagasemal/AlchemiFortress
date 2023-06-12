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

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void Initialize();

	void Update(bool hitFiledToMouse,bool hitBaseToMouse,MousePointer* pMP, std::list<EnemyObject> enemys);

	// 弾丸の処理
	void MajicBulletUpdate(int index, std::list<EnemyObject> enemys);

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

	std::list<Bullet>* GetBullet()										{ return m_bullets.get();}

private:

	// 回す
	void MovingMachine(int number);

private:

	std::unique_ptr<MachineSelectManager> m_selectManager;

	//std::unique_ptr<DirectX::Model> m_Model[AlchemicalMachineObject::MACHINE_TYPE::NUM];

	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// アルケミカルマシン確保
	std::unique_ptr<AlchemicalMachineObject> m_AMObject[AM_MAXNUM];

	// アルケミカルマシン(アタックタイプ)から射出される弾のリスト
	std::unique_ptr<std::list<Bullet>>	m_bullets;

	// アルケミカルマシンモデルの受け渡し
	std::unique_ptr<AlchemicalMachineFilter> m_AMFilter;

	bool m_allHitObjectToMouse;

	// 現在選択されているオブジェクトの番号
	int m_selectNumber;

	//===後で消すテスト用変数
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  仮置き弾モデル

};