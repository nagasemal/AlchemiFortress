#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"

class EnemyObject;

class Bullet : public GameObjct3D
{
public:

public:
	Bullet(float speed, float damage, float life,DirectX::SimpleMath::Color color, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 targetVector);
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

	void SetLife(float life)	{ m_life = life; }

	float GetDamage()			{ return m_damage;}
	float GetLife()				{ return m_life; }

private:

	float m_speed;
	float m_damage;
	float m_life;

	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Vector3 m_startPos;
	DirectX::SimpleMath::Vector3 m_targetVectol;


};