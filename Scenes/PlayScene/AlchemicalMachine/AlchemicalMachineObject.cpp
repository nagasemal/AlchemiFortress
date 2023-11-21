#include "pch.h"
#include "AlchemicalMachineObject.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define AM_RAGE SimpleMath::Vector3(1, 1, 1)

// �C���ɂ����閂��
#define REPAIR_HP 30 * m_lv

// HP�Q�[�W�̑傫��
#define HPGAUGE_RAGE 0.7f

// �}�V���̖��O�̑傫��
#define MACHINENAME_RAGE 2.35f

// Lv�\���̑傫��
#define MACHINELV_RAGE 2.0f

// �}�V���̃A�C�R���̑傫��
#define MACHINEICON_RAGE 0.8f

// ���@�w�̑傫��
#define MAGICCIRCLE_RAGE 0.2f

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
	m_spawnTime(1.0f),
	m_difRedioHp(),
	m_popHPTimer()
{
	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(150, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(210, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(270, 600),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_color = Json::ChangeColor(m_element);

}

void AlchemicalMachineObject::Update_Common()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// ���G���Ԍv��
	if (m_invincibleFlag)
	{
		m_invincibleTime += deltaTime;

		// ����̎��ԂɒB������t���O��False�ɂ��Ė��G��Ԃ���������
		if (m_invincibleTime >= 1.0f)
		{
			m_invincibleTime = 0.0f;
			m_invincibleFlag = false;
		}
	}

	// �o�����̉��o�Ɏg���^�C�}�[
	m_spawnTime += deltaTime;
	// ��]�Ɏg�p����l
	m_rotateAnimation += deltaTime;

	float radio_Hp = (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()));

	// ���X�Ɍ���HP�̏���
	m_difRedioHp -= 0.3f * deltaTime;
	m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

	// �G�ꂽ��o������HP�o�[UI�p�̎��ԕϐ����X�V����
	m_popHPTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();

	// �}�E�X�ɐG��Ă���or�I��������Ă���Ȃ�΃^�C�}�[�����Z����
	if (m_hitMouseFlag || m_selectModeFlag || m_hp <= 0)
	{
		m_popHPTimer += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;
	}

	m_popHPTimer = std::min(std::max(0.0f, m_popHPTimer), 1.0f);

}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;

	DataManager& pDataM = *DataManager::GetInstance();
	//auto pSJD = &ShareJsonData::GetInstance();

	// LvUp�p�̑I���{�b�N�X�̐ݒ�
	m_selectLvUpBox->HitMouse();

	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	m_selectLvUpBox->SetActiveFlag(m_lv <= MAX_LV && pDM.GetNowCrystal() - GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse()) 		LvUp();

	// �C�U�p�̑I���{�b�N�X�̐ݒ�
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - GetRepairCrystal() >= 0 && m_hp < m_maxHp);

	// �C�U�I���{�b�N�X�������@���݂�Crystal�ʂ���C�U�Ɋ|����Crystal�ʂ�0�ȏ�Ȃ�Ύ��s
	if (m_repairBox->ClickMouse())
	{
		m_hp = m_maxHp;
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - GetRepairCrystal());
	}

	// �j��p�̑I���{�b�N�X
	m_dismantlingBox->HitMouse();

	// �j��I���{�b�N�X�������@���݂�Crystal�ʂ��瑝������Crystal�ʂ��ő�l�ȉ��Ȃ�Ύ��s
	if (m_dismantlingBox->ClickMouse() && pDataM.GetNowCrystal() + GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX())
	{
		m_dismantlingFlag = true;

		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + GetDismantlingCrystal());
	}

}

void AlchemicalMachineObject::SelectRenderUI_Common()
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	// LVUP�pUI
	m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

	// �C�U�pUI
	m_repairBox->DrawUI(SpriteLoder::REPAIR);

	// ��̗pUI
	m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);

}

void AlchemicalMachineObject::RenderHP()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	// ���X�ɏ�����֏o��������ϐ�
	float easingValCirc  = Easing::EaseInOutCirc(0.0f, 1.0f, m_popHPTimer);
	SimpleMath::Vector2 billboardUIPosition = SimpleMath::Vector2(GetPos().x, GetPos().y - easingValCirc * 50.0f);

	// ���@�w����]������ϐ�
	float easingValRotate = Easing::EaseOutQuint(0.0f, XMConvertToRadians(360.0f), m_popHPTimer);

	SimpleMath::Color color(1.0f , 1.0f, 1.0f, easingValCirc);

	// �}�V����HP�Q�[�W(�O�g)========
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetBaseGage();
	RECT rect = { 0,0,texData.width,texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	// �}�V���A�C�R���̕`��ʒu�̐ݒ�Ɏg�p
	float gaugeWidthHalf = texData.width / 2;

	// �}�V����HP�Q�[�W(���X�Ɍ���)===========
	texData = SpriteLoder::GetInstance().GetMainGage();
	rect = { 0,0,static_cast<LONG>(texData.width * m_difRedioHp),texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::Red * color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	// �}�V����HP�Q�[�W(��C�Ɍ���)===========
	rect = { 0,0,static_cast<LONG>(texData.width * (static_cast<float>(GetHP()) / static_cast<float>(GetMAXHP()))),texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::HotPink * color,
		0.0f,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		HPGAUGE_RAGE);

	//�@�}�V���̖��O��`��========
	texData = SpriteLoder::GetInstance().GetMachineNameTexture();
	rect = SpriteCutter(texData.width / MACHINE_TYPE::NUM,texData.height,(int)m_machineID,0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		color,
		0.0f,
		SimpleMath::Vector2((texData.width / MACHINE_TYPE::NUM) / 2, texData.height / 2),
		MACHINENAME_RAGE);

	// �}�V���̖��@�w��`��========
	texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture((int)m_machineID);
	rect = { 0,0, texData.width,texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.7f) * color,
		easingValRotate,
		SimpleMath::Vector2(texData.width / 2, texData.height / 2),
		MAGICCIRCLE_RAGE);

	//�@�}�V���̃A�C�R����`��========
	texData = SpriteLoder::GetInstance().GetMachineIconTexture();
	rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, (int)m_machineID - 1, 0);

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		SimpleMath::Color(1.0f,1.0f,1.0f,0.6f) * color,
		0.0f,
		SimpleMath::Vector2(texData.width / (MACHINE_TYPE::NUM - 1 ) / 2, texData.height / 2),
		MACHINEICON_RAGE);

	//�@�}�V���̃��x����`��=======
	rect = SpriteCutter(64, 64, m_lv , 0);
	pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Black * color,
		0.0f,
		SimpleMath::Vector2(64 / 2, 64 / 2),
		MACHINELV_RAGE);

}

void AlchemicalMachineObject::HitToMouse(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	//InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// �I�u�W�F�N�g�ƃ}�E�X�|�C���^�[�̓����蔻��
	if (CircleCollider(GetCircle(), mouseWolrdPos))
	{
		// �}�V���ɓ����������Ƃ�m�点��
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

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	// �f�B�t�F���T�[�^�͏�ɋ��_�̕���������
	if (m_machineID == DEFENSER)
	{
		// ���_�Ƃ̋���
		SimpleMath::Vector3 basepos = SimpleMath::Vector3() - m_data.pos;

		m_rotateAnimation = 0.0f;
		modelData *= SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(
			SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, basepos));
	}

	// ��ɉE���ɉ�]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	// ��ɏc�ɗh���
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.2f), m_data.pos.z);

	SimpleMath::Matrix ringData = SimpleMath::Matrix::Identity;
	ringData = SimpleMath::Matrix::CreateScale(m_data.rage);
	ringData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
	ringData *= SimpleMath::Matrix::CreateTranslation
	(m_data.pos.x,
		m_data.pos.y + (sinf(m_rotateAnimation) * 0.2f),
		m_data.pos.z);

	// �ǉ��p�[�c�����݂���ꍇ
	if (m_machineID == ATTACKER)
	{
		m_subColor = SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f);

		// �G�t�F�N�g�̐ݒ�
		ring->UpdateEffects([&](IEffect* effect)
			{
				// ����̓��C�g�����~����
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// �F�ύX
				lights->SetLightDiffuseColor(0,m_subColor);
				lights->SetLightDiffuseColor(1,m_subColor);
				lights->SetLightDiffuseColor(2,m_subColor);

			});

	}

	// �G�t�F�N�g�̐ݒ�
	model->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// �F�ύX
			lights->SetLightDiffuseColor(0, GetColor());
			lights->SetLightDiffuseColor(1, GetColor());
			lights->SetLightDiffuseColor(2, GetColor());

		});

	// �V���G�b�g�`��
	if (silhouette)
	{
		SilhouetteRender(model, modelData);
		// �V�F�[�_�[�̓K��
		if (ring != nullptr) 		SilhouetteRender(ring, ringData);

	}
	// �ʏ�`��
	else
	{
		NomalRender(model, modelData, m_color);
		// �ʏ�`��
		if (ring != nullptr) 		NomalRender(ring, ringData, m_subColor);
	}

	// �V�F�[�_�[�̉���
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

}

// �������ɌĂ΂��֐�
void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
	m_spawnTime = 0.0f;
}

bool AlchemicalMachineObject::GetRepairFlag()
{
	return m_repairBox->ClickMouse();
}

bool AlchemicalMachineObject::GetLvUpFlag()
{
	return m_selectLvUpBox->ClickMouse();
}

const int AlchemicalMachineObject::GetNextLvCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).lvUp_crystal;
}

const int AlchemicalMachineObject::GetRepairCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).repea_crystal;
}

const int AlchemicalMachineObject::GetDismantlingCrystal()
{
	auto pSJD = &ShareJsonData::GetInstance();

	return m_lv * (int)pSJD->GetMachineData(m_machineID).dismantling_crystal;
}

SelectionBox* AlchemicalMachineObject::GetMenuButton(int buttonType)
{
	switch (buttonType)
	{
	case 0:
		return m_repairBox.get();
	case 1:
		return m_dismantlingBox.get();
	case 2:
		return m_selectLvUpBox.get();
	}

	// ����ȊO�̏ꍇ�͂Ƃ肠�����C�U�I��Box��I������
	return m_repairBox.get();

}

void AlchemicalMachineObject::SilhouetteRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	// �o�������o���I���܂ŏ��������Ȃ�
	if (m_spawnTime <= 1.0f) return;

	ShareData& pSD = ShareData::GetInstance();

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

}

void AlchemicalMachineObject::NomalRender(DirectX::Model* model, SimpleMath::Matrix matrix, SimpleMath::Color color)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().ModelDrawShader(color,SimpleMath::Vector4(m_spawnTime, m_span, m_invincibleTime,1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(m_element).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(m_element).GetAddressOf());

		});

	//�V�F�[�_�̓o�^���������Ă���
	pSD.GetContext()->VSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);

}

void AlchemicalMachineObject::TransparentRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{

	ShareData& pSD = ShareData::GetInstance();

	// �d�Ȃ����ہA�e��`��
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().ToransparentShader();


		});

}

void AlchemicalMachineObject::NotShaderRender(DirectX::Model* model, SimpleMath::Matrix matrix)
{
	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), matrix, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			//�������`��w��
			ID3D11BlendState* blendstate = pSD.GetCommonStates()->NonPremultiplied();
			// �������菈��
			pSD.GetContext()->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		});
}
