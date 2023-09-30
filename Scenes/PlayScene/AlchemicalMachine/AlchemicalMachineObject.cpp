#include "pch.h"
#include "AlchemicalMachineObject.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define AM_RAGE SimpleMath::Vector3(1, 1, 1)

// �C���ɂ����閂��
#define REPAIR_HP 30 * m_lv

AlchemicalMachineObject::AlchemicalMachineObject():
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
	m_color(1,1,1,1),
	m_rotateAnimation(),
	m_element(ELEMENT::NOMAL),
	m_powerUPFlag(),
	m_line(),
	m_dismantlingFlag()
{
	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(170, 490),
		SimpleMath::Vector2(1.0f, 1.0f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(250, 490),
		SimpleMath::Vector2(1.0f, 1.0f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(250, 650),
		SimpleMath::Vector2(1.0f, 1.0f));

}

void AlchemicalMachineObject::SelectUpdate_Common()
{

	m_dismantlingFlag = false;

	DataManager& pDataM = *DataManager::GetInstance();
	//auto pSJD = &ShareJsonData::GetInstance();

	// LvUp�p�̑I���{�b�N�X�̐ݒ�
	m_selectLvUpBox->HitMouse();

	if (m_selectLvUpBox->ClickMouse()) 		LvUp();

	// �C�U�p�̑I���{�b�N�X�̐ݒ�
	m_repairBox->HitMouse();

	// �C�U�I���{�b�N�X�������@���݂�Crystal�ʂ���C�U�Ɋ|����Crystal�ʂ�0�ȏ�Ȃ�Ύ��s
	if (m_repairBox->ClickMouse() && pDataM.GetNowCrystal() - GetRepairCrystal() >= 0)
	{
		m_hp = m_maxHp;
		pDataM.SetNowMP(pDataM.GetNowCrystal() - GetRepairCrystal());
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
	RECT rect_UI = SpriteCutter(64, 64, m_lv, 0);
	m_selectLvUpBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetNumberTexture(), rect_UI);

	// �C�U�pUI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::REPAIR, 0);
	m_repairBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
						SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));

	// ��̗pUI
	rect_UI = SpriteCutter(64, 64, SpriteLoder::DISMATIONG, 0);
	m_dismantlingBox->DrawUI(pSL.GetSelectBoxTexture(), pSL.GetUIIcons(), rect_UI,
							 SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

}

void AlchemicalMachineObject::HitToObject(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	//InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// �I�u�W�F�N�g�ƃ}�E�X�|�C���^�[�̓����蔻��
	if (CircleCollider(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model, DirectX::Model* ring)
{
	ShareData& pSD = ShareData::GetInstance();

	m_rotateAnimation += DeltaTime::GetInstance().GetDeltaTime();

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);

	// �f�B�t�F���T�[�^�͏�ɋ��_�̕���������
	if (m_machineID == DEFENSER)
	{
		m_rotateAnimation = 0.0f;
		modelData *= SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180.0f));
		modelData *= SimpleMath::Matrix::CreateFromQuaternion(LookAt({ 0.0f,m_data.pos.y,0.0f }));
	}

	// ��ɉE���ɉ�]
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);

	// ��ɏc�ɗh���
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f), m_data.pos.z);

	// �ǉ��p�[�c�����݂���ꍇ
	if (ring != nullptr)
	{
		SimpleMath::Matrix ringData = SimpleMath::Matrix::Identity;
		ringData = SimpleMath::Matrix::CreateScale(m_data.rage);
		ringData *= SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
		ringData *= SimpleMath::Matrix::CreateTranslation
		(m_data.pos.x,
			m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f),
			m_data.pos.z);

		// �G�t�F�N�g�̐ݒ�
		ring->UpdateEffects([&](IEffect* effect)
			{
				// ����̓��C�g�����~����
				auto lights = dynamic_cast<IEffectLights*>(effect);
				// �F�ύX
				lights->SetLightDiffuseColor(0, SimpleMath::Color((float)m_powerUPFlag, (float)m_powerUPFlag, 0.0f, 1.0f));
			});

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// �[�x�X�e���V���X�e�[�g�̐ݒ�
				pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilShadow().Get(), 1);
				pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
			});

		pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
		pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// �[�x�X�e���V���X�e�[�g�̐ݒ�
				pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
			});

	}

	// �G�t�F�N�g�̐ݒ�
	model->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, GetColor());
		});

	{
		// �d�Ȃ����ہA�e��`��
		model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// �[�x�X�e���V���X�e�[�g�̐ݒ�
				pSD.GetContext()->OMSetDepthStencilState(pSD.GetStencilShadow().Get(), 1);
				pSD.GetContext()->PSSetShader(pSD.GetModelShadowShader().Get(), nullptr, 0);
			});

		pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);
		pSD.GetContext()->OMSetDepthStencilState(nullptr, 0);

		model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
			{
				// �[�x�X�e���V���X�e�[�g�̐ݒ�
				pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
			});

	}
}

void AlchemicalMachineObject::SummonAM(SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;
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
