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

public:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilBase() { return m_depthStencilState_Base; }
	// �I�u�W�F�N�g�̌��Ƀ��f������������`�悳���X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilShadow() { return m_depthStencilState_Shadow; }

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilNomal() { return m_depthStencilState_Nomal; }

	// ���f���̃V���G�b�g�p
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelShadowShader() { return m_modelShadowShader; }
	// ���f���̔������`��p
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelTransparentShader() { return m_modelTransparentShader; }

	// ���f���`��̃V�F�[�_�[ (�e�X�g)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelMyShader_PS() { return m_modelMyShader_PS; }
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetModelMyShader_VS() { return m_modelMyShader_VS; }

	// ���f���`��(�}�V��)�̃V�F�[�_�[�ݒ�
	void ModelDrawShader(SimpleMath::Color color,SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// �V���G�b�g�`��p�ݒ�
	void SilhouetteShader();

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
		SimpleMath::Vector4 Time;
		SimpleMath::Color PaintColor;
	};

	// �}�V�����f���Ɏg�p����V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelMyShader_PS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_modelMyShader_VS;
	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �󂯓n���p�̃R���X�^���g�o�b�t�@
	D3D11_BUFFER_DESC m_constBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbuffer;

};