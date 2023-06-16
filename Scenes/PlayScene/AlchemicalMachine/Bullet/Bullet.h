#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"

class EnemyObject;

class Bullet : public GameObjct3D
{
public:

	struct BulletData
	{
		float speed;
		float damage;
		float life;
	};

public:
	Bullet(float speed, float damage, float life,DirectX::SimpleMath::Color color, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetVector);
	Bullet(BulletData data, DirectX::SimpleMath::Color color, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetVector);
	~Bullet();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void Render(GeometricPrimitive* geo);

	// 親クラスに消す処理を行わせる
	bool deleteRequest();

public:

	void SetLife(float life)	{ m_bulletData.life = life; }

	float GetDamage()			{ return m_bulletData.damage;}
	float GetLife()				{ return m_bulletData.life; }

private:

	BulletData  m_bulletData;

	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Vector3 m_startPos;
	DirectX::SimpleMath::Vector3 m_targetVectol;


};