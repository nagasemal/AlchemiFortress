//--------------------------------------------------------------------------------------
// File: DeltaTime.h
//
// デルタタイムを得る為のシングルトンクラス
//
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

class DeltaTime
{
public:
	~DeltaTime() = default;

	DeltaTime(const DeltaTime&) = delete;
	DeltaTime& operator=(const DeltaTime&) = delete;

	static void Create();
	static void Destroy();

	static DeltaTime& GetInstance() { return *instance; }

	// ゲーム内デルタタイムを返します
	float GetDeltaTime() { return (m_deltaTime * m_doubleSpeed) * !m_stopTimeFlag; }

	// 加算減算が行われていない素のデルタタイムを返します
	float GetNomalDeltaTime();

	void SetDeltaTime(float time)		{ m_deltaTime = time; }
	void SetDoubleSpeed(float speed)	{ m_doubleSpeed = speed; }
	void SetStopFlag(bool flag)			{ m_stopTimeFlag = flag; }

private:
	DeltaTime();
	static DeltaTime* instance;

	float m_deltaTime;

	float m_doubleSpeed;

	bool m_stopTimeFlag;

};