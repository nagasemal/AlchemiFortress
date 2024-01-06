#pragma once
#include "NecromaLib/GameData/Animation.h"

class UserInterface;

class ClearVeil
{
public:
	ClearVeil();
	~ClearVeil();

	void Update(bool startFlag);
	void Render();

	// 次のWaveに移行させる
	bool NextWave();

private:

	// NextWaveの文字描画
	std::unique_ptr<UserInterface> m_nextWave_Top,m_nextWave_Bottom;
	// WAVEクリアした際に流すアニメーション用変数
	AnimationData m_waveAnimation;

};