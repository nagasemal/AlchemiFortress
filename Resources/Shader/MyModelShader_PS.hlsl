#include "MyModelShader.hlsli"

Texture2D MachineTexture: register(t1);
Texture2D NomalTexture: register(t2);

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

// リムライトの設定
// 視線ベクトルと法線ベクトルが直角に近いほどライトの値を大きくする
float4 ApplyLimLight(float3 normal)
{
	//　リムライト
	float3 invEye = normalize(mul(WorldInverseTranspose, EyePosition));
	float rim = pow(1.0 - clamp(dot(invEye,normal), 0.0, 1.0), 5.0);


	float3 eyedif = normalize(Eyes - EyePosition);


	// 視線ベクトルとライトベクトルも考慮に入れる
	// pow = 強度増
	// max = 1.0 以上に行かないようにする
	float dotLE_1 = pow(max(dot(eyedif, normalize(LightDirection[0])), 1.0), 30.0);
	float dotLE_2 = pow(max(dot(eyedif, normalize(LightDirection[1])), 1.0), 30.0);
	float dotLE_3 = pow(max(dot(eyedif, normalize(LightDirection[2])), 1.0), 30.0);

	return lerp(0, LimLightColor, rim  * dotLE_1 * dotLE_2 * dotLE_3);

}

void Asobi(inout float4 color,float2 uv)
{
	float4 col = PaintColor;

	float power = 0.4f;
	float nanikore = 0.1f;

	//float alphalx = col.a * lerp(1, power, (0 - uv.x));
	//col.a = saturate(lerp(alphalx, col.a, step(0, uv.x)));

	//float alpharx = col.a * lerp(1, power, (uv.x - 1));
	//col.a = saturate(lerp(col.a, alpharx, step(1, uv.x)));

	float alphaty = col.a * lerp(1, power, (1 - uv.y));
	col.a = saturate(lerp(alphaty, col.a, step(1, uv.y)));

	//col.a = saturate(lerp(1.0f, 0.0f, step(1.0f, uv.y)));

	//float alphaby = col.a * lerp(1, power, (uv.y - 0));
	//col.a = saturate(lerp(col.a, alphaby, step(0, uv.y)));

	color.a = col.a;

}

// ノーマルマップの設定
void ApplyNomalMap()
{
	//float3 normalMap = (texture2D(texture, vTextureCoord) * 2.0 - 1.0).rgb;
}

// グレースケール化
float ChangeGray(float4 col)
{
	return col.r * 0.3f + col.g * 0.6f + col.b * 0.1f;
}

// セピア化
float4 ChangeSepia(float4 col)
{
	float g = ChangeGray(col);
	return float4(g * 1.44f, g * 0.99f, g * 0.57f, col.a);
}

float4 main(PSInput input) : SV_TARGET0
{

	float texInput = Texture.Sample(Sampler, input.TexCoord);

	// テクスチャ取得
	float3 modelTexture = MachineTexture.Sample(Sampler, input.TexCoord);

	// ノーマルマップ取得
	float3 nomalTex = NomalTexture.Sample(Sampler, input.TexCoord);

	float4 diff = pow(dot(nomalTex, input.Diffuse),0.5f);

	float4 color = diff;

	// ツヤ消し
	input.Specular *= SpecularPower;

	// スペキュラーを設定する
	AddSpecular(color, pow(clamp(dot(input.Specular, nomalTex), 0.0f, 1.0f), 2.0f));

	//深度
	//float depth = length(LightPosition.xyz - input.Position.xyz) / MaxDepth;

	// ライトの計算
	input.Normal = normalize(input.Normal);
	// リムライトを設定する
	color.rgb += ApplyLimLight(input.Normal);

	// 徐々に色を塗るルール画像)
	color.rgb *= (/*modelTexture.rgb * */PaintColor.rgb) * step(texInput, 1 - Time.z);

	// 時間経過で出現させる(ルール画像)
	color.w = step(texInput, Time.x);


	//float3 eyevec = nomalize(EyePosition - pin.PositionWS.xyz);
	//float halfVec = nomalize(eyeVec - LightDirection[0]);
	//float dotH = dot(halfVector,worldNormal);
	//float3 specular = pow(max(dotH,0) * zeroL,SoecularPower) + dotH);
	//
	// あそび
	//Asobi(color,input.TexCoord);

	// 網目模様をかける
	//color.rgb *= float4(1.0f,1.0f,1.0f,1.0f) * frac(input.Position.y + Time.z);

	color.w *= DiffuseColor.w;

	// 時間経過で色を付ける
	return float4(color.xyz, color.w);
}