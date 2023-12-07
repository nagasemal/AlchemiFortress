#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"


#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#include "Scenes/Commons/DrawArrow.h"

struct position2D
{
	double x;
	double y;
};

MachineExplanation::MachineExplanation():
	m_moveTime(),
	m_modelPos(),
	m_hitFlag()
{
}

MachineExplanation::~MachineExplanation()
{
}

void MachineExplanation::Initialize()
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_moveTime = 0;

	m_camera = std::make_unique<Camera>();

	//�@====================[�@�}�V��UI�̏����擾�@]
	UI_Data uiData = pSJD.GetUIData("ExplanationOffset");

	m_data.pos  = uiData.pos;
	m_data.rage = uiData.rage;

	uiData = pSJD.GetUIData("ExplanationSpawn");
	m_spawnBox			= std::make_unique<SelectionBox>(uiData.pos, uiData.rage);
	m_spawnBox			->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationLvUp");
	m_selectLvUpBox		= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_selectLvUpBox		->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationRepair");
	m_repairBox			= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_repairBox			->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationDestory");
	m_dismantlingBox	= std::make_unique<SelectionBox>(uiData.pos,uiData.rage);
	m_dismantlingBox	->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationNext");
	m_nextMachineArrow	= std::make_unique<DrawArrow>(uiData.pos, uiData.rage,3);
	m_nextMachineArrow	->SetKey(uiData.key);

	uiData = pSJD.GetUIData("ExplanationBack");
	m_backMachineArrow	= std::make_unique<DrawArrow>(uiData.pos, uiData.rage,1);
	m_backMachineArrow	->SetKey(uiData.key);

}

void MachineExplanation::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	//�@====================[�@UI�\���}�V������]������@]
	m_moveTime += deltaTime.GetDeltaTime();

	//�@====================[�@�I���{�b�N�X�̐ݒ�@]
	//�@�@|=>�@�ݒu
	m_spawnBox->HitMouse();
	m_spawnBox->SetActiveFlag(false);
	//�@�@|=>�@LvUP
	m_selectLvUpBox->HitMouse();
	m_selectLvUpBox->SetActiveFlag(false);

	//�@�@|=>�@�C�U
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(false);

	//�@�@|=>�@�j��
	m_dismantlingBox->HitMouse();
	m_dismantlingBox->SetActiveFlag(false);

	// ���̃}�V����
	m_nextMachineArrow->HitMouse();

	// �O�̃}�V����
	m_backMachineArrow->HitMouse();

}

void MachineExplanation::Update_MachineData(AlchemicalMachineObject* object)
{

	DataManager& pDataM = *DataManager::GetInstance();
	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// None�}�V����I�����Ă���Ƃ��̂ݔ�������
	m_spawnBox->SetActiveFlag(object->GetModelID() == MACHINE_TYPE::NONE);

	//�@====================[�@�������^�[���@]
	//�@�@|=>�@�I�𒆂�None�}�V��
	if (m_spawnBox->GetActiveFlag()) return;

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	m_selectLvUpBox->SetActiveFlag(object->GetLv() <= 5 && pDM.GetNowCrystal() - object->GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse())
	{
		object->LvUp();
	}
	// �C�U�p�̑I���{�b�N�X�̐ݒ�
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - object->GetRepairCrystal() >= 0 && object->GetHP() < object->GetMAXHP());

	// �C�U�I���{�b�N�X�������@���݂�Crystal�ʂ���C�U�Ɋ|����Crystal�ʂ�0�ȏ�Ȃ�Ύ��s
	if (m_repairBox->ClickMouse())
	{
		object->SetHP(object->GetMAXHP());
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - object->GetRepairCrystal());
	}

	// �j��p�̑I���{�b�N�X
	m_dismantlingBox->SetActiveFlag(pDataM.GetNowCrystal() + object->GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX());

	// �j��I���{�b�N�X�������@���݂�Crystal�ʂ��瑝������Crystal�ʂ��ő�l�ȉ��Ȃ�Ύ��s
	if (m_dismantlingBox->ClickMouse())
	{
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + object->GetDismantlingCrystal());
	}

}

void MachineExplanation::Draw()
{
	// �ݒu���f�B�X�v���C�pUI
	m_spawnBox		->DrawUI();

	//�@====================[�@�}�V�����vUI�̕\���@]
	//�@�@|=>�@�ݒu���f�B�X�v���CUI�̃A�N�e�B�u��True�ł���Ƃ���None�ł��邽�߁A����ȊO�ł���Ε\�����s��
	if (!m_spawnBox->GetActiveFlag())
	{
		// LVUP�pUI
		m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

		// �C�U�pUI
		m_repairBox->DrawUI(SpriteLoder::REPAIR);

		// ��̗pUI
		m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);
	}

	m_nextMachineArrow->Draw();
	m_backMachineArrow->Draw();

}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(0.3f,0.3f, 0.3f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ���[���h���W�ϊ�
	SimpleMath::Vector3 worldPos = CalcScreenToXZN((int)m_data.pos.x,
												   (int)m_data.pos.y,
												   pDR->GetOutputSize().right,
												   pDR->GetOutputSize().bottom,
												   m_camera->GetViewMatrix(),
												   m_camera->GetProjectionMatrix());

	worldPos.z = 4.25f;

	modelData *= SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ���C�g
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, object->GetColor());
			lights->SetLightDiffuseColor(1, object->GetColor());
			lights->SetLightDiffuseColor(2, object->GetColor());
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), false,[&]
		{

			ModelShader::GetInstance().MachineDrawShader(object->GetColor(), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(object->GetElement()).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(object->GetElement()).GetAddressOf());

		});

	// �Z�J���h���f�������݂���Ȃ�Ύ��s
	if (secondModel != nullptr)
	{
		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}

}

bool MachineExplanation::OnMouse()
{
	return m_hitFlag;
}

void MachineExplanation::Finalize()
{
	m_camera.reset();
}

SelectionBox* MachineExplanation::GetMenuButton(int buttonType)
{
	//�@====================[�@�ԍ��ɑ������I���{�b�N�X��Ԃ��@]
	switch (buttonType)
	{
	case 0:
		//�@�@|=>�@�C�U
		return m_repairBox.get();

	case 1:
		//�@�@|=>�@�j��
		return m_dismantlingBox.get();

	case 2:
		//�@�@|=>�@����
		return m_selectLvUpBox.get();

	case 3:
		//�@�@|=>�@����
		return m_spawnBox.get();
	}

	// ����ȊO�̏ꍇ�͏C�U��I������
	return m_repairBox.get();

}

bool MachineExplanation::GetDismantlingFlag()
{
	return m_dismantlingBox->ClickMouse();
}


bool MachineExplanation::GetRepairFlag()
{
	return m_repairBox->ClickMouse();
}

bool MachineExplanation::GetLvUpFlag()
{
	return m_selectLvUpBox->ClickMouse();
}

bool MachineExplanation::GetSpawnFlag()
{
	return m_spawnBox->ClickMouse();
}

bool MachineExplanation::GetNextMachineFlag()
{
	return m_nextMachineArrow->ClickMouse();
}

bool MachineExplanation::GetBackMachineFlag()
{
	return m_backMachineArrow->ClickMouse();;
}
