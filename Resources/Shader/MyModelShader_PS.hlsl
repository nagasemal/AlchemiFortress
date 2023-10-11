#include "MyModelShader.hlsli"

void AddSpecular(inout float4 color, float3 specular)
{
	color.rgb += specular * color.a;
}

float ComputeFogFactor(float4 position)
{
	// ���ɍs���قǍ�������
	return saturate(dot(position, float4(0.0f, -0.05f, 0.0f, 0.0f)));
}

// �t�H�O�̐ݒ�
void ApplyFog(inout float4 color, float fogFactor)
{
	color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}

float4 main(PSInput input) : SV_TARGET0
{

	float texInput = Texture.Sample(Sampler, input.TexCoord);

	float4 color = float4(1.0f,1.0f,1.0f,1.0f) * input.Diffuse;

	float depth = length(LightPosition.xyz - input.Position.xyz) / MaxDepth;

	// �X�y�L�����[��ݒ肷��
	AddSpecular(color, input.Specular);

	// ���ɍs���قǍ�������
	ApplyFog(color, ComputeFogFactor(input.Position));

	// ���Ԍo�߂ŐF��h��
	color.rgb *= PaintColor.rgb * step(texInput, Time.y);

	// ���Ԍo�߂ŏo��������
	color.w *= step(texInput, Time.x);

	// ���Ԍo�߂ŐF��t����
	return float4(color.xyz /**lit*/, color.w);
}