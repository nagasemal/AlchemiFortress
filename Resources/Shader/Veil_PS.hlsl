#include "Veil.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// ���n�̃e�N�X�`��
	Texture2D none;
	// �^�C�g�����S
	float4 output = none.Sample(samLinear,input.Tex);

	// ���[���摜
	float4 output_Rule = tex2.Sample(samLinear, input.Tex);

	// �������I������Ă���̂���m�点�锽���p
	float2 textTex = float2(input.Tex.x * 1.5f, input.Tex.y * 1.5f);
	// ���ɕ\������摜
	float4 output_Text = tex.Sample(samLinear, textTex);

	// �ݒ肵���F���󂯂Ƃ�
	output = Colors;

	// ���[���t�F�[�h
	output.a *= step(output_Rule.x, diffuse.a);
	// �Z���F�𔖂�������
	output.xyz *= step(output_Rule.x, diffuse.r);
	// ���������𒆂ɕ`�悷�镶���摜�ɂ��s��
	output_Text.a *= step(output_Rule.x, diffuse.r);
	// ���ɕ����摜��`�悷��
	output += output_Text.a;

	return output;
}