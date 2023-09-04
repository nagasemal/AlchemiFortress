#include "MagicCircle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 魔法陣画像
	float4 output = tex.Sample(samLinear, input.Tex);

	// 魔法陣の出現をさせるルール画像
	float4 output_2 = tex2.Sample(samLinear, input.Tex);

	output.a *= step(output_2.x, diffuse.a);

	output.x *= step(output_2.x, diffuse.a);
	output.y *= step(output_2.y, diffuse.a);
	output.z *= step(output_2.z, diffuse.a);

	return output * input.Color;
}