#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

////　色情報
//static const float4 colors[4] =
//{
//	float4(1.0f,  1.0f, 1.0f, 1.0f),	// 白
//	float4(1.0f,  0.0f, 0.0f, 1.0f),	// 赤
//	float4(0.0f,  1.0f, 0.0f, 1.0f),	// 緑
//	float4(0.0f,  0.0f, 1.0f, 1.0f),	// 青
//};

float4 main(PS_INPUT input) : SV_TARGET
{
	//トムの画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	//板ポリゴンに任意の色を付ける
	float4 outputw = output * colors;

	outputw.rgb *= input.color.rgb;

	// if文の描き方と同じ 割合で削る
	outputw.a *= lerp(1.0f, 0.0f, step(input.color.a, input.tex.x));

	return outputw;
}