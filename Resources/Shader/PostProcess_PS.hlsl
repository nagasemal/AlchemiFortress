#include "PostProcess.hlsli"

Texture2D tex : register(t0);
Texture2D dTex : register(t1);

SamplerState samLinear : register(s0);

float4 blur(float power,float2 uv)
{
    float center = 1.0f - power * 9.0f;
    float4 output = 0;
    for (int i = 0; i < 9; i++)
    {
        output += tex.Sample(samLinear, uv + float2(((i % 3) - 1) * 0.01f, ((i / 3) - 1) * 0.01f)) * power;
    }
    
    output += tex.Sample(samLinear, uv) * center;
    
    
    return output;
}

float AddOutLine(float power, float2 uv, float offset)
{
    float lineflag = 0.0f;
    
    lineflag -= 8 * step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y)).g);
    
    // 上のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y - power)).g);
    
    // 左のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y)).g);
    
    // 右のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y)).g);
        
    // 上のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y + power)).g);
    
    // 斜め左上のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y - power)).g);
    
    // 斜め右上のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y - power)).g);
    
    // 斜め左下のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y + power)).g);
    
    // 斜め右下のテクスチャ
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y + power)).g);
    
    return lineflag;
}

float4 chromaticAberration(float power,float2 uv)
{
    half4 col = tex.Sample(samLinear, uv);
    
    half2 uvBase = uv - 0.5h;
    
    // R値を拡大したものに置き換える    
    half2 uvR = uvBase * (1.0h - power * 2.0h) + 0.5h;
    col.r = tex.Sample(samLinear, uvR).r;
    
    // G値を拡大したものに置き換える
    half2 uvG = uvBase * (1.0h - power) + 0.5h;
    col.g = tex.Sample(samLinear, uvG).g;
    
    
    return col;
}


float4 AddScreen(float4 color,float4 screenCol)
{
    return 1 - (1 - color) * (1 - screenCol);
}

float4 AddOverLay(float4 color,float4 overLayCol)
{
    
    half4 ovly_mul = 2 * color * overLayCol;
    half4 ovly_scr = 1 - 2 * (1 - color) * (1 - overLayCol);
    half4 ovly = step(color, 0.5) * ovly_mul + (1 - step(color, 0.5)) * ovly_scr;
    half4 hdlgt = step(overLayCol, 0.5) * ovly_mul + (1 - step(overLayCol,0.5)) * ovly_scr;

    return hdlgt;

}

float4 main(PS_INPUT input) : SV_TARGET
{   
    // 注視点との距離を算出
    float lengthEye = input.Color.xyz - eyePosition;
    
    float4 depthTex     = dTex.Sample(samLinear, input.Tex);
    
    // 大ボケテクスチャ
    float4 bigBlurTex = chromaticAberration(0.0055 - depthTex.r * 0.0055, input.Tex);
    
    // 明度算出
    float v = max(bigBlurTex.r, max(bigBlurTex.g, bigBlurTex.b));
    
    // 明るい位置に赤み(ややオレンジ)をオーバーレイ
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.5, 0.35, 0.0, 0.2f)) * step(0.925 , v );
    
    // 暗い位置に青み(やや紫)をオーバーレイ
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.2, 0.0, 0.5, 0.2f)) * step(v , 0.125);
    
    // 光の当たる位置に赤みをオーバーレイ
    //bigBlurTex += AddScreen(bigBlurTex, float4(0.8, 0.65, 0.0, 0.2f)) * depthTex.g;
    
    float outlineFlag = step(AddOutLine(0.0025f, input.Tex, 1.0f), 1.0f);
    
    bigBlurTex *= outlineFlag;

    return float4(bigBlurTex.xyz,1.0f);
    
}