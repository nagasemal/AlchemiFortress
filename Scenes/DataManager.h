//--------------------------------------------------------------------------------------
// File: DataManager.h
//
//　他のクラスから共通で使う変数の受け渡しを担当するシングルトンクラス
//  所持魔力　所持結晶　倒した敵の数　設置しているアルケミカルマシンの総数/種類別
// 
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class DataManager
{
public:
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;

	//static void Create();
	//static void Destroy();

	//static DataManager& GetInstance() { return *instance; }

	//	インスタンスの確保
	static DataManager* GetInstance() { return instance; }

	//	インスタンスの生成
	static void Create() { if (instance == nullptr) instance = new DataManager; }

	//	インスタンスの削除
	static void Destroy()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

// ゲッター　セッター
public:

	//魔力総量
	int GetNowMP()			const { return m_nowMP; }
	void SetNowMP(int value)		{ m_nowMP = value; }

	//結晶総量
	int GetNowCrystal()		const { return m_nowCrystal; }
	void SetNowCrystal(int value)		{ m_nowCrystal = value; }

	//敵討伐数
	int GetNowEnemyKill()		const { return m_nowEnemyKill; }
	void SetNowEnemyKill(int value)		{ m_nowEnemyKill = value; }

private:
	DataManager();
	static DataManager* instance;


	// 現在の魔力総量
	int m_nowMP;

	// 現在の結晶総量
	int m_nowCrystal;

	// 現在の敵討伐数
	int m_nowEnemyKill;


};