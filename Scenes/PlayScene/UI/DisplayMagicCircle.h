#pragma once
#include "PrimitiveBatch.h"

class DisplayMagicCircle
{
public:

	//UIのアンカーポイント
	enum ANCHOR
	{
		TOP_LEFT = 0,
		TOP_CENTER,
		TOP_RIGHT,

		MIDDLE_LEFT,
		MIDDLE_CENTER,
		MIDDLE_RIGHT,

		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT

	};

public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
		DirectX::SimpleMath::Vector4	color;
		DirectX::SimpleMath::Matrix		rotationMatrix;
	};

public:
	DisplayMagicCircle();
	~DisplayMagicCircle();

	void LoadTexture(const wchar_t* path);

	void Create(const wchar_t* path);

	void Update();
	void Render();

	// 苦肉の策
	void SpritebatchRender();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(DirectX::SimpleMath::Vector2 scale);
	void SetPosition(DirectX::SimpleMath::Vector2 position);

	void SetColor(DirectX::SimpleMath::Color color) { m_color = color; }
	DirectX::SimpleMath::Color GetColor() { return m_color; }

	void SetRotationMatrix(DirectX::SimpleMath::Matrix matrix) { m_rotationMatrix = matrix;}

	// 魔法陣を出現させる
	void TransparentUpdate(bool spawnFlag);

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

private:

	// 魔法陣テクスチャ情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;

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

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Matrix	m_rotationMatrix;

	// 魔法陣を回す時間計測変数
	float m_rotateTime;

	// 魔法陣を透明→半透明に出現させる時間計測変数
	float m_transparentTime;
	float m_transparentDeltaTime;

private:

	void CreateShader();

};