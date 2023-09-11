#include "Veil.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 無地のテクスチャ
	Texture2D none;
	// タイトルロゴ
	float4 output = none.Sample(samLinear,input.Tex);

	// ルール画像
	float4 output_Rule = tex2.Sample(samLinear, input.Tex);

	// 何処が選択されているのかを知らせる反応用
	float2 textTex = float2(input.Tex.x * 1.5f, input.Tex.y * 1.5f);
	// 中に表示する画像
	float4 output_Text = tex.Sample(samLinear, textTex);

	// 設定した色を受けとる
	output = Colors;

	// ルールフェード
	output.a *= step(output_Rule.x, diffuse.a);
	// 濃い色を薄く剥がす
	output.xyz *= step(output_Rule.x, diffuse.r);
	// 同じ処理を中に描画する文字画像にも行う
	output_Text.a *= step(output_Rule.x, diffuse.r);
	// 幕に文字画像を描画する
	output += output_Text.a;

	return output;
}