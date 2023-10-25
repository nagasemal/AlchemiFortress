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
	// オブジェクトの後ろにモデルが入ったら描画されるステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilShadow() { return m_depthStencilState_Shadow; }

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilNomal() { return m_depthStencilState_Nomal; }

	// モデルのシルエット用
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelShadowShader() { return m_modelShadowShader; }
	// モデルの半透明描画用
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelTransparentShader() { return m_modelTransparentShader; }
	// モデルの凍結描画用
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelEffectShader() { return m_modelEffect_PS; }

	// モデル描画のシェーダー (テスト)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelMyShader_PS() { return m_modelMyShader_PS; }
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetModelMyShader_VS() { return m_modelMyShader_VS; }

	DirectX::Model* GetMagicTrauabgukarPyram() { return m_magicTrauabgukarPyram.get(); }


	// モデル描画(マシン)のシェーダー設定
	void ModelDrawShader(SimpleMath::Color color,SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// モデルを用いたエフェクトのシェーダー設定
	void ModelEffectShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// シルエット描画用設定
	void SilhouetteShader();

	// 透明描画用
	void ToransparentShader();

private:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Base;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Nomal;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelShadowShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelTransparentShader;

	// 時間経過を伝える追加のコンスタントバッファ
	struct ConstBuffer
	{
		SimpleMath::Vector4 Time;			// 値遷移用時間変数
		SimpleMath::Color PaintColor;		// 追加の色情報
		SimpleMath::Color LimLightColor;	// リムライトのカラー
		SimpleMath::Vector4 eyes;			// 注視点(4の倍数しか受け取れない為、Vector4とする)
	};

	// マシンモデルに使用するシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelMyShader_PS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_modelMyShader_VS;

	// モデルを用いたエフェクトに用いるシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelEffect_PS;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 受け渡し用のコンスタントバッファ
	D3D11_BUFFER_DESC m_constBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbuffer;

	// エフェクト用のモデル
	std::unique_ptr<DirectX::Model> m_magicTrauabgukarPyram;

};