#include "MyModelShader.hlsli"

Texture2D ModelTexture : register(t1);
Texture2D NomalTexture : register(t2);

// リムライトの設定
// 視線ベクトルと法線ベクトルが直角に近いほどライトの値を大きくする
float4 ApplyLimLight(float3 normal)
{
	//　リムライト
    float3 invEye = normalize(mul(WorldInverseTranspose, EyePosition));
    float rim = pow(1.0 - clamp(dot(invEye, normal), 0.0, 1.0), 5.0);

    float3 eyedif = normalize(Eyes.xyz - EyePosition);


	// 視線ベクトルとライトベクトルも考慮に入れる
	// pow = 強度増
	// max = 1.0 以上に行かないようにする
    float dotLE_1 = pow(max(dot(eyedif, normalize(LightDirection[0])), 1.0), 30.0);
    float dotLE_2 = pow(max(dot(eyedif, normalize(LightDirection[1])), 1.0), 30.0);
    float dotLE_3 = pow(max(dot(eyedif, normalize(LightDirection[2])), 1.0), 30.0);

    return lerp(0, LimLightColor, rim * dotLE_1 * dotLE_2 * dotLE_3);

}

float4 ApplyPointLight(float3 position, float3 lightPos,float power)
{
    // 光源の方向
    float3 lightDirection = position - lightPos;
    
    // 光源までの距離
    float lightDistance = length(lightDirection);

    // 光源からの距離の影響
    float atten = saturate(1.0f / (lightDistance * lightDistance));
    
    return atten * power;
}

void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

float4 main(PSInput input) : SV_TARGET0
{    
    // テクスチャ取得
    float texInput = Texture.Sample(Sampler, input.TexCoord);

	// テクスチャ取得
    float3 modelTexture = ModelTexture.Sample(Sampler, input.TexCoord);
   
    
	// ライトの計算
    input.Normal = normalize(input.Normal);
	
	// ノーマルマップ取得
    float3 nomalTex = NomalTexture.Sample(Sampler, input.TexCoord);

    float4 diff = pow(dot(nomalTex, input.Diffuse.rgb), 0.5f) + ApplyLimLight(input.Normal);

    float4 color = diff;
    
    color.rgb *= PaintColor.rgb;

    // ツヤ消し
    input.Specular *= 0.2f;
    
	// スペキュラーを設定する
    AddSpecular(color, pow(clamp(dot(input.Specular, nomalTex), 0.0f, 1.0f), 1.0f));
    
    color.rgb *= modelTexture.rgb;
    
    color *= texInput;
    
    color += float4(0.7, 0.7, 0.4, 0.0f) * ApplyPointLight(input.Position.xyz, LightPosition.xyz,1.0f);
    
    color.w = 1.0f;
    
    // クリスタルの周辺を発光させる
    for (int i = 0; i < 10; i++)
    {
        color += float4(0.7, 0.2, 0.7, 0.0f) * ApplyPointLight(input.Position.xyz, CrystalPosition[i].xyz,2.0f);
    }
    
    
    
    // 徐々に色を塗るルール画像)

 
    //徐々に色を塗るルール画像)
    //color.rgb *= ( /*modelTexture.rgb * */PaintColor.rgb) * step(texInput, 1);
    
    //時間経過で出現させる(ルール画像)

    //color.w = texInput;
    
	
    return float4(color.xyz, color.w);
}