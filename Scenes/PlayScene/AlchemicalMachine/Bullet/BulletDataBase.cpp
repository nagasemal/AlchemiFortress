#include "pch.h"
#include "BulletDataBase.h"

#define MaxLv	5

// バレットの元データ配列
BulletStatus dataBase[AlchemicalMachineObject::MACHINE_ELEMENT::SUM]
{
	// 無属性	攻撃△　生存時間〇　スピード〇　発射間隔△　消費魔力◎
	{20.0f,5.0f,3.0f,1.0f,8.0f},

	// 火属性	攻撃〇　生存時間△　スピード〇　発射間隔〇　消費魔力△
	{25.0f,3.5f,3.0f,0.8f,20.0f },

	// 水属性	攻撃△　生存時間◎　スピード△　発射間隔△　消費魔力◎
	{20.0f,8.0f,2.5f,1.0f,8.0f},

	// 風属性	攻撃×　生存時間〇　スピード〇　発射間隔◎　消費魔力〇
	{10.0f,5.0f,6.5f,0.4f,10.0f },

	// 土属性	攻撃◎　生存時間〇　スピード△　発射間隔×　消費魔力〇
	{30.0f,4.0f,2.5f,1.2f,10.0f},
};

BulletStatus GetBaseBulletStatus(AlchemicalMachineObject::MACHINE_ELEMENT element)
{
	// 攻撃力,生存時間,スピード,発射間隔,消費魔力
    return dataBase[element];
}

BulletStatus RecalculationStatus(AlchemicalMachineObject::MACHINE_ELEMENT element, int lv)
{
	//発射間隔以外　元ステータス * lv / MaxLv + 2
	//発射間隔		(1 + 元ステータス) * MaxLv / lv - 1


	BulletStatus status;

	// 攻撃力
	status.damage = (dataBase[element].damage)	* lv / MaxLv;
	// 生存時間
	status.life = (dataBase[element].life)		* lv / MaxLv;
	// スピード
	status.speed = (dataBase[element].speed)	* lv / MaxLv;
	// 消費魔力
	status.lossMp = (dataBase[element].lossMp)  * lv / MaxLv;
	// 発射間隔
	status.span = dataBase[element].span / lv ;

    return status;
}
