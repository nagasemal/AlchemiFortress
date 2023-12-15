#include "UI.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

float4 main(PS_INPUT input) : SV_TARGET
{
	//　画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	//　板ポリゴンに任意の色を付ける
	float4 outputw = output * colors;

	outputw.rgb *= input.color.rgb;

	//　if文の描き方と同じ 割合で削る
	outputw.a *= lerp(1.0f, 0.0f, step(input.color.a, input.tex.x));

	return outputw;
}