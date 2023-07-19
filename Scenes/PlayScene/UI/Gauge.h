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
	void Add_MP(DirectX::SimpleMath::Vector2 position
			,DirectX::SimpleMath::Vector2 scale
			, UserInterface::ANCHOR anchor);

	void Add_Crystal(DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	void Add_Hp(DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

private:

	// MP
	// 減増する用
	std::unique_ptr<UserInterface> m_gauge_Mp;
	// 背景用
	std::unique_ptr<UserInterface> m_base_Mp;
	// フレーム
	std::unique_ptr<UserInterface> m_falme_Mp;

	// クリスタル
	std::unique_ptr<UserInterface> m_gauge_Crystal;
	std::unique_ptr<UserInterface> m_base_Crystal;
	std::unique_ptr<UserInterface> m_falme_Crystal;

	// HP
	std::unique_ptr<UserInterface> m_gauge_Hp;
	std::unique_ptr<UserInterface> m_base_Hp;
	std::unique_ptr<UserInterface> m_falme_Hp;

};