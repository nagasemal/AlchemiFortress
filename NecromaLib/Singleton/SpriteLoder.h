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

	// �����A�C�R����n��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetElementTexture() { return m_elementTextures;}

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//�@�����A�C�R���̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

};