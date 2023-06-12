#include "pch.h"
#include "MachineExplanation.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/GameData/Camera.h"
#include <WICTextureLoader.h>

#define MAX_RAGE DirectX::SimpleMath::Vector2(450,300)

#define MINI_BOX_POS DirectX::SimpleMath::Vector2(20,20)

MachineExplanation::MachineExplanation():
	m_moveTime(),
	m_modelPos()
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

	m_data.pos  = { 60,350};
	m_data.rage = { 5,5};
}

void MachineExplanation::Update()
{

	DeltaTime& deltaTime = DeltaTime::GetInstance();

	m_moveTime += deltaTime.GetDeltaTime();

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
	pSB->Draw(m_texture.Get(),m_data.pos,&srcRect,colour,0.0f,XMFLOAT2(),m_data.rage.x);

	DirectX::SimpleMath::Vector2 miniBox_pos = { m_data.pos.x + MINI_BOX_POS.x ,m_data.pos.y + MINI_BOX_POS.y };

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(m_texture.Get(), miniBox_pos, &srcRect, colour, 0.0f, XMFLOAT2(), m_data.rage.x / 3.f);

	pSB->End();
}

void MachineExplanation::DisplayObject(DirectX::Model* model, DirectX::Model* secondModel,DirectX::SimpleMath::Color color)
{

	ShareData& pSD = ShareData::GetInstance();
	DX::DeviceResources* pDR = pSD.GetDeviceResources();

	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f,0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);
	modelData *= SimpleMath::Matrix::CreateRotationZ(m_moveTime);

	// ���[���h���W�ϊ�
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN(m_data.pos.x + 72,
															m_data.pos.y + 72,
															pDR->GetOutputSize().right,
															pDR->GetOutputSize().bottom,
															m_camera->GetViewMatrix(),
															m_camera->GetProjectionMatrix());

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([&](IEffect* effect)
		{
			// ���C�g
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, color);
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());

	// �Z�J���h���f�������݂���Ȃ�Ύ��s
	if (secondModel != nullptr)
	{
		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}

}

void MachineExplanation::Finalize()
{
	m_texture.Reset();
	m_camera.reset();
}
