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

	// UIアイコンの種類
	enum UIICON_TYPE : int
	{

		CLOCK = 0,
		EXPRANATION = 1,
		STOP = 2,
		START = 3,
		SOUND = 4,
		SE = 5,
		REPAIR = 6,
		LVUP = 7,
		MACHINE = 8,
		DISMATIONG = 9,

	};

	enum DIRECTION :int
	{

	};

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

	// マシンの名前を渡す
	// 横幅:128　縦幅:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMachineNameTexture() { return m_machineNameTextures; }
	const wchar_t* GetMachineNameTexturePath() { return L"Resources/Textures/MachineNames.png"; }

	// エネミーの名前を渡す
	// 横幅:128　縦幅:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetEnemyNameTexture() { return m_enemyNameTextures; }
	const wchar_t* GetEnemyNameTexturePath() { return L"Resources/Textures/EnemyNames.png"; }

	// 「を」を渡す
	// 横幅:28　縦幅:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetLetterTexture() { return m_letterTexture; }
	const wchar_t* GetLetterTexturePath() { return L"Resources/Textures/wo.png"; }

	// ミッション内容を渡す
	// 横幅:112　縦幅:28
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMissionTexture() { return m_missionTexture; }
	const wchar_t* GetMissionTexturePath() { return L"Resources/Textures/MissionText.png"; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMissionLabelTexture() { return m_missionLabelTexture; }
	const wchar_t* GetMissionLabelTexturePath() { return L"Resources/Textures/MissionLabel.png"; }

	// 「製造」文字アイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetManufacturingTexture() { return m_ManufacturingTexture; }
	const wchar_t* GetManufacturingTexturePath() { return L"Resources/Textures/Seizou.png"; }

	// デザイン数字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

	// 魔法陣テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicCircleTexture(int number) { return m_MagicCircleTexture[number]; }
	const wchar_t* GetMagicCircleTexturePath() { return L"Resources/Textures/MagicCircle/MajicCircle.png"; }

	// タイトルロゴテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTitleLogo() { return m_titleLogo; }
	const wchar_t* GetTitleLogoPath() { return L"Resources/Textures/TitleLogo.png"; }

	// UIアイコンテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIIcons() { return m_uiIcons; }
	const wchar_t* GetUIIconsPath() { return L"Resources/Textures/UIIcons.png"; }

	// 矢印アイコンテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIArrow() { return m_uiArrow; }
	const wchar_t* GetUIArrowPath() { return L"Resources/Textures/Arrow.png"; }

	// ルール画像を渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRule() { return m_rule; }
	const wchar_t* GetRulePath() { return L"Resources/Textures/MagicRule.png"; }

	// 0~3
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicRule(int index) { return m_transition[index]; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetPleaseMouseButton() { return m_pleaseMouseButtonText; }

	//// 説明用画像を渡す
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetExplanationTexture(int type) { return m_explanationTexture[type]; }

	// 画像を読み込む
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadingPngFile(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource);

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//　選択ボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	//　属性アイコンのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

	//　マシンの名前のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineNameTextures;

	// エネミーの名前テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyNameTextures;

	// 製造の文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;

	// をテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_letterTexture;

	// ミッション文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionTexture;

	// ミッションのラベルテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionLabelTexture;

	// デザイン数字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTexture;

	// 魔法陣のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_MagicCircleTexture[6];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleLogo;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiIcons;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiArrow;

	// ルール画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rule;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_transition[4];

	// PleaseMouseButtonのテキスト画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pleaseMouseButtonText;

};