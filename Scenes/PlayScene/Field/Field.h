#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

class Field : public GameObjct3D
{
public:
	Field();
	~Field();

	// 初期化
	void Initialize()	override;

	// 更新
	void Update()		override;

	// 描画
	void Draw()			override;

	// 終了処理
	void Finalize()		override;

/*===[　アクセサ　]===*/
public:

	bool		GetHitMouse()	const { return m_hitMouseFlag; }

private:

	// 床モデル
	std::unique_ptr<DirectX::Model> m_Model;

	// マウスとの衝突時
	bool		m_hitMouseFlag;

};