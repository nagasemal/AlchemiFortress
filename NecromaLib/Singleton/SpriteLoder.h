//--------------------------------------------------------------------------------------
// File: SpriteLoder.h
//
// �X�v���C�g����map�z��ŕۑ�����V���O���g���N���X
// �J��Ԃ��g�����̂�A�V�[�����܂����Ŏg�����̂͒�`���Ďg���₷�����Ă���
// 
// Date: 2023.6.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include <map>

class SpriteLoder
{
public:

	// UI�A�C�R���̎��
	enum UIICON_TYPE : int
	{

		CLOCK = 0,
		EXPRANATION = 1,
		STOP = 2,
		START = 3,
		SOUND = 4,
		SE = 5,
		REPAIR = 6,
		LVUP = 7,
		MACHINE = 8,
		DISMATIONG = 9,

	};

	enum DIRECTION :int
	{

	};

public:
	~SpriteLoder() = default;

	SpriteLoder(const SpriteLoder&) = delete;
	SpriteLoder& operator=(const SpriteLoder&) = delete;

	static void Create();
	static void Destroy();

	static SpriteLoder& GetInstance() { return *instance; }

	// �ǂݍ���
	void Loading();

	// ���
	void Finalize();

	// �I���{�b�N�X�̃e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSelectBoxTexture() { return m_selectBoxTexture; }
	const wchar_t* GetSelectBoxTexturePath() { return L"Resources/Textures/log.png"; }

	// �����A�C�R����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetElementTexture() { return m_elementTextures;}
	const wchar_t* GetElementTexturePath() { return L"Resources/Textures/elementTex.png";}

	// �}�V���̖��O��n��
	// ����:128�@�c��:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMachineNameTexture() { return m_machineNameTextures; }
	const wchar_t* GetMachineNameTexturePath() { return L"Resources/Textures/MachineNames.png"; }

	// �G�l�~�[�̖��O��n��
	// ����:128�@�c��:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetEnemyNameTexture() { return m_enemyNameTextures; }
	const wchar_t* GetEnemyNameTexturePath() { return L"Resources/Textures/EnemyNames.png"; }

	// �u���v��n��
	// ����:28�@�c��:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetLetterTexture() { return m_letterTexture; }
	const wchar_t* GetLetterTexturePath() { return L"Resources/Textures/wo.png"; }

	// �~�b�V�������e��n��
	// ����:112�@�c��:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMissionTexture() { return m_missionTexture; }
	const wchar_t* GetMissionTexturePath() { return L"Resources/Textures/MissionText.png"; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMissionLabelTexture() { return m_missionLabelTexture; }
	const wchar_t* GetMissionLabelTexturePath() { return L"Resources/Textures/MissionLabel.png"; }

	// �u�����v�����A�C�R����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetManufacturingTexture() { return m_ManufacturingTexture; }
	const wchar_t* GetManufacturingTexturePath() { return L"Resources/Textures/Seizou.png"; }

	// �f�U�C�������e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

	// ���@�w�e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicCircleTexture(int number) { return m_MagicCircleTexture[number]; }
	const wchar_t* GetMagicCircleTexturePath() { return L"Resources/Textures/MagicCircle/MajicCircle.png"; }

	// �^�C�g�����S�e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTitleLogo() { return m_titleLogo; }
	const wchar_t* GetTitleLogoPath() { return L"Resources/Textures/TitleLogo.png"; }

	// UI�A�C�R���e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIIcons() { return m_uiIcons; }
	const wchar_t* GetUIIconsPath() { return L"Resources/Textures/UIIcons.png"; }

	// ���A�C�R���e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIArrow() { return m_uiArrow; }
	const wchar_t* GetUIArrowPath() { return L"Resources/Textures/Arrow.png"; }

	// ���[���摜��n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRule() { return m_rule; }
	const wchar_t* GetRulePath() { return L"Resources/Textures/MagicRule.png"; }

	// 0~3
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicRule(int index) { return m_transition[index]; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetPleaseMouseButton() { return m_pleaseMouseButtonText; }

	//// �����p�摜��n��
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetExplanationTexture(int type) { return m_explanationTexture[type]; }

	// �摜��ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadingPngFile(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource);

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//�@�I���{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	//�@�����A�C�R���̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

	//�@�}�V���̖��O�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineNameTextures;

	// �G�l�~�[�̖��O�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyNameTextures;

	// �����̕����e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;

	// ���e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_letterTexture;

	// �~�b�V���������e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionTexture;

	// �~�b�V�����̃��x���e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionLabelTexture;

	// �f�U�C�������e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTexture;

	// ���@�w�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_MagicCircleTexture[6];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleLogo;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiIcons;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiArrow;

	// ���[���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rule;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_transition[4];

	// PleaseMouseButton�̃e�L�X�g�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pleaseMouseButtonText;

};