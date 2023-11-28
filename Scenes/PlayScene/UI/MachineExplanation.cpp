#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"


#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#define MAX_RAGE SimpleMath::Vector2(450,300)

#define MINI_BOX_POS SimpleMath::Vector2(-85,-95)

#define BIG_BOX_RAGEPERCENT SimpleMath::Vector2(5.0f,0.5f)


// �A�C�R����ݒu����ӏ�
#define ICON_POS SimpleMath::Vector2(100,600)

// �A�C�R������ׂ�Ԋu
#define ICON_OFFSET_X 60.0f

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
	m_moveTime = 0;

	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Log.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	m_camera = std::make_unique<Camera>();

	m_data.pos  = { 150,680};
	m_data.rage = { 150,150};

	//m_machineGauge = std::make_unique<MachineGauge>();
	//m_machineGauge->AddHPGauge({m_data.pos.x + 60,m_data.pos.y - 130}, { 0.20,0.20 }, UserInterface::MIDDLE_CENTER);

	m_selectLvUpBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_repairBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X * 2, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

	m_dismantlingBox = std::make_unique<SelectionBox>(SimpleMath::Vector2(ICON_POS.x + ICON_OFFSET_X * 3, ICON_POS.y),
		SimpleMath::Vector2(0.8f, 0.8f));

}

void MachineExplanation::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	m_moveTime += deltaTime.GetDeltaTime();

	m_hitFlag = HitObject_RageSet(pINP.GetMousePosScreen(),64,64, BIG_BOX_RAGEPERCENT);






}

void MachineExplanation::Update_MachineData(AlchemicalMachineObject* object)
{

	DataManager& pDataM = *DataManager::GetInstance();

	// LvUp�p�̑I���{�b�N�X�̐ݒ�
	m_selectLvUpBox->HitMouse();

	// �N���X�^�������炷
	DataManager& pDM = *DataManager::GetInstance();

	// Lv������܂��͕ύX��̃N���X�^����0�ȉ�
	m_selectLvUpBox->SetActiveFlag(object->GetLv() <= 5 && pDM.GetNowCrystal() - object->GetNextLvCrystal() >= 0);

	if (m_selectLvUpBox->ClickMouse()) 		object->LvUp();

	// �C�U�p�̑I���{�b�N�X�̐ݒ�
	m_repairBox->HitMouse();
	m_repairBox->SetActiveFlag(pDataM.GetNowCrystal() - object->GetRepairCrystal() >= 0 && object->GetHP() < object->GetMAXHP());

	// �C�U�I���{�b�N�X�������@���݂�Crystal�ʂ���C�U�Ɋ|����Crystal�ʂ�0�ȏ�Ȃ�Ύ��s
	if (m_repairBox->ClickMouse())
	{
		object->SetHP(object->GetMAXHP());
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() - object->GetRepairCrystal());
	}

	// �j��p�̑I���{�b�N�X
	m_dismantlingBox->HitMouse();
	m_dismantlingBox->SetActiveFlag(pDataM.GetNowCrystal() + object->GetDismantlingCrystal() <= pDataM.GetNowCrystal_MAX());

	// �j��I���{�b�N�X�������@���݂�Crystal�ʂ��瑝������Crystal�ʂ��ő�l�ȉ��Ȃ�Ύ��s
	if (m_dismantlingBox->ClickMouse())
	{
		pDataM.SetNowCrystal(pDataM.GetNowCrystal() + object->GetDismantlingCrystal());
	}


}

void MachineExplanation::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = {0, 0, 64, 64 };

	// ���O�̐F
	SimpleMath::Color colour = SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	// BOX�`��

	SimpleMath::Vector2 miniBox_pos = { m_data.pos.x + MINI_BOX_POS.x ,m_data.pos.y + MINI_BOX_POS.y };

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(m_texture.Get(), miniBox_pos, &srcRect, colour, 0.0f, XMFLOAT2(64 / 2,64 / 2), 1.5f);

	pSB->End();

	// LVUP�pUI
	m_selectLvUpBox->DrawUI(SpriteLoder::LVUP);

	// �C�U�pUI
	m_repairBox->DrawUI(SpriteLoder::REPAIR);

	// ��̗pUI
	m_dismantlingBox->DrawUI(SpriteLoder::DISMATIONG);
}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(0.35f,0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ���[���h���W�ϊ�
	SimpleMath::Vector3 worldPos = CalcScreenToXZN((int)m_data.pos.x - 86,
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
	m_texture.Reset();
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
