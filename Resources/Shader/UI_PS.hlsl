#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

////�@�F���
//static const float4 colors[4] =
//{
//	float4(1.0f,  1.0f, 1.0f, 1.0f),	// ��
//	float4(1.0f,  0.0f, 0.0f, 1.0f),	// ��
//	float4(0.0f,  1.0f, 0.0f, 1.0f),	// ��
//	float4(0.0f,  0.0f, 1.0f, 1.0f),	// ��
//};

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
	float4 output = tex.Sample(samLinear, input.tex);

	//�|���S���ɔC�ӂ̐F��t����
	float4 outputw = output * colors;

	outputw.rgb *= input.color.rgb;

	// if���̕`�����Ɠ��� �����ō��
	outputw.a *= lerp(1.0f, 0.0f, step(input.color.a, input.tex.x));

	return outputw;
}