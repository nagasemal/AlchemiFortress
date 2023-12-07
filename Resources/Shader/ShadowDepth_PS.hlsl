#include "MyModelShader.hlsli"

float4 main(PSInput pin) : SV_TARGET0
{    
    // �����_�̕���
    float3 Direction = pin.Position - Eyes;
    
    // �����_�ƃJ�����ʒu�̋���(�����قǎ��_�͈͂�傫�����邽��)
    float d = distance(Eyes.xyz, EyePosition);
    
    // �����_�܂ł̋���
    float Distance = length(Direction) / (d * 0.35);
    // �����_����̋����̉e��
    float depthAtten = saturate(1.0f / (Distance * Distance));
    float power = 1.25;
    
    depthAtten *= power;
    
    
    //float depth = smoothstep(-50.0, 0.0f, length(Eyes.xyz - pin.Position.xyz));
    
    // �@���𐳋K��
    float3 worldNormal = normalize(pin.Normal);
    // ���̋�������ς���Z�o����
    float3 dotL = dot(-Direction, worldNormal);
    // �\�ʂ̏ꍇ�͂P�A���ʂ̏ꍇ��0
    float3 zeroL = step(0, dotL);
    // ���ʂ̏ꍇ�͍��ɂȂ�
    float3 diffuse = zeroL * dotL;
    
    // �����_�͈�,�A�E�g���C���`��,�[�x�l,���ݒ�
    return float4(depthAtten, Time.x, 0.0f, 1.0f);
    
}