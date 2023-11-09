#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/CommonStruct.h"

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{

	// �I���{�b�N�X�̃e�N�X�`��
	LoadingPngFile(L"Resources/Textures/log.png", m_selectBoxTexture);

	// �B���p�I���{�^���̎擾
	LoadingPngFile(L"Resources/Textures/AlchemiButton.png",m_alchemiSelectTexture);

	// �B���p�I���{�b�N�X�J�o�[�̎擾
	LoadingPngFile(L"Resources/Textures/ButtonCover.png", m_alchemiCoverTexture);


	LoadingPngFile(L"Resources/Textures/SelectMachineUICursor.png", m_machineUICursorTexture);

	// �}�E�X�e�N�X�`���̎擾
	LoadingPngFile(L"Resources/Textures/Mouse.png", m_mouseTexture);

	// �}�V���A�C�R���擾
	LoadingPngFile(L"Resources/Textures/MachineIcons.png", m_machineIconTexture);
	

	LoadingPngFile(L"Resources/Textures/NormalMap.png", m_machineNormalMap[0]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Fire_NormalMap.png", m_machineNormalMap[1]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Ice_NormalMap.png", m_machineNormalMap[2]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Wind_NormalMap.png", m_machineNormalMap[3]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Earth_NormalMap.png", m_machineNormalMap[4]);

	LoadingPngFile(L"Resources/Textures/NormalMap.png", m_machineTexture[0]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Fire_Texture.png", m_machineTexture[1]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Ice_Texture.png", m_machineTexture[2]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Wind_Texture.png", m_machineTexture[3]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Earth_Texture.png", m_machineTexture[4]);

	LoadingPngFile(L"Resources/Textures/Effect/Aura.png", m_auraBaseTexture);
	LoadingPngFile(L"Resources/Textures/Effect/Flare.png", m_auraMaskTexture);

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

	// ���U���g�ɂĕ`�悷�镶���e�N�X�`��
	LoadingPngFile(L"Resources/Textures/Result_Text.png", m_resultTextTexture);

	LoadingPngFile(L"Resources/Textures/SuccessFailureText.png", m_stageClearTextTexture);

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

	LoadingPngFile(L"Resources/Textures/Explanation_Text.png", m_explanationTextTexture);

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
	LoadingPngFile(L"Resources/Textures/Rule/109.png", m_transition[2]);
	LoadingPngFile(L"Resources/Textures/Rule/156.png", m_transition[3]);

	LoadingPngFile(L"Resources/Textures/TitleText.png", m_pleaseMouseButtonText);

	// �}�V�����@�w
	{
		LoadingPngFile(L"Resources/Textures/MagicCircle/MajicCircle.png", m_MagicCircleTexture[MACHINE_TYPE::NONE]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/Attacker.png", m_MagicCircleTexture[MACHINE_TYPE::ATTACKER]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/Defencer.png", m_MagicCircleTexture[MACHINE_TYPE::DEFENSER]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/Upper.png", m_MagicCircleTexture[MACHINE_TYPE::UPPER]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/Attacker.png", m_MagicCircleTexture[MACHINE_TYPE::RECOVERY]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/Attacker.png", m_MagicCircleTexture[MACHINE_TYPE::MINING]);
		LoadingPngFile(L"Resources/Textures/MagicCircle/EnemySpawn.png", m_MagicCircleTexture[6]);
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