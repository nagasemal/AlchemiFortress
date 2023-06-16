//--------------------------------------------------------------------------------------
// File: BulletDataBase.h
//
//　バレットのステータスを全て保持している
//  LvUpによる再計算も可　Lv5が最大
// 
// Date: 2023.6.16
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

	// 詳細な設定
	struct BulletStatus
	{
		float damage;
		float life;
		float speed;
		float span;		// 攻撃間隔
		float lossMp;	// 消費魔力
	};

	// 属性に応じた弾の元ステータスを返す
	BulletStatus GetBaseBulletStatus(AlchemicalMachineObject::MACHINE_ELEMENT element);

	// ステータスの再計算
	BulletStatus RecalculationStatus(AlchemicalMachineObject::MACHINE_ELEMENT element,int lv);