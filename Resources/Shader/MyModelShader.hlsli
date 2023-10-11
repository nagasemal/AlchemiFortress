Texture2D Texture : register(t0);
sampler Sampler : register(s0);

// �萔�o�b�t�@
cbuffer Parameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);	// �f�B�t���[�Y�F
    float3 EmissiveColor            : packoffset(c1);	// �G�~�b�V�u�F
    float3 SpecularColor            : packoffset(c2);	// �X�y�L�����F
    float  SpecularPower            : packoffset(c2.w);	// �X�y�L�����̋��x

    float3 LightDirection[3]        : packoffset(c3);	// ���C�g�̕���
    float3 LightDiffuseColor[3]     : packoffset(c6);	// ���C�g�̐F
    float3 LightSpecularColor[3]    : packoffset(c9);	// ���C�g�̃X�y�L�����F

    float3 EyePosition              : packoffset(c12);	// �J�����̈ʒu

    float3 FogColor                 : packoffset(c13);	// �t�H�O�̐F
    float4 FogVector                : packoffset(c14);	// �t�H�O�̂�����

    float4x4 World                  : packoffset(c15);	// ���[���h�s��
    float3x3 WorldInverseTranspose  : packoffset(c19);	// ���[���h�t�]�u�s��
    float4x4 WorldViewProj          : packoffset(c22);	// ���[���h�s�� �~ �r���[�s�� �~ �v���W�F�N�V�����s��

};

// �萔�o�b�t�@
cbuffer Parameters_User : register(b1)
{
    // �V���h�E�}�b�v��̎ˉe��Ԃ֕ϊ�����s��
    float4x4 LightViewProj          : packoffset(c0);
    float3   LightPosition          : packoffset(c4);
    float    MaxDepth               : packoffset(c4.w);
    float    Offset                 : packoffset(c5);
    float    VSMFilterEnable        : packoffset(c5.y);
};

// �v���O�������ŕω����鎞�ԕϐ�������
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

// ���_�V�F�[�_�[�̓���
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float4 Tangent  : TANGENT;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};
