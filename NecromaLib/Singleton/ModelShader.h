#pragma once
class ModelShader
{
private:

	ModelShader();
	static ModelShader* instance;

public:
	~ModelShader() = default;

	ModelShader(const ModelShader&) = delete;
	ModelShader& operator=(const ModelShader&) = delete;

	static void Create();
	static void Destroy();

	static ModelShader& GetInstance() { return *instance; }

	void CreateModelShader();

	void CreateEffectModel();

public:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilBase() { return m_depthStencilState_Base; }
	// �I�u�W�F�N�g�̌��Ƀ��f������������`�悳���X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilShadow() { return m_depthStencilState_Shadow; }

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilNomal() { return m_depthStencilState_Nomal; }

	// ���f���̃V���G�b�g�p
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelShadowShader() { return m_modelShadowShader; }
	// ���f���̔������`��p
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelTransparentShader() { return m_modelTransparentShader; }
	// ���f���̓����`��p
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelEffectShader() { return m_modelEffect_PS; }

	// ���f���`��̃V�F�[�_�[ (�e�X�g)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelMyShader_PS() { return m_modelMyShader_PS; }
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetModelMyShader_VS() { return m_modelMyShader_VS; }

	DirectX::Model* GetMagicTrauabgukarPyram() { return m_magicTrauabgukarPyram.get(); }


	// ���f���`��(�}�V��)�̃V�F�[�_�[�ݒ�
	void ModelDrawShader(SimpleMath::Color color,SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// ���f����p�����G�t�F�N�g�̃V�F�[�_�[�ݒ�
	void ModelEffectShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// �V���G�b�g�`��p�ݒ�
	void SilhouetteShader();

	// �����`��p
	void ToransparentShader();

private:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Base;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Nomal;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelShadowShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelTransparentShader;

	// ���Ԍo�߂�`����ǉ��̃R���X�^���g�o�b�t�@
	struct ConstBuffer
	{
		SimpleMath::Vector4 Time;			// �l�J�ڗp���ԕϐ�
		SimpleMath::Color PaintColor;		// �ǉ��̐F���
		SimpleMath::Color LimLightColor;	// �������C�g�̃J���[
		SimpleMath::Vector4 eyes;			// �����_(4�̔{�������󂯎��Ȃ��ׁAVector4�Ƃ���)
	};

	// �}�V�����f���Ɏg�p����V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelMyShader_PS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_modelMyShader_VS;

	// ���f����p�����G�t�F�N�g�ɗp����V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelEffect_PS;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �󂯓n���p�̃R���X�^���g�o�b�t�@
	D3D11_BUFFER_DESC m_constBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbuffer;

	// �G�t�F�N�g�p�̃��f��
	std::unique_ptr<DirectX::Model> m_magicTrauabgukarPyram;

};