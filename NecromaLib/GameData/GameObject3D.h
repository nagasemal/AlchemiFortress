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
	ObjectData3D GetData() { return m_data; }

	SimpleMath::Vector3 GetRage() { return m_data.rage; }
	SimpleMath::Vector3 GetPos() { return m_data.pos; }

	int  HitObject(ObjectData3D dataA);

};