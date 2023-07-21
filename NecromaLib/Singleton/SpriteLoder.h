//--------------------------------------------------------------------------------------
// File: SpriteLoder.h
//
// スプライト情報をmap配列で保存するシングルトンクラス
// 繰り返し使うものや、シーンをまたいで使うものは定義して使いやすくしておく
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

	// 読み込み
	void Loading();

	// 解放
	void Finalize();

	// 選択ボックスのテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSelectBoxTexture() { return m_selectBoxTexture; }
	const wchar_t* GetSelectBoxTexturePath() { return L"Resources/Textures/log.png"; }

	// 属性アイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetElementTexture() { return m_elementTextures;}
	const wchar_t* GetElementTexturePath() { return L"Resources/Textures/elementTex.png";}

	// 「製造」文字アイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetManufacturingTexture() { return m_ManufacturingTexture; }
	const wchar_t* GetManufacturingTexturePath() { return L"Resources/Textures/Seizou.png"; }

	// デザイン数字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

	// 魔法陣テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicCircleTexture(int number) { return m_MagicCircleTexture[number]; }
	const wchar_t* GetMagicCircleTexturePath() { return L"Resources/Textures/MajicCircle.png"; }

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//　選択ボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	//　属性アイコンのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

	// 製造の文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;

	// デザイン数字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTexture;

	// 魔法陣のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_MagicCircleTexture[6];

};