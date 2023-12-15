#include "PostProcess.hlsli"

Texture2D tex       : register(t0);
Texture2D dTex      : register(t1);
Texture2D rampTex   : register(t2);

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
    
    // ���̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y - power)).g);
    
    // ���̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y)).g);
    
    // �E�̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y)).g);
        
    // ��̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y + power)).g);
    
    // �΂ߍ���̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y - power)).g);
    
    // �΂߉E��̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y - power)).g);
    
    // �΂ߍ����̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x - power, uv.y + power)).g);
    
    // �΂߉E���̃e�N�X�`��
    lineflag += step(offset, dTex.Sample(samLinear, float2(uv.x + power, uv.y + power)).g);
    
    return lineflag;
}

float4 chromaticAberration(float power,float2 uv)
{
    half4 col = tex.Sample(samLinear, uv);
    
    half2 uvBase = uv - 0.5h;
    
    // R�l���g�債�����̂ɒu��������    
    half2 uvR = uvBase * (1.0h - power * 2.0h) + 0.5h;
    col.r = tex.Sample(samLinear, uvR).r;
    
    // G�l���g�債�����̂ɒu��������
    half2 uvG = uvBase * (1.0h - power) + 0.5h;
    col.g = tex.Sample(samLinear, uvG).g;
    
    
    return col;
}


float4 AddScreen(float4 color,float4 screenCol)
{
    return 1 - (1 - color) * (1 - screenCol);
}

float4 AddOverLay(float4 color, float4 overLayCol)
{
    // �I�[�o�[���C�̌v�Z�Ɏg�p����ϐ���錾
    half4 ovly_mul = 2 * color * overLayCol; // �J���[�ƃI�[�o�[���C�̏�Z
    half4 ovly_scr = 1 - 2 * (1 - color) * (1 - overLayCol); // �X�N���[�������̌v�Z

    // �I�[�o�[���C�̌��ʂ��v�Z
    half4 ovly = step(color, 0.5) * ovly_mul + (1 - step(color, 0.5)) * ovly_scr;

    // �n�C���C�g�̌v�Z�i�I�[�o�[���C�̐F��0.5���傫���ꍇ�̓I�[�o�[���C�̌��ʂ��g���A�����łȂ��ꍇ�͌��̐F���g���j
    half4 hdlgt = step(overLayCol, 0.5) * ovly_mul + (1 - step(overLayCol, 0.5)) * ovly_scr;

    // �n�C���C�g�̐F��Ԃ�
    return hdlgt;
}
float4 main(PS_INPUT input) : SV_TARGET
{   
    // �����_�Ƃ̋������Z�o
    float lengthEye = input.Color.xyz - eyePosition;
    
    float4 depthTex     = dTex.Sample(samLinear, input.Tex);
    
    // �F�����e�N�X�`��
    float4 bigBlurTex = chromaticAberration(0.0055 - depthTex.r * 0.0055, input.Tex);
    
    // 
    
    
    // ���x�Z�o
    float v = max(bigBlurTex.r, max(bigBlurTex.g, bigBlurTex.b));
    
    // �Óx�Z�o
    float v_2 = min(bigBlurTex.r, min(bigBlurTex.g, bigBlurTex.b));
    
    //// �ʓx���Ƃ�
    //bigBlurTex = saturate(bigBlurTex) * 0.85f;
    
    // �I�[�o�[���C�F���邢�ʒu�ɐԂ�(���I�����W)
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.2, 0.25, 0.0, 0.2f)) * step(0.875, v);
    
    // �I�[�o�[���C�F�Â��ʒu�ɐ�(��⎇)
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.2, 0.0, 0.25, 0.2f)) * step(v_2, 0.1);
    
    // �t�H�O
    bigBlurTex += (float4) 0.35f * depthTex.b;
    
    // �֊s�̒��o���s��
    float outlineFlag = step(AddOutLine(0.002f, input.Tex, 1.0f), 1.0f);
    
    // �֊s�����݂���ꍇ�̓e�N�X�`���ɏ�Z����
    bigBlurTex *= outlineFlag;

    return float4(bigBlurTex.xyz,1.0f);
    
}