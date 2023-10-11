#include "BaseLv.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

float4 main(PS_INPUT input) : SV_TARGET
{
	// 画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	// 板ポリゴンに任意の色を付ける
	float4 outputw = output * colors;

	outputw.rgb = input.color.rgb;

	outputw.rgb = secondColors * lerp(1.0f, 0.0f, step(input.tex.y,1 - input.color.a));

	// ret.a =  lerp(0.0f,ret.a,step(length(intput.Tex),0,5f)))

	return outputw;
}