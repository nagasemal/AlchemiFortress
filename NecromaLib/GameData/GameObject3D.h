#pragma once

#include <SimpleMath.h>

#include "StepTimer.h"
#include "NecromaLib/GameData/Collider.h"
#include <DeviceResources.h>
#include <SpriteBatch.h>
#include "NecromaLib/Singleton/ShareData.h"

using namespace DirectX;

class GameObjct3D
{
protected:

	struct ObjectData3D
	{
		SimpleMath::Vector3 pos;
		SimpleMath::Vector3 rage;

		ObjectData3D(DirectX::SimpleMath::Vector3 pos  = DirectX::SimpleMath::Vector3::Zero,
					 DirectX::SimpleMath::Vector3 rage = DirectX::SimpleMath::Vector3::Zero) 
					 { this->pos = pos, this->rage = rage; }
	};

	ObjectData3D m_data;

public:
	GameObjct3D();
	~GameObjct3D();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// ポジションと大きさをもった構造体を渡す
	ObjectData3D GetData()							{ return m_data; }

	SimpleMath::Vector3 GetRage()					{ return m_data.rage; }
	SimpleMath::Vector3 GetPos()					{ return m_data.pos; }

	int  HitObject(ObjectData3D dataA);

	Circle GetCircle()		const					{ return Circle(m_data.pos, m_data.rage.x * 1.5f); }

	// targetの方向へ角度を向けるQuaternionを返します
	DirectX::SimpleMath::Quaternion LookAt(DirectX::SimpleMath::Vector3 target);

};