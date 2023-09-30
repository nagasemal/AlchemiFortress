#pragma once
#include "Easing.h"

struct AnimationData
{
	enum Type
	{
		STOP = 0,
		LOOP = 1,
	};

	// ����
	float time;
	// �ŏ�
	float min;
	// �ő�
	float max;
	// ����
	float middle;
	// �l�̓�����
	Type type;
	// ���x�J��Ԃ��Ă��邩
	int iterations;
	// ���ۂ̑J�ڒl
	float anim;

	/// <summary>
	/// �A�j���[�V�����ɓ��������\����
	/// ������������l�����݂���A�j���[�V���������Ȃ�L�p
	/// �����I��min,max�̒l�ŃN�����v���������܂��B
	/// </summary>
	/// <param name="time">�J�n����</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <param name="middle">�����l</param>
	/// <param name="type">�l����ꑱ�����ۂ̓���</param>
	AnimationData(float value = 0.0f, float min = 0.0f, float max = 1.0f, float middle = 0.5f, Type type = Type::STOP)
	{
		this->time = value;
		this->min = min;
		this->max = max;
		this->middle = middle;
		this->type = type;
		anim = 0;

		iterations = 0;
	}

	bool operator==(const AnimationData& value) const
	{
		return this->time == value.time;
	}

	bool operator!=(const AnimationData& value) const
	{
		return this->time != value.time;
	}

	AnimationData operator+(const AnimationData& value) const
	{
		return AnimationData(this->time + value.time,min,max);
	}

	AnimationData operator+(const float value) const
	{
		return AnimationData(this->time + value, min, max);
	}

	AnimationData operator-(const AnimationData& value) const
	{
		return AnimationData(this->time - value.time,min, max);
	}

	AnimationData operator*(const AnimationData& value) const
	{
		return AnimationData(this->time * value.time, min, max);
	}

	AnimationData operator*(const float value) const
	{
		return AnimationData(this->time * value, min, max);
	}

	AnimationData operator/(const AnimationData& value) const
	{
		return AnimationData(this->time / value.time, min, max);
	}

	AnimationData operator/(const float value) const
	{
		return AnimationData(this->time / value, min, max);
	}

	AnimationData& operator=(const AnimationData& value)
	{
		this->time = value.time;

		return *this;
	}


	operator float()
	{
		return time;
	}

	AnimationData& operator+=(const AnimationData& value)
	{

		this->time += value.time;

		if (MaxCheck())
		{
			this->time = max;
			iterations++;

			if (type == LOOP) 	this->time = min;
		}
		return *this;
	}

	AnimationData& operator-=(const AnimationData& value)
	{
		this->time -= value.time;

		if (MinCheck())
		{

			this->time = min;
			iterations++;

			if (type == LOOP) 	this->time = max;
		}

		return *this;
	}
	
	float Clamp()
	{
		return std::min(std::max(time, min), max);
	}

	// time��max�̒l���z����
	bool MaxCheck()
	{
		return time >= max;
	}

	// time��min�̒l���������
	bool MinCheck()
	{
		return time <= min;
	}

	// time��middle���z����
	bool MiddleFlag()
	{
		return time >= middle;
	}

	bool ActiveFlag()
	{
		return !MaxCheck() && !MinCheck();
	}

};