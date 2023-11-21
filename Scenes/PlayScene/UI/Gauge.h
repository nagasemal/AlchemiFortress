//--------------------------------------------------------------------------------------
// File		: Gauge.h
//
//　現在のリソース量を横棒UIとして表示する
// 
// Use		: PlayScene
// Date		: 2023.6.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/UserInterfase.h"

class SelectionBox;
class Number;

class Gauge
{
private:

public:
	Gauge();
	~Gauge();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	// 複数のUserInterfaseを同じ位置に出現させるための関数
	void Add_MP(SimpleMath::Vector2 position
			,SimpleMath::Vector2 scale
			, UserInterface::ANCHOR anchor);

	void Add_Crystal(SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	void Add_Hp(SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	SimpleMath::Vector2 GetHPGaugePos();
	SimpleMath::Vector2 GetMPGaugePos();
	SimpleMath::Vector2 GetCrystalGaugePos();

private:

	// MP
	// 減増する用
	std::unique_ptr<UserInterface> m_gauge_Mp;
	// 背景用
	std::unique_ptr<UserInterface> m_base_Mp;
	// 減る際の遷移用
	std::unique_ptr<UserInterface> m_back_Mp;

	// 詳細な数を描画
	std::unique_ptr<Number> m_resourceRenderMP;
	//// 前Fの魔力量
	//float m_prevMP;

	// Mpの画像表示
	std::unique_ptr<SelectionBox> m_box_mp;

	// クリスタル
	std::unique_ptr<UserInterface> m_gauge_Crystal;
	std::unique_ptr<UserInterface> m_base_Crystal;
	std::unique_ptr<UserInterface> m_back_Crystal;

	std::unique_ptr<SelectionBox> m_box_Crystal;

	// 詳細な数を描画
	std::unique_ptr<Number> m_resourceRenderCrystal;
	//// 前Fの結晶量
	//float m_prevCrystal;


	// HP
	std::unique_ptr<UserInterface> m_gauge_Hp;
	std::unique_ptr<UserInterface> m_base_Hp;
	std::unique_ptr<UserInterface> m_back_Hp;

	std::unique_ptr<SelectionBox> m_box_Hp;

	// 詳細な数を描画
	std::unique_ptr<Number> m_resourceRenderHP;
	//// 前FのHP量
	//float m_prevHP;

	// 前FのHP割合を保持
	float m_difRedioHp;

	// 前FのMP割合を保持
	float m_difRedioMp;

	// 前Fのクリスタル割合を保持
	float m_difRedioCrystal;

};