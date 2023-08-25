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

	// ステージのjsonファイルを読み込む
	void LoadingJsonFile_Stage(int number);

	// クリアデータのjsonファイルを読み込む
	void LoadingJsonFile_ClearData(int number);

public:

	// 属性に対応したバレットのパラメータを渡す
	const Bullet_Data GetBulletData(MACHINE_ELEMENT element);

	// マシンタイプに対応したマシンのパラメータを渡す
	const Machine_Data GetMachineData(MACHINE_TYPE type);

	// ステージのデータを渡す
	const Stage_Data GetStageData();

	// ステージのクリアデータを渡す
	const Stage_ClearData GetClearData();

	// ステージデータの中身を全て消す(メモリ領域の圧迫を防ぐため)
	void StageDataCleanUP();

	DirectX::SimpleMath::Color GetElementColor(MACHINE_ELEMENT element);


private:
	ShareJsonData();
	static ShareJsonData* instance;

	Bullet_Data m_bulletData[MACHINE_ELEMENT::Num];

	Machine_Data m_machineData[MACHINE_TYPE::NUM];

	Stage_Data m_stageData;

	Stage_ClearData m_clearData;

};