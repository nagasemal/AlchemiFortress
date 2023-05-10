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

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

	// �|�W�V�����Ƒ傫�����������\���̂�n��
	ObjectData3D GetData() { return m_data; }

	SimpleMath::Vector3 GetRage() { return m_data.rage; }
	SimpleMath::Vector3 GetPos() { return m_data.pos; }

	int  HitObject(ObjectData3D dataA);

};