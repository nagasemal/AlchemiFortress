#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/CommonStruct.h"

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{

	// �I���{�b�N�X�̃e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/log.png",
		nullptr,
		m_selectBoxTexture.ReleaseAndGetAddressOf()
	);

	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/elementTex.png",
		nullptr,
		m_elementTextures.ReleaseAndGetAddressOf()
	);

	// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MachineNames.png",
		nullptr,
		m_machineNameTextures.ReleaseAndGetAddressOf()
	);

	// �G�l�~�[�̖��O�̃e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/EnemyNames.png",
		nullptr,
		m_enemyNameTextures.ReleaseAndGetAddressOf()
	);

	// �����̕����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Seizou.png",
		nullptr,
		m_ManufacturingTexture.ReleaseAndGetAddressOf()
	);

	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/wo.png",
		nullptr,
		m_letterTexture.ReleaseAndGetAddressOf()
	);

	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionText.png",
		nullptr,
		m_missionTexture.ReleaseAndGetAddressOf()
	);

	// ���x���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionLabel.png",
		nullptr,
		m_missionLabelTexture.ReleaseAndGetAddressOf()
	);
	
	// �����e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/number_1.png",
		nullptr,
		m_NumberTexture.ReleaseAndGetAddressOf()
	);

	// �^�C�g�����S�e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/TitleLogo.png",
		nullptr,
		m_titleLogo.ReleaseAndGetAddressOf()
	);

	// UI�A�C�R���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/UIIcons.png",
		nullptr,
		m_uiIcons.ReleaseAndGetAddressOf()
	);

	// UI�A�C�R���e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Arrow.png",
		nullptr,
		m_uiArrow.ReleaseAndGetAddressOf()
	);

	// ���[���摜�e�N�X�`��
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Rule.png",
		nullptr,
		m_rule.ReleaseAndGetAddressOf()
	);

	LoadingPngFile(L"Resources/Textures/Rule/Right_Transition.png", m_transition[0]);
	LoadingPngFile(L"Resources/Textures/Rule/UP_Transition.png", m_transition[1]);

	LoadingPngFile(L"Resources/Textures/TitleText.png", m_pleaseMouseButtonText);

	// �}�V�����@�w
	{
		// ���@�w�e�N�X�`��_None
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/MajicCircle.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::NONE].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Attacker
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::ATTACKER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Defenser
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Defencer.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::DEFENSER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Upper
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Upper.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::UPPER].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Recovery
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::RECOVERY].ReleaseAndGetAddressOf()
		);

		// ���@�w�e�N�X�`��_Mining
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			L"Resources/Textures/MagicCircle/Attacker.png",
			nullptr,
			m_MagicCircleTexture[MACHINE_TYPE::MINING].ReleaseAndGetAddressOf()
		);
	}

	// �����p�e�N�X�`��
	{
		//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::NONE]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::ATTACKER]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Defencer.png", m_explanationTexture[INSTRUCTION_TYPE::DEFENSER]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Upper.png", m_explanationTexture[INSTRUCTION_TYPE::UPPER]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Recovery.png", m_explanationTexture[INSTRUCTION_TYPE::RECOVERY]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Excavator.png", m_explanationTexture[INSTRUCTION_TYPE::MINING]);
		//LoadingPngFile(L"Resources/Textures/Explanation/Attacker.png", m_explanationTexture[INSTRUCTION_TYPE::OPERATION]);
	}

}

void SpriteLoder::Finalize()
{
	// ���
	m_selectBoxTexture.Reset();
	m_elementTextures.Reset();
	m_machineNameTextures.Reset();
	m_ManufacturingTexture.Reset();
	m_NumberTexture.Reset();

	for (int i = 0; i < MACHINE_TYPE::NUM;i++)
	{
		m_MagicCircleTexture[i].Reset();
	}
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteLoder::LoadingPngFile
(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource)
{
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		filename,
		nullptr,
		spriteResource.ReleaseAndGetAddressOf()
	);

	return spriteResource;
}

SpriteLoder::SpriteLoder()
{

}

void SpriteLoder::Create()
{
	if (instance == nullptr)
	{
		instance = new SpriteLoder;
	}
}

void SpriteLoder::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}