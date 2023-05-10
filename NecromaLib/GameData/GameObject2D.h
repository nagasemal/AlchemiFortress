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

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

	// �|�W�V�����Ƒ傫�����������\���̂�n��
	ObjectData2D GetData() { return m_data; }

	SimpleMath::Vector2 GetRage() { return m_data.rage; }
	SimpleMath::Vector2 GetPos() { return m_data.pos; }

	bool  HitObject(SimpleMath::Vector2 pos);

};