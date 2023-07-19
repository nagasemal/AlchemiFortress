#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include <WICTextureLoader.h>

#define MAX_RAGE DirectX::SimpleMath::Vector2(450,300)

#define MINI_BOX_POS DirectX::SimpleMath::Vector2(-85,-95)

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

	m_data.pos  = { 180,550};
	m_data.rage = { 300,300};

	m_machineGauge = std::make_unique<MachineGauge>();
	m_machineGauge->AddHPGauge({m_data.pos.x + 50,m_data.pos.y - 110}, { 0.20,0.20 }, UserInterface::MIDDLE_CENTER);

}

void MachineExplanation::Update()
{
	InputSupport& pIS = InputSupport::GetInstance();
	DeltaTime& deltaTime = DeltaTime::GetInstance();

	m_moveTime += deltaTime.GetDeltaTime();

	m_hitFlag = HitObject(pIS.GetMousePosScreen());

}

void MachineExplanation::Update_MachineData(AlchemicalMachineObject* object)
{

	m_machineGauge->Update(object->GetHP(),object->GetMAXHP());

}

void MachineExplanation::Draw()
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = {0, 0, 64, 64 };

	// ���O�̐F
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	// BOX�`��
	pSB->Draw(m_texture.Get(),m_data.pos,&srcRect,colour,0.0f,XMFLOAT2(64 / 2, 64 / 2),5.0f);

	DirectX::SimpleMath::Vector2 miniBox_pos = { m_data.pos.x + MINI_BOX_POS.x ,m_data.pos.y + MINI_BOX_POS.y };

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(m_texture.Get(), miniBox_pos, &srcRect, colour, 0.0f, XMFLOAT2(64 / 2,64 / 2), 1.5f);

	pSB->End();

	// HP�Q�[�W
	m_machineGauge->Render();
}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel, AlchemicalMachineObject* object)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f,0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ���[���h���W�ϊ�
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN((int)m_data.pos.x - 78,
															(int)m_data.pos.y - 64,
															pDR->GetOutputSize().right,
															pDR->GetOutputSize().bottom,
															m_camera->GetViewMatrix(),
															m_camera->GetProjectionMatrix());

	worldPos.z = 2.0f;

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ���C�g
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, object->GetColor());
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());

	// �Z�J���h���f�������݂���Ȃ�Ύ��s
	if (secondModel != nullptr)
	{

		secondModel->UpdateEffects([&](IEffect* effect)
			{
				// ����̓��C�g�����~����
				auto lights = dynamic_cast<IEffectLights*>(effect);

				// �F�ύX
				lights->SetLightDiffuseColor(0, DirectX::SimpleMath::Color((float)object->GetPowerUpFlag(),(float)object->GetPowerUpFlag(), 0.0f, 1.0f));
			});

		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}

}

bool MachineExplanation::OnMouse()
{

	if (m_hitFlag)
	{
		int a = 0;
	}

	return m_hitFlag;
}

void MachineExplanation::Finalize()
{
	m_texture.Reset();
	m_camera.reset();
}
