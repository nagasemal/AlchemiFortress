#include "CutTexture.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// タイトルロゴ
	float4 output = tex.Sample(samLinear, input.Tex);

	// タイトルロゴの柄
	float4 output_2 = tex2.Sample(samLinear, input.Tex);

	output.rgb = Colors.rgb;

	output.r *= output_2.a;
	output.g *= output_2.a;
	output.b *= output_2.a;

	return output;
}