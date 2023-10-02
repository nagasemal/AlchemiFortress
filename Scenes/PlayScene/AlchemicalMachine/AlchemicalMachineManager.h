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
class MoveCamera;

class AlchemicalMachineManager
{
public:
	AlchemicalMachineManager();
	~AlchemicalMachineManager();

	void Initialize();

	void Update(FieldObjectManager* fieldManager,MousePointer* pMP, EnemyManager* enemys ,MoveCamera* moveCamera);

	void Render();

	void DrawUI();

	void Finalize();


public:

//アクセサ
public:

	const bool GetHitMouseFlag(int number)									const { return m_AMObject[number]->GetHitMouse();}

	const std::vector<std::shared_ptr<AlchemicalMachineObject>> GetAlchemicalMachineObject()		const { return m_AMObject; }
	const std::shared_ptr<AlchemicalMachineObject>* GetAlchemicalMachineObject(int index)			const { return &m_AMObject[index];}

	std::list<std::unique_ptr<Bullet>>* GetBullet()									{ return &m_bullets;}

	const std::unique_ptr<MachineSelectManager>* GetMachineSelect()			const { return &m_selectManager; }
	const std::unique_ptr<MachineExplanation>* GetMachineExplanation()		const { return &m_machineExplanation;}

	// マシンが設置されたことを知らせるフラグ
	const MACHINE_TYPE SpawnMachineNotification() const { return m_spawnMachine; }

	// 回転停止のフラグ
	const bool GetRotateStopFlag()	const { return m_rotationStop; }

	// マシンの選択番号を変更する
	void SetSelectMachineNumber(int index) { m_selectNumber = index; }

private:

	// 専用の更新処理を回す
	void Update_None(int baseLv);

	void Update_Attacker(int index, EnemyManager* enemyManager);

	void Update_Defenser(int index, EnemyManager* enemyManager);

	void Update_Mining(int index, FieldObjectManager* fieldManager, EnemyManager* enemyManager);

	void Update_Recovery(EnemyManager* enemyManager);

	void Update_Upper(int index,EnemyManager* enemyManager);

	// 回す
	void MovingMachine(int number);

	// 円形状にアルケミカルマシン(None)を未使用状態で並べる。
	void CreateAMMachine();

	// 並べたアルケミカルマシンをlineNumberに応じて使用可能状態に変更する
	void LvToObjectActives(int lineNumber);

	void SpawnAMMachine(bool leftButtom);

	// 解体時処理(Noneにもどす)
	void Dismantling(int index);

	// Jsonデータから読み取ったリソースを反映させる
	void JsonLoadResources();

private:

	// 円周上に並べる処理
	SimpleMath::Vector3 SetVelocityCircle(int index, int max, float range)
	{

		float radian = XM_2PI / static_cast<float>(max);

		// 一つの角度が分かればもう全部わかる
		float x = range * cosf(radian * index);
		float z = range * sinf(radian * index);

		SimpleMath::Vector3 pos = { x,0,z};

		return pos;
	}

	// アルケミカルマシン選択ボックスの表示
	std::unique_ptr<MachineSelectManager> m_selectManager;

	// アルケミカルマシンの説明ボックスの表示
	std::unique_ptr<MachineExplanation> m_machineExplanation;

	// アルケミカルマシン確保
	std::vector<std::shared_ptr<AlchemicalMachineObject>> m_AMObject;

	// アルケミカルマシン(アタックタイプ)から射出される弾のリスト
	std::list<std::unique_ptr<Bullet>>	m_bullets;

	// アルケミカルマシンモデルの受け渡し
	std::unique_ptr<AlchemicalMachineFilter> m_AMFilter;

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


	bool m_allHitObjectToMouse;

	// 回転を止める
	bool m_rotationStop;

	// 現在選択されているオブジェクトの番号
	int m_selectNumber;
	SimpleMath::Vector3 m_prevSelectMachinePos;

	// Mpが追加されるまでの時間
	float m_mpPulsTimer;

	// 前回のマウスホイールの数値
	int m_saveWheelValue;
	int m_scrollValue;

	// マシンが設置されたことを通知する変数
	MACHINE_TYPE m_spawnMachine;

	// 現在保有しているアルケミカルマシンの個数
	int m_AMnums[MACHINE_TYPE::NUM];

};