#include "3DUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//　画像表示
    float4 output = tex.Sample(samLinear, input.Tex);

	//　板ポリゴンに任意の色を付ける
    float4 outputw = output * colors;

    outputw.rgb *= input.Color.rgb;

	//　if文の描き方と同じ 割合で削る
    outputw.a *= lerp(1.0f, 0.0f, step(input.Color.a, input.Tex.x));

    return outputw;
}