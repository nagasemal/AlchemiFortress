//--------------------------------------------------------------------------------------
// File: Particle.h
//
//�@�p�[�e�B�N���𔭐������܂�
// 
// Date: 2023.7.3
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "ParticleUtility.h"
#include <list>

class Particle
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	enum EFFECT_TYPE : int
	{
		HIT_BULLET		= 0,		// �G�l�~�[�ɋ�����������
		SPAWN_ENEMY		= 1,		// �G�l�~�[���o������
		DELETE_ENEMY	= 2,		// �G�l�~�[���|�ꂽ
		MACHINE_LVUP	= 3,		// �}�V���̃��x�����オ����
		MACHINE_BREAK	= 4,		// �}�V�����󂳂ꂽ

	};

	//�֐�
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Particle(EFFECT_TYPE type);
	~Particle();

	void LoadTexture(const wchar_t* path);
	void CreateBillboard();

	void Initialize();

	// �펞�p
	void Update(DirectX::SimpleMath::Vector3 pos);
	// �P���p
	bool OnShot(DirectX::SimpleMath::Vector3 pos, bool flag, DirectX::SimpleMath::Color color = {1.0f,1.0f,1.0f,1.0f});

	// �p�[�e�B�N���𓮂���/��������
	void UpdateParticle();

	void Render();

	//�ϐ�
private:

	// �G�t�F�N�g�̎��
	EFFECT_TYPE m_effectType;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// ���_�z��
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	// �p�[�e�B�N���z��
	std::list<ParticleUtility> m_particleUtility;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::SimpleMath::Matrix m_billboard;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// ��x�ɏo��Particle�̗�
	int m_particleNum;

	// �p�[�e�B�N���̐����Ԋu
	float m_particleSpawnTime;

	bool m_colFlag;

	float m_timer;

private:

	void CreateShader();

	ParticleUtility CreateEffectParam(EFFECT_TYPE type, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Color color = {1.0f,1.0f,1.0f,1.0f});

};