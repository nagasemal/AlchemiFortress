#pragma once

#include <SimpleMath.h>

#include "StepTimer.h"
#include "NecromaLib/GameData/Collider.h"
#include <DeviceResources.h>
#include <SpriteBatch.h>
#include "NecromaLib/Singleton/ShareData.h"

using namespace DirectX;

class GameObjct2D
{
protected:

	struct ObjectData2D
	{
		SimpleMath::Vector2 pos;
		SimpleMath::Vector2 rage;
	};

	ObjectData2D m_data;

public:
	GameObjct2D();
	~GameObjct2D();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// ポジションと大きさをもった構造体を渡す
	ObjectData2D GetData() { return m_data; }

	SimpleMath::Vector2 GetRage() { return m_data.rage; }
	SimpleMath::Vector2 GetPos() { return m_data.pos; }

	bool  HitObject(SimpleMath::Vector2 pos);

};