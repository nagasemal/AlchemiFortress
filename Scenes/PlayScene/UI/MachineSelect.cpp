#include "pch.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"


#define IMAGE_WIGHT		 64
#define IMAGE_HEIGHT	 64

#define BOX_DISTANS_X 68
#define BOX_DISTANS_Y 80

MachineSelect::MachineSelect():
	m_hitMouseFlag(),
	m_selectMachineType(AlchemicalMachineObject::MACHINE_TYPE::NONE)
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{
	m_data.rage = { 64,64 };

	for (int i = 0; i < 3; i++)
	{
		m_selectionBox[i] = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2((m_data.pos.x - BOX_DISTANS_X) + (i * BOX_DISTANS_X),m_data.pos.y + BOX_DISTANS_Y),
														   DirectX::SimpleMath::Vector2(1, 1));

		m_selectionBox[i]->Initialize();
	}
}

void MachineSelect::Update()
{
	InputSupport&	pIS = InputSupport::GetInstance();

	bool leftFlag = pIS.GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED;
	bool onMouseFlag = HitObject(pIS.GetMousePosScreen());

	// ���������N���b�N��������� �t�B�[���h��ł���ꍇ�͂��̌���ł͂Ȃ�
	if (leftFlag)
	{
		m_hitMouseFlag = false;
	}

	// �Ώۂ��N���b�N������True�ɂ���
	if (onMouseFlag && leftFlag)
	{
		m_hitMouseFlag = true;
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

void MachineSelect::DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::Model* model, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::Model* secondModel)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	SpriteLoder& pSL = SpriteLoder::GetInstance();


	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ���O�̐F
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if(m_hitMouseFlag) colour = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.8f, 0.9f);

	DirectX::SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y};

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(IMAGE_WIGHT / 2, IMAGE_HEIGHT / 2), 1.5f);

	pSB->End();

	// �K�v�f�ނ�\������
	if (m_hitMouseFlag)
	{

		RECT rect[3] = {SpriteCutter(64, 64, 0, 0), // �K�v���͗�
						SpriteCutter(64, 64, 1, 0), // �K�v������
						SpriteCutter(64, 64, m_selectMachineType, 1)}; // �K�v���@

		for (int i = 0; i < 3; i++)
		{
			m_selectionBox[i]->DrawUI(texture.Get(),pSL.GetElementTexture(),rect[i]);
		}
	}


	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f, 0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);

	// ���[���h���W�Ԋ�
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN(
		(int)m_data.pos.x,
		(int)m_data.pos.y,
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

