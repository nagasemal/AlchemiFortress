#include "BaseLv.hlsli"

Texture2D tex : register(t0);
Texture2D effectTex : register(t1);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

float4 main(PS_INPUT input) : SV_TARGET
{
	// 画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	// 板ポリゴンに任意の色を付ける
    float4 outputw = output;

    //outputw.rgb *= colors * lerp(0.0f, 1.0f, step(input.tex.y, input.color.a));

	// EXP表示の色を塗る
    outputw.rgb = secondColors * lerp(1.0f, 0.0f, step(input.tex.y, 1 - input.color.a));
	
	// ベースの色を塗る
    outputw.rgb += colors * lerp(0.0f, 1.0f, step(input.tex.y, 1 - input.color.a));
	
	return outputw;
}