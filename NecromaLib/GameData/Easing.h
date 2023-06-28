//--------------------------------------------------------------------------------------
// File: Easing.h
//
// 始点a と 終点b を t かけて向かうイージングを実装したクラス + a
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include <SimpleMath.h>

//	動作が現段階で保障されていないもの
//	Expo
//	Elastic
class Easing
{
public:
	static float Leap(float a, float b, float t);

	static int   Leap(int a, int b, float t);
	static DirectX::SimpleMath::Vector3 Leap(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, float time);

	static DirectX::SimpleMath::Vector3 Sleap(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, float time);

	static DirectX::SimpleMath::Matrix CalcLookAt(
		DirectX::SimpleMath::Matrix matrix,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 look,
		DirectX::SimpleMath::Vector3 up);

	// 距離減衰なく動けます。少し重い。
	static DirectX::SimpleMath::Vector3 Moveing(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);

	// 向きを出します
	static float LookAt(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b);


	static float Angle(float a1, float a2, float b1, float b2);

	static float EaseInSine(float a, float b, float t);
	static float EaseOutSine(float a, float b, float t);
	static float EaseInOutSine(float a, float b, float t);

	static float EaseInQuad(float a, float b, float t);
	static float EaseOutQuad(float a, float b, float t);
	static float EaseInOutQuad(float a, float b, float t);

	static float EaseInCubic(float a, float b, float t);
	static float EaseOutCubic(float a, float b, float t);
	static float EaseInOutCubic(float a, float b, float t);

	static float EaseInQuart(float a, float b, float t);
	static float EaseOutQuart(float a, float b, float t);
	static float EaseInOutQuart(float a, float b, float t);

	static float EaseInQuint(float a, float b, float t);
	static float EaseOutQuint(float a, float b, float t);
	static float EaseInOutQuint(float a, float b, float t);

	static float EaseInExpo(float a, float b, float t);
	static float EaseOutExpo(float a, float b, float t);
	static float EaseInOutExpo(float a, float b, float t);

	static float EaseInCirc(float a, float b, float t);
	static float EaseOutCirc(float a, float b, float t);
	static float EaseInOutCirc(float a, float b, float t);

	static float EaseInBack(float a, float b, float t);
	static float EaseOutBack(float a, float b, float t);
	static float EaseInOutBack(float a, float b, float t);

	static float EaseInElastic(float a, float b, float t);
	static float EaseOutElastic(float a, float b, float t);
	static float EaseInOutElastic(float a, float b, float t);

	static float EaseInBounce(float a, float b, float t);
	static float EaseOutBounce(float a, float b, float t);
	static float EaseInOutBounce(float a, float b, float t);

private:
	static float EaseOutBounce(float t);

	static bool StrictEqualityOperator(float a, float b);
};