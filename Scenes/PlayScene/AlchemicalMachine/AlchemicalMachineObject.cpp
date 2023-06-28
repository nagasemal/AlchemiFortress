#include "pch.h"
#include "AlchemicalMachineObject.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"


#define AM_RAGE DirectX::SimpleMath::Vector3(1, 1, 1)


AlchemicalMachineObject::AlchemicalMachineObject():
	m_hp(),
	m_active(),
	m_hitMouseFlag(),
	m_objectName(),
	m_magicCircle(),
	m_machineID(MACHINE_TYPE::NONE),
	m_lv(1),
	m_machineEffectNum(),
	m_span(),
	m_color(1,1,1,1),
	m_rotateAnimation(),
	m_element(MACHINE_ELEMENT::NOMAL),
	m_powerUPFlag()
{
}

void AlchemicalMachineObject::HitToObject(MousePointer* pMP)
{

	m_hitMouseFlag = false;

	InputSupport& pINP = InputSupport::GetInstance();
	Circle mouseWolrdPos = Circle();
	mouseWolrdPos.p = InputSupport::GetInstance().GetMousePosWolrd();
	mouseWolrdPos.r = (pMP->GetRage().x + pMP->GetRage().z) / 2;

	// �I�u�W�F�N�g�ƃ}�E�X�|�C���^�[�̓����蔻��
	if (CircleCollider(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

}

bool AlchemicalMachineObject::OnCollisionEnter_MagicCircle(GameObjct3D* object)
{
	return 	CircleCollider(m_magicCircle, object->GetCircle());
}

void AlchemicalMachineObject::ModelRender(DirectX::Model* model, DirectX::Model* ring)
{
	ShareData& pSD = ShareData::GetInstance();


	m_rotateAnimation += DeltaTime::GetInstance().GetDeltaTime();

	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);

	// �f�B�t�F���T�[�^�͏�ɋ��_�̕���������
	if (m_machineID == DEFENSER)
	{
		m_rotateAnimation = 0.0f;
		modelData *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(LookAt({0,0,0}));
	}

	// ��ɉE���ɉ�]
	modelData *= DirectX::SimpleMath::Matrix::CreateRotationY(m_rotateAnimation);
	// ��ɏc�ɗh���
	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f), m_data.pos.z);

	// �G�t�F�N�g�̐ݒ�
	model->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// �F�ύX
			lights->SetLightDiffuseColor(0, m_color);
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	// �ǉ��p�[�c�����݂���ꍇ
	if (ring != nullptr)
	{
		DirectX::SimpleMath::Matrix ringData = DirectX::SimpleMath::Matrix::Identity;
		ringData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);
		ringData *= DirectX::SimpleMath::Matrix::CreateRotationY(-m_rotateAnimation * 1.5f);
		ringData *= DirectX::SimpleMath::Matrix::CreateTranslation
		(m_data.pos.x,
		 m_data.pos.y + (sinf(m_rotateAnimation) * 0.5f),
		 m_data.pos.z);

		// �G�t�F�N�g�̐ݒ�
		ring->UpdateEffects([&](IEffect* effect)
		{
			// ����̓��C�g�����~����
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0,DirectX::SimpleMath::Color((float)m_powerUPFlag,(float)m_powerUPFlag,0.0f,1.0f));
		});

		ring->Draw(pSD.GetContext(), *pSD.GetCommonStates(), ringData, pSD.GetView(), pSD.GetProjection());

	}


}

void AlchemicalMachineObject::SummonAM(DirectX::SimpleMath::Vector3 pos)
{
	m_data.rage = AM_RAGE;
	m_data.pos = pos;
	m_active = true;

}
