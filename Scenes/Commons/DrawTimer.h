#pragma once

class DrawTimer
{
private:

	int m_flame, m_seconds;

	SimpleMath::Vector2 m_pos;
	SimpleMath::Vector2 m_rage;

public:
	DrawTimer(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);
	~DrawTimer();

	void Update(float timer);

	void Reset();

	// State End‚ÌŠÔ‚ÅŒÄ‚Ño‚·‚±‚Æ
	void TimerDraw();

	int GetFlame() { return m_flame; }
	int GetSeconds() { return m_seconds; }

};