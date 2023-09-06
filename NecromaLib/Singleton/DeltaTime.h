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

	float GetDeltaTime() { return m_deltaTime * !m_stopTimeFlag; }
	void SetDeltaTime(float time) { m_deltaTime = time; }

	void SetStopFlag(bool flag) { m_stopTimeFlag = flag; }

private:
	DeltaTime();
	static DeltaTime* instance;

	float m_deltaTime;

	bool m_stopTimeFlag;

};