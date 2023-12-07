#pragma once

class MissionAnimation
{
public:
	MissionAnimation();
	~MissionAnimation();

	void Update();

	void Render();

private:

	// アニメーション表示を行うフラグ
	bool m_animationFlag;

	// 現在のアニメーション枚数
	int m_animPage;

	// 次のアニメーションに遷移するための変数
	float m_animTime;

};