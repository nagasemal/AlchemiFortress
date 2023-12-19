#include "pch.h"
#include "AlchemicalMachineObject.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define AM_RAGE				SimpleMath::Vector3(1, 1, 1)

// �C���ɂ����閂��
#define REPAIR_HP			30 * m_lv

// �c�h��l
#define PITCHING_VAL		0.2f

// �T�u���f����Y����]���x
#define SUBMODEL_SPEED		1.5f

// �����摜�̏c���傫��
#define SPRITE_RAGE			64.0f

//�@====================[�@3DUI�ɂĎg�p���鐔�l�̒�`�@]

//�@�@|=> HP�Q�[�W�̑傫��
#define HPGAUGE_RAGE		0.7f

//�@�@|=> �}�V���̖��O�̑傫��
#define MACHINENAME_RAGE	2.35f

//�@�@|=> Lv�\���̑傫��
#define MACHINELV_RAGE		2.0f

//�@�@|=> �}�V���̃A�C�R���̑傫��
#define MACHINEICON_RAGE	0.8f

//�@�@|=> ���@�w�̑傫��
#define MAGICCIRCLE_RAGE	0.2f

AlchemicalMachineObject::AlchemicalMachineObject() :
	m_hp(1),
	m_maxHp(1),
	m_active(),
	m_hitMouseFlag(),
	m_selectModeFlag(),
	m_objectName(),
	m_magicCircle(),
	m_machineID(MACHINE_TYPE::NONE),
	m_lv(1),
	m_machineEffectValue(),
	m_span(),
	m_color(1, 1, 1, 1),
	m_subColor(1, 1, 1, 1),
	m_rotateAnimation(),
	m_element(ELEMENT::NOMAL),
	m_powerUPFlag(),
	m_line(),
	m_dismantlingFlag(),
	m_invincibleTime(),
	m_invincibleFlag(),
	m_spawnTime(0.0f),
	m_difRedioHp(),
	m_popHPTimer(),
	m_selectModeTime()
{
	m_color = Json::ChangeColor(m_element);

}

void AlchemicalMachineObject::Update_Common()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	//�@====================[�@���G���Ԍv���@]
	if (m_invincibleFlag)
	{
		m_invincibleTime += deltaTime;

		// ����̎��ԂɒB����
		if (m_invincibleTime >= 1.0f)
		{
			//�@�@|=>�@���G���ԃ��Z�b�g
			m_invincibleTime = 0.0f;
			//�@�@|=>�@���G���ԃt���O���Z�b�g
			m_invincibleFlag = false;
		}
	}

	//�@====================[�@���o�p���ԕϐ��@]
	//�@�@|=>�@�ݒu��
	m_spawnTime += deltaTime;
	//�@�@|=>�@�}�V����Y����]
	m_rotateAnimation += deltaTime;
	//�@�@|=>�@HP�o�[�o��
	m_popHPTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();

	//�@====================[�@HP�̊����v�Z�@]
	float radio_Hp = (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()));

	//�@====================[�@���X�Ɍ���HP�̏����@]
	m_difRedioHp -= 0.3f * deltaTime;
	m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

	// �}�E�X�ɐG��Ă���or�I��������Ă���Ȃ�΃^�C�}�[�����Z����
	if (m_hitMouseFlag || m_selectModeFlag || m_hp <= 0)
	{
		m_popHPTimer += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;
	}

	m_popHPTimer = std::min(std::max(0.0f, m_popHPTimer), 1.0f);


	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//�@====================[�@���ʔ͈͈ʒu/�͈͂̍X�V�@]
	m_magicCircle.p = m_data.pos;
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage + (float)(m_lv / pSJD.GetMachineData(m_machineID).effect_rage_rate);

}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;
	m_selectModeTime += DeltaTime::GetInstance().GetDeltaTime();

}

void AlchemicalMachineObject::SelectRenderUI_Common()
{

}

void AlchemicalMachineObject::RenderHP()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	//�@====================[�@���X�ɏ�����֏o��������ϐ��@]
	float easingValCirc  = Easing::EaseInOutCirc(0.0f, 1.0f, m_popHPTimer);
	SimpleMath::Vector2 billboardUIPosition = SimpleMath::Vector2(GetPos().x, GetPos().y - easingValCirc * 50.0f);

	//�@====================[�@���@�w����]������ϐ��@]
	float easingValRotate = Easing::EaseOutQuint(0.0f, XMConvertToRadians(360.0f), m_popHPTimer);

	//�@====================[�@���X�ɓ����x��������ϐ��@]
	SimpleMath::Color color(1.0f , 1.0f, 1.0f, easingValCirc);


	//�@====================[�@�}�V����HP�Q�[�W(�O�g)�@]
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetBaseGage();
	RECT rect = { 0,0,texData.width,texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);

	// �}�V���A�C�R���̕`��ʒu�̐ݒ�Ɏg�p
	float gaugeWidthHalf = static_cast<float>(texData.width / 2);


	//�@====================[�@�}�V����HP�Q�[�W(���X�Ɍ���)�@]
	texData = SpriteLoder::GetInstance().GetMainGage();
	rect = { 0,0,static_cast<LONG>(texData.width * m_difRedioHp),texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::Red * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
			HPGAUGE_RAGE);


	//�@====================[�@�}�V����HP�Q�[�W(��C�Ɍ���)�@]
	rect = { 0,0,static_cast<LONG>(texData.width * (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()))),texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::HotPink * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);


	//�@====================[�@�}�V���̖��O�`��@]
	texData = SpriteLoder::GetInstance().GetMachineNameTexture();
	rect = SpriteCutter(texData.width / MACHINE_TYPE::NUM,texData.height,(int)m_machineID,0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>((texData.width / MACHINE_TYPE::NUM) / 2),
							static_cast<float>(texData.height / 2)),
		MACHINENAME_RAGE);


	//�@====================[�@�}�V���̖��@�w�`��@]
	texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture((int)m_machineID);
	rect = { 0,0, texData.width,texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.7f) * color,
		easingValRotate,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
							static_cast<float>(texData.height / 2)),
		MAGICCIRCLE_RAGE);


	//�@====================[�@�}�V���̃A�C�R���`��@]
	texData = SpriteLoder::GetInstance().GetMachineIconTexture();
	rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, (int)m_machineID - 1, 0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f,1.0f,1.0f,0.6f) * color,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / (MACHINE_TYPE::NUM - 1 ) / 2),
							static_cast<float>(texData.height / 2)),
		MACHINEICON_RAGE);


	//�@====================[�@�}�V����Lv�`��@]
	rect = SpriteCutter((int)SPRITE_RAGE, (int)SPRITE_RAGE, m_lv , 0);
	pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Black * color,
		0.0f,
		SimpleMath::Vector2(SPRITE_RAGE / 2, SPRITE_RAGE / 2),
		MACHINELV_RAGE);

}

void AlchemicalMachineObject::HitToMouse(MousePointer* pMP)
{
	//�@====================[�@�t���O���Z�b�g�@]
	m_hitMouseFlag = false;

	//�@====================[�@�}�E�X������g��@]
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	//�@====================[�@�}�V��To�}�E�X�@]
	if (CircleCollider(GetCircle(), mouseWolrdPos))
	{
		//�@�@|=>�@�ڐG��ʒm
		pMP->HitMachine(m_data.pos);
		m_hitMouseFlag = true;
	}

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model, DirectX::Model* ring, bool silhouette)
{
	ShareData& pSD = ShareData::GetInstance();

	//�@====================[�@���f���̍s��@]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	//�@====================[�@�f�B�t�F���T�[�͏�ɋ��_�̕����������@]
	if (m_machineID == DEFENSER)
	{
		//�@�@|=> ���_�Ƃ̋���
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		//�@�@|=> ��]�A�j���[�V������ł�����
		m_rotateAnimation = 0.0f;

		//�@�@|=> �p�x�����킹�邽�߂�90�x��]������
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));

		//�@�@|=> ���_�����ɖʂ�������
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	//�@====================[�@��ɉE���ɉ�]�@]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	//�@====================[�@��ɏc�ɗh���@]
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);

	//�@====================[�@�ǉ��p�[�c�̍s��@]
	SimpleMath::Matrix subModelData = SimpleMath::Matrix::Identity;
	subModelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	subModelData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * SUBMODEL_SPEED);
	subModelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x,m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL),m_data.pos.z);

	//�@====================[�@�ǉ��p�[�c�����݂���ꍇ�@]
	if (m_machineID == ATTACKER)
	{
		m_subColor = SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f);

		// �G�t�F�N�g�̐ݒ�
		ring->UpdateEffects([&](IEffect* effect)
			{
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// �F�ύX
				lights->SetLightDiffuseColor(0,m_subColor);
				lights->SetLightDiffuseColor(1,m_subColor);
				lights->SetLightDiffuseColor(2,m_subColor);

			});

	}

	//�@====================[�@�G�t�F�N�g�̐ݒ�@]
	model->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);

			//�@====================[�@�F�ύX�@]
			lights->SetLightDiffuseColor(0, GetColor());
			lights->SetLightDiffuseColor(1, GetColor());
			lights->SetLightDiffuseColor(2, GetColor());

		});

	//�@====================[�@�V���G�b�g�`��@]
	if (silhouette)
	{
		SilhouetteRender(model, modelData);
		// �V�F�[�_�[�̓K��
		if (ring != nullptr) 		SilhouetteRender(ring, subModelData);

	}
	//�@====================[�@�ʏ�`��@]
	else
	{
		NomalRender(model, modelData, m_color);
		// �ʏ�`��
		if (ring != nullptr) 		NomalRender(ring, subModelData, m_subColor);
	}

	//�@====================[�@�V�F�[�_�[�̉����@]
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

}

//�@====================[�@�������ɌĂ΂��֐��@]
void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
	m_spawnTime = 0.0f;
}

//�@====================[�@���̃��x���A�b�v�ɕK�v�Ȍ����ʁ@]
const int AlchemicalMachineObject::GetNextLvCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).lvUp_crystal;
}

//�@====================[�@�C�U�ɕK�v�Ȍ����ʁ@]
const int AlchemicalMachineObject::GetRepairCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).repea_crystal;
}

//�@====================[�@�j�󎞎�ɓ��錋���ʁ@]
const int AlchemicalMachineObject::GetDismantlingCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).dismantling_crystal;
}

//�@====================[�@�V���G�b�g�`��@]
void AlchemicalMachineObject::SilhouetteRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	//�@====================[�@�o�������o���I���܂ŏ��������Ȃ��@]
	if (m_spawnTime <= 1.0f) return;

	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

}

//�@====================[�@�ʏ�`��@]
void AlchemicalMachineObject::NomalRender(DirectX::Model* model, SimpleMath::Matrix matrix, SimpleMath::Color color)
{
	ShareData& pSD = ShareData::GetInstance();

	ModelShader& pMS = ModelShader::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			pMS.MachineDrawShader(color,SimpleMath::Vector4(m_spawnTime, m_span, m_invincibleTime,1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(m_element).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(m_element).GetAddressOf());

		});


	//�V�F�[�_�̓o�^���������Ă���
	pSD.GetContext()->VSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);

}

//�@====================[�@�������`��@]
void AlchemicalMachineObject::TransparentRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{

	ShareData& pSD = ShareData::GetInstance();

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().ToransparentShader();
		});

}

//�@====================[�@�J�X�^���V�F�[�_�[�����`��@]
void AlchemicalMachineObject::NotShaderRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			//�@====================[�@�������`��w��@]
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// �������菈��
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		});
}

//�@====================[�@�c�[�p�X�����_�����O�ɗp����`��@]
void AlchemicalMachineObject::WriteDepathRender(DirectX::Model* model, DirectX::Model* secondModel)
{
	ShareData& pSD = ShareData::GetInstance();

	//�@====================[�@���f���̍s��@]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	//�@====================[�@�f�B�t�F���T�[�͏�ɋ��_�̕����������@]
	if (m_machineID == DEFENSER)
	{
		//�@�@|=> ���_�Ƃ̋���
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		//�@�@|=> ��]�A�j���[�V������ł�����
		m_rotateAnimation = 0.0f;

		//�@�@|=> �p�x�����킹�邽�߂�90�x��]������
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));

		//�@�@|=> ���_�����ɖʂ�������
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	//�@====================[�@��ɉE���ɉ�]�@]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	//�@====================[�@��ɏc�ɗh���@]
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);

	//�@====================[�@�ǉ��p�[�c�̍s��@]
	SimpleMath::Matrix subModelData = SimpleMath::Matrix::Identity;
	subModelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	subModelData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * SUBMODEL_SPEED);
	subModelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * PITCHING_VAL), m_data.pos.z);


	//�@====================[�@���f���̕`��@]
	//�@�@|=>�@�{��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw();
		});

	//�@�@|=>�@�T�u���f��
	if (secondModel != nullptr)
	{
		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), subModelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
			{
				ModelShader::GetInstance().ShadowModelDraw();
			});
	}
}
