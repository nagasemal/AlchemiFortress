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

	// 属性アイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetElementTexture() { return m_elementTextures;}

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//　属性アイコンのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

};