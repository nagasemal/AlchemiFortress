#include "MyModelShader.hlsli"

Texture2D MachineTexture: register(t1);
Texture2D NomalTexture: register(t2);

void AddSpecular(inout float4 color, float3 specular)
{
	color.rgb += specular * color.a;
}

float ComputeFogFactor(float4 position)
{
	// ���ɍs���قǍ�������
	return saturate(dot(position, float4(0.0f, -0.05f, 0.0f, 0.0f)));
}

// �t�H�O�̐ݒ�
void ApplyFog(inout float4 color, float fogFactor)
{
	color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}

// �������C�g�̐ݒ�
// �����x�N�g���Ɩ@���x�N�g�������p�ɋ߂��قǃ��C�g�̒l��傫������
float4 ApplyLimLight(float3 normal)
{
	//�@�������C�g
	float3 invEye = normalize(mul(WorldInverseTranspose, EyePosition));
	float rim = pow(1.0 - clamp(dot(invEye,normal), 0.0, 1.0), 5.0);


	float3 eyedif = normalize(Eyes - EyePosition);


	// �����x�N�g���ƃ��C�g�x�N�g�����l���ɓ����
	// pow = ���x��
	// max = 1.0 �ȏ�ɍs���Ȃ��悤�ɂ���
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

// �m�[�}���}�b�v�̐ݒ�
void ApplyNomalMap()
{
	//float3 normalMap = (texture2D(texture, vTextureCoord) * 2.0 - 1.0).rgb;
}

// �O���[�X�P�[����
float ChangeGray(float4 col)
{
	return col.r * 0.3f + col.g * 0.6f + col.b * 0.1f;
}

// �Z�s�A��
float4 ChangeSepia(float4 col)
{
	float g = ChangeGray(col);
	return float4(g * 1.44f, g * 0.99f, g * 0.57f, col.a);
}

float4 main(PSInput input) : SV_TARGET0
{

	float texInput = Texture.Sample(Sampler, input.TexCoord);

	// �e�N�X�`���擾
	float3 modelTexture = MachineTexture.Sample(Sampler, input.TexCoord);

	// �m�[�}���}�b�v�擾
	float3 nomalTex = NomalTexture.Sample(Sampler, input.TexCoord);

	float4 diff = pow(dot(nomalTex, input.Diffuse),0.5f);

	float4 color = diff;

	// �c������
	input.Specular *= SpecularPower;

	// �X�y�L�����[��ݒ肷��
	AddSpecular(color, pow(clamp(dot(input.Specular, nomalTex), 0.0f, 1.0f), 2.0f));

	//�[�x
	//float depth = length(LightPosition.xyz - input.Position.xyz) / MaxDepth;

	// ���C�g�̌v�Z
	input.Normal = normalize(input.Normal);
	// �������C�g��ݒ肷��
	color.rgb += ApplyLimLight(input.Normal);

	// ���X�ɐF��h�郋�[���摜)
	color.rgb *= (/*modelTexture.rgb * */PaintColor.rgb) * step(texInput, 1 - Time.z);

	// ���Ԍo�߂ŏo��������(���[���摜)
	color.w = step(texInput, Time.x);


	//float3 eyevec = nomalize(EyePosition - pin.PositionWS.xyz);
	//float halfVec = nomalize(eyeVec - LightDirection[0]);
	//float dotH = dot(halfVector,worldNormal);
	//float3 specular = pow(max(dotH,0) * zeroL,SoecularPower) + dotH);
	//
	// ������
	//Asobi(color,input.TexCoord);

	// �Ԗږ͗l��������
	//color.rgb *= float4(1.0f,1.0f,1.0f,1.0f) * frac(input.Position.y + Time.z);

	color.w *= DiffuseColor.w;

	// ���Ԍo�߂ŐF��t����
	return float4(color.xyz, color.w);
}