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
		SYSTEMSE = 6,
		REPAIR = 7,
		LVUP = 8,
		MACHINE = 9,
		DISMATIONG = 10,
		SPEED_1 = 11,
		SPEED_2 = 12,
		SPEED_3 = 13,
		SPEED_4 = 14,
		CANCEL = 15

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

	// 錬金用選択ボックスのテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAlchemiButtonTexture() { return m_alchemiSelectTexture; }
	const wchar_t* GetAlchemiButtonTexturePath() { return L"Resources/Textures/AlchemiButton.png"; }

	// 錬金用選択ボックスのカバーテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAlchemiButtonCoverTexture() { return m_alchemiCoverTexture; }

	// マシンUIのカーソルテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMachineUICursorTexture() { return m_machineUICursorTexture; }

	// マウスのテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMouseTexture() { return m_mouseTexture; }
	const wchar_t* GetMouseTexturePath() { return L"Resources/Textures/Mouse.png"; }

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

	// マシンアイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMachineIconTexture() { return m_machineIconTexture; }

	// ステージクリア、失敗を示すアイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetStageClearTextTexture() { return m_stageClearTextTexture; }
	const wchar_t* GetStageClearTextTexturePath() { return L"Resources/Textures/SuccessFailureText.png"; }

	// デザイン数字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

	// 魔法陣テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicCircleTexture(int number) { return m_MagicCircleTexture[number]; }
	const wchar_t* GetMagicCircleTexturePath() { return L"Resources/Textures/MagicCircle/MajicCircle.png"; }

	// タイトルロゴテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTitleLogo() { return m_titleLogo; }
	const wchar_t* GetTitleLogoPath() { return L"Resources/Textures/TitleLogo.png"; }

	// プレイシーンで描画する操作説明文字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetExplanationTextTexture() { return m_explanationTextTexture; }
	const wchar_t* GetExplanationTextTexturePath() { return L"Resources/Textures/Explanation_Text.png"; }

	// リザルトで描画する文字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResultTextTexture() { return m_resultTextTexture; }
	const wchar_t* GetResultTextTexturePath() { return L"Resources/Textures/Result_Text.png"; }

	// UIアイコンテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIIcons() { return m_uiIcons; }
	const wchar_t* GetUIIconsPath() { return L"Resources/Textures/UIIcons.png"; }

	// 矢印アイコンテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetUIArrow() { return m_uiArrow; }
	const wchar_t* GetUIArrowPath() { return L"Resources/Textures/Arrow.png"; }

	// ルール画像を渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRule() { return m_rule; }
	const wchar_t* GetRulePath() { return L"Resources/Textures/MagicRule.png"; }

	// マシンに使うノーマルマップの画像を渡します
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNormalMap(int element) { return m_machineNormalMap[element]; }
	const wchar_t* GetNormalMapPath() { return L"Resources/Textures/NormalMap.png"; }

	// マシンに使うテクスチャの画像を渡します
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMachineTextuer(int element) { return m_machineTexture[element]; }

	// 0~3
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicRule(int index) { return m_transition[index]; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetPleaseMouseButton() { return m_pleaseMouseButtonText; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAuraBase() { return m_auraBaseTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAuraMask() { return m_auraMaskTexture; }


	//// 説明用画像を渡す
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetExplanationTexture(int type) { return m_explanationTexture[type]; }

	// 画像を読み込む
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadingPngFile(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource);

public:

	void PlaySceneLoad();
	void TitleSceneLoad();
	void ResultSceneLoad();

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//　選択ボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	// 錬金用選択ボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_alchemiSelectTexture;

	// 錬金用選択ボックスのカバーテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_alchemiCoverTexture;

	// マシンUIのカーソルテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineUICursorTexture;

	// マウスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mouseTexture;

	//　属性アイコンのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_elementTextures;

	//　マシンの名前のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineNameTextures;

	// エネミーの名前テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyNameTextures;

	// 製造の文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;

	// マシンアイコン
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineIconTexture;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_explanationTextTexture;

	// をテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_letterTexture;

	// ミッション文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionTexture;

	// ミッションのラベルテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionLabelTexture;

	// デザイン数字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTexture;

	// リザルト画面にて使用する文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTextTexture;

	// ステージクリア,失敗を示す文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageClearTextTexture;

	// 魔法陣のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_MagicCircleTexture[6 + 1];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleLogo;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiIcons;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_uiArrow;

	// ルール画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rule;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_transition[4];

	// PleaseMouseButtonのテキスト画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pleaseMouseButtonText;

	// ノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineNormalMap[5];
	// マシンテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_machineTexture[5];

	// エフェクト用のテクスチャ(オーラ表現)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_auraBaseTexture;
	// エフェクト用のテクスチャ(オーラ表現)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_auraMaskTexture;


};