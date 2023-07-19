#pragma once

class Animation
{
public:

	// 始点と終点の構造体
	struct AnimData
	{
		float first;
		float last;
		float speed;
		float flyingTime;

		AnimData(float firstParam, float lastParam,float speed,float flyingTime)
		{
			first				= firstParam;
			last				= lastParam;
			this->speed			= speed;
			this->flyingTime	= flyingTime;
		}
	};

	Animation();
	~Animation();

	void AddAnim(std::vector<AnimData> animData);
	void AddAnim(float animFirst, float animLast,float speed,float flyingTime = 0.0f);

	void ClearAnim();

	float Animatior();

private:

	int m_nextIndex;
	std::vector<AnimData> m_animData;
	std::vector<float> m_animTimer;

};