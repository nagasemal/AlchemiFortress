#include "pch.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"


#define IMAGE_WIGHT			64
#define IMAGE_HEIGHT		64

#define BOX_DISTANS_X		68
#define BOX_DISTANS_Y		80

MachineSelect::MachineSelect():
	m_hitMouseFlag(),
	m_onMouseFlag(),
	m_manufacturingFlag(),
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_changeColorFlag(),
	m_boxColor{1.0f,1.0f,1.0f,1.0f},
	m_colorChangeTime()
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{
	m_data.rage = { 1.0f,1.0f };

	m_machineBox = std::make_unique<SelectionBox>(m_data.pos, m_data.rage);
	m_machineBox->Initialize();

	// �I���\�{�b�N�X
	for (int i = 0; i < 3; i++)
	{
		m_selectionBox[i] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2((m_data.pos.x - BOX_DISTANS_X) + (i * BOX_DISTANS_X),m_data.pos.y + BOX_DISTANS_Y),
														   DirectX::SimpleMath::Vector2(1.0f, 1.0f));
		m_selectionBox[i]->Initialize();
	}

	// �����{�^��
	m_selectionManufacturing = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(m_data.pos.x, m_data.pos.y + 150),
							   DirectX::SimpleMath::Vector2(3.0f,1.0f));

	m_selectionManufacturing->Initialize();

	m_colorChangeTime = 0;

}

void MachineSelect::Update()
{
	InputSupport&	pIS = InputSupport::GetInstance();
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// �F��ς���
	m_colorChangeTime += deltaTime * 5.0f;
	m_boxColor.G(0.5f + cosf(m_colorChangeTime) / 2);

	DirectX::SimpleMath::Vector2 mousePos = pIS.GetMousePosScreen();
	bool leftFlag = pIS.GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED;
	bool onMouseFlag = HitObject(mousePos);
	leftFlag;
	onMouseFlag;

	m_onMouseFlag = m_machineBox->HitMouse();
	m_hitMouseFlag = m_machineBox->SelectionMouse();

	// �I������Ă���Ȃ�΁A�S�̂̑��x�𗎂Ƃ�
	if (m_hitMouseFlag)
	{
		DeltaTime::GetInstance().SetDeltaTime(deltaTime / 2);

		m_selectionManufacturing->HitMouse();
		m_manufacturingFlag = m_selectionManufacturing->ClickMouse();

	}

}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
	//for (int i = 0; i < 3; i++)
	//{
	//	m_selectionBox[i]->Finalize();
	//}
}

void MachineSelect::DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
								  DirectX::Model* model, DirectX::SimpleMath::Matrix view,
								  DirectX::SimpleMath::Matrix proj,
								  DirectX::Model* secondModel)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ���O�̐F
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if (m_changeColorFlag) colour = m_boxColor;
	if(m_onMouseFlag) colour = DirectX::SimpleMath::Color(0.9f, 0.95f, 0.8f, 0.9f);
	if(m_hitMouseFlag) colour = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.85f, 0.98f);

	DirectX::SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y};

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(IMAGE_WIGHT / 2, IMAGE_HEIGHT / 2), 1.5f);

	pSB->End();

	// �K�v�f�ނ�\������
	if (m_hitMouseFlag)
	{

		RECT rect[3] = {SpriteCutter(IMAGE_WIGHT, IMAGE_HEIGHT, 0, 0),						// �K�v���͗�
						SpriteCutter(IMAGE_WIGHT, IMAGE_HEIGHT, 1, 0),						// �K�v������
						SpriteCutter(0, 0, m_selectMachineType, 1)};	// �K�v���@

		for (int i = 0; i < 3; i++)
		{
			m_selectionBox[i]->DrawUI(texture.Get(),pSL.GetElementTexture(),rect[i]);
		}

		m_selectionManufacturing->DrawUI(texture.Get(), pSL.GetManufacturingTexture());
	}

	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f, 0.35f, 0.35f);

	// �p�x����
	modelData *= SimpleMath::Matrix::CreateRotationX(-20);

	// ���[���h���W�ϊ�
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN(
		(int)m_data.pos.x,
		(int)m_data.pos.y + 30,
		(int)pDR->GetOutputSize().right,
		(int)pDR->GetOutputSize().bottom,
		view,
		proj);

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(worldPos);

	model->UpdateEffects([](IEffect* effect)
		{
			// ���C�g
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// �F�ύX
			lights->SetLightDiffuseColor(0, Colors::White);

		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, view, proj);

	// �Z�J���h���f�������݂���̂Ȃ�Ύ��s
	if (secondModel != nullptr)
	{
		secondModel->UpdateEffects([&](IEffect* effect)
			{
				// ����̓��C�g�����~����
				auto lights = dynamic_cast<IEffectLights*>(effect);

				// �F�ύX
				lights->SetLightDiffuseColor(0, DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
			});

		secondModel->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, view, proj);
	}
}

