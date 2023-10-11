#include "MyModelShader.hlsli"

void AddSpecular(inout float4 color, float3 specular)
{
	color.rgb += specular * color.a;
}

float ComputeFogFactor(float4 position)
{
	// 下に行くほど黒くする
	return saturate(dot(position, float4(0.0f, -0.05f, 0.0f, 0.0f)));
}

// フォグの設定
void ApplyFog(inout float4 color, float fogFactor)
{
	color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}

float4 main(PSInput input) : SV_TARGET0
{

	float texInput = Texture.Sample(Sampler, input.TexCoord);

	float4 color = float4(1.0f,1.0f,1.0f,1.0f) * input.Diffuse;

	float depth = length(LightPosition.xyz - input.Position.xyz) / MaxDepth;

	// スペキュラーを設定する
	AddSpecular(color, input.Specular);

	// 下に行くほど黒くする
	ApplyFog(color, ComputeFogFactor(input.Position));

	// 時間経過で色を塗る
	color.rgb *= PaintColor.rgb * step(texInput, Time.y);

	// 時間経過で出現させる
	color.w *= step(texInput, Time.x);

	// 時間経過で色を付ける
	return float4(color.xyz /**lit*/, color.w);
}