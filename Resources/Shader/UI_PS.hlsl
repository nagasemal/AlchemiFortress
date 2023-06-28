#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
	float4 output = tex.Sample(samLinear, input.tex);

	//�^�����Ȕ|���S��
	float4 outputw = output * float4(1, 1, 1, 1);
	
	// if���̕`�����Ɠ���
	outputw.a *= lerp(1.0f, 0.0f, step(input.color.x, input.tex.x));

	return outputw;
}