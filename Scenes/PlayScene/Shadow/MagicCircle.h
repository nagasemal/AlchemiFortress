//--------------------------------------------------------------------------------------
// File: MagicCircle.h
//
//  シェーダー使用
//　アルケミカルマシンが選択中　/　プレイヤーベースのLv表示につかう魔法陣の処理
//  
// Date: 2023.7.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class MagicCircle
{
public:

	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		m_matWorld;
		DirectX::SimpleMath::Matrix		m_matView;
		DirectX::SimpleMath::Matrix		m_matProj;
		DirectX::SimpleMath::Vector4	m_diffuse;
	};

public:
	MagicCircle();
	~MagicCircle();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	void CreateShader();

	// vector配列に中身を入れる
	void CreateMagicCircle(DirectX::SimpleMath::Vector3 pos, float r, DirectX::SimpleMath::Color color = {1.0f,1.0f,1.0f,1.0f});

	void DeleteMagicCircle();

	void CreateWorld();

	void LoadTexture(const wchar_t* path);

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	float GetAnimationTime() { return m_animationTime; }

private:

private:

	// 魔法陣テクスチャ マシン用
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textures;

	// コンスタンスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 頂点情報
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	float m_animationTime;
};
