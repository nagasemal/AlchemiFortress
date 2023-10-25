#include "MyModelShader.hlsli"

Texture2D AuraTexture: register(t0);
Texture2D MaskTexture: register(t1);

float4 main(PSInput input) : SV_TARGET0
{


	float2 texCoord = float2(input.TexCoord.x,input.TexCoord.y - (Time.w * 0.2f));

	float4 auraTex = AuraTexture.Sample(Sampler, input.TexCoord);

	float4 output = float4(0.3f,0.7f,1.0f,1.0f);

	//auraTex.rgb *= auraTex.rgb;


	//float4 maskTex = MaskTexture.Sample(Sampler, texCoord);

	//float color = smoothstep(maskTex,auraTex,0.0);

	////color.w *= 1.0f;

	//float4 col = color * PaintColor;
	//col.w *= 0.9f;

	//col.w *= lerp(0.0f, 1.0f, step(0.5f, input.TexCoord.y));
	return auraTex;
}