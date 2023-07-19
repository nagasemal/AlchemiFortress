#include "Shadow.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
	float4 output = tex.Sample(samLinear, input.Tex);

	//�^�����Ȕ|���S��
	float4 outputw = float4(1, 1, 1, 1);

	return output * input.Color;
}