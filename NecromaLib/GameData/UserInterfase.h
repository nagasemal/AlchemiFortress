//--------------------------------------------------------------------------------------
// File: UserInterface.h
//
// UIをシェーダーを用いて表示するためのクラス
// 
// Date: 2023.5.20
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#include "PrimitiveBatch.h"
#include "DeviceResources.h"

#pragma once

class UserInterface
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
	};

	//変数
private:
	DX::DeviceResources* m_pDR;

	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;

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
	float m_value;	// 明暗


	// 表示位置
	ANCHOR m_anchor;

	// ratio = 比率,割合
	float m_renderRatio;

	//関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UserInterface();
	~UserInterface();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor);

	void Render();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(DirectX::SimpleMath::Vector2 scale);
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
	DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position);
	DirectX::SimpleMath::Vector2 GetPosition() { return m_position; }
	void SetAnchor(ANCHOR anchor);
	ANCHOR GetAnchor() { return m_anchor; }

	int GetTexture_W() { return m_textureWidth;}
	int GetTexture_H() { return m_textureHeight; }

	// 割合
	void SetRenderRatio(float ratio) { m_renderRatio = ratio; }
	float GetRenderRatio() { return m_renderRatio; }

	void SetValue(float value) { m_value = value;}
	float GetValue() { return m_value;}

	void SetColor(DirectX::SimpleMath::Color color) { m_color = color;}
	DirectX::SimpleMath::Color GetColor()			{ return m_color;}

private:

	void CreateShader();

};