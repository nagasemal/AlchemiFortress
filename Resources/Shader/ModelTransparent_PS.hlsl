//struct PS_INPUT
//{
//	float4 Pos : SV_POSITION;
//	float4 Color: COLOR;
//	float2 Tex : TEXCOORD;
//};
//
//Texture2D tex : register(t0);
//SamplerState samLinear : register(s0);

float4 main() : SV_TARGET
{

	return float4(0.8f, 0.8f, 0.8f, 0.4f);

}