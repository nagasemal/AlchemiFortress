Texture2D Texture : register(t0);
sampler Sampler : register(s0);

// 定数バッファ
cbuffer Parameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);	// ディフューズ色
    float3 EmissiveColor            : packoffset(c1);	// エミッシブ色
    float3 SpecularColor            : packoffset(c2);	// スペキュラ色
    float  SpecularPower            : packoffset(c2.w);	// スペキュラの強度

    float3 LightDirection[3]        : packoffset(c3);	// ライトの方向
    float3 LightDiffuseColor[3]     : packoffset(c6);	// ライトの色
    float3 LightSpecularColor[3]    : packoffset(c9);	// ライトのスペキュラ色

    float3 EyePosition              : packoffset(c12);	// カメラの位置

    float3 FogColor                 : packoffset(c13);	// フォグの色
    float4 FogVector                : packoffset(c14);	// フォグのかかり具合

    float4x4 World                  : packoffset(c15);	// ワールド行列
    float3x3 WorldInverseTranspose  : packoffset(c19);	// ワールド逆転置行列
    float4x4 WorldViewProj          : packoffset(c22);	// ワールド行列 × ビュー行列 × プロジェクション行列

};

// 定数バッファ
cbuffer Parameters_User : register(b1)
{
    // シャドウマップ上の射影空間へ変換する行列
    float4x4 LightViewProj          : packoffset(c0);
    float3   LightPosition          : packoffset(c4);
    float    MaxDepth               : packoffset(c4.w);
    float    Offset                 : packoffset(c5);
    float    VSMFilterEnable        : packoffset(c5.y);
};

// プログラム側で変化する時間変数を入れる
cbuffer TimeUpdate : register(b2)
{
    float4 Time;
    float4 PaintColor;
}

struct PSInput
{
    float4 PositionPS   : SV_Position;
    float4 Diffuse      : COLOR0;
    float3 Specular     : COLOR1;
    float2 TexCoord     : TEXCOORD0;
    float4 LightViewPos : TEXCOORD1;
    float4 Position     : Position;
};

// 頂点シェーダーの入力
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float4 Tangent  : TANGENT;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};
