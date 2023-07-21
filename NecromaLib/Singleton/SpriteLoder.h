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

	// �u�����v�����A�C�R����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetManufacturingTexture() { return m_ManufacturingTexture; }
	const wchar_t* GetManufacturingTexturePath() { return L"Resources/Textures/Seizou.png"; }

	// �f�U�C�������e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

	// ���@�w�e�N�X�`����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicCircleTexture(int number) { return m_MagicCircleTexture[number]; }
	const wchar_t* GetMagicCircleTexturePath() { return L"Resources/Textures/MajicCircle.png"; }

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//�@�I���{�b�N�X�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	//�@�����A�C�R���̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

	// �����̕����e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;

	// �f�U�C�������e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTexture;

	// ���@�w�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_MagicCircleTexture[6];

};