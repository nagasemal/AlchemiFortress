//--------------------------------------------------------------------------------------
// File: ShareJsonData.h
//
// jsonで読み込んだ数値を渡す
//
// 内容物
// ・バレットのパラメータ
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

#include "NecromaLib/GameData/JsonLoder.h"

class ShareJsonData
{
public:
	~ShareJsonData() = default;

	ShareJsonData(const ShareJsonData&) = delete;
	ShareJsonData& operator=(const ShareJsonData&) = delete;

	static void Create();
	static void Destroy();

	static ShareJsonData& GetInstance() { return *instance; }

	// バレットのjsonファイルを読み込む
	void LoadingJsonFile_Bullet();

	// マシンのjsonファイルを読み込む
	void LoadingJsonFile_Machine();

	// エネミーのjsonファイルを読み込む
	void LoadingJsonFile_Enemy();

	// ステージのjsonファイルを読み込む
	void LoadingJsonFile_Stage(int number);

	// クリアデータのjsonファイルを読み込む
	void LoadingJsonFile_ClearData(int number);

public:

	// 属性に対応したバレットのパラメータを渡す
	const Bullet_Data GetBulletData(ELEMENT element);

	// マシンタイプに対応したマシンのパラメータを渡す
	const Machine_Data GetMachineData(MACHINE_TYPE type);

	// エネミータイプに対応したエネミーのパラメータを渡す
	const Enemy_Data GetEnemyData(ENEMY_TYPE type);

	// ステージのデータを渡す
	const Stage_Data GetStageData();

	// ステージのクリアデータを渡す
	const Stage_ClearData GetClearData();

	// ステージデータの中身を全て消す(メモリ領域の圧迫を防ぐため)
	void StageDataCleanUP();

	// エレメントに紐づく色情報を取得
	DirectX::SimpleMath::Color GetElementColor(ELEMENT element);


private:
	ShareJsonData();
	static ShareJsonData* instance;

	// Attackerが繰り出す弾のパラメータ
	Bullet_Data m_bulletData[ELEMENT::Num];

	// 各マシンのデータ
	Machine_Data m_machineData[MACHINE_TYPE::NUM];

	// 各エネミーのデータ
	Enemy_Data m_enemyData[ENEMY_TYPE::ENEMY_NUM];

	// 読み込んだステージのデータ(全てを読み込むとメモリ効率が悪い為、随時読み込む方式を採用)
	Stage_Data m_stageData;

	// 読み込んだステージのクリア時データ
	Stage_ClearData m_clearData;

};