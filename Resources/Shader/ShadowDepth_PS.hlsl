#include "MyModelShader.hlsli"

float4 main(PSInput pin) : SV_TARGET0
{    
    // 注視点の方向
    float3 Direction = pin.Position - Eyes;
    
    // 注視点とカメラ位置の距離(離れるほど視点範囲を大きくするため)
    float d = distance(Eyes.xyz, EyePosition);
    
    // 注視点までの距離
    float Distance = length(Direction) / (d * 0.35);
    // 注視点からの距離の影響
    float depthAtten = saturate(1.0f / (Distance * Distance));
    float power = 1.25;
    
    depthAtten *= power;
    
    
    //float depth = smoothstep(-50.0, 0.0f, length(Eyes.xyz - pin.Position.xyz));
    
    // 法線を正規化
    float3 worldNormal = normalize(pin.Normal);
    // 光の強さを内積から算出する
    float3 dotL = dot(-Direction, worldNormal);
    // 表面の場合は１、裏面の場合は0
    float3 zeroL = step(0, dotL);
    // 裏面の場合は黒になる
    float3 diffuse = zeroL * dotL;
    
    // 注視点範囲,アウトライン描画,深度値,未設定
    return float4(depthAtten, Time.x, 0.0f, 1.0f);
    
}