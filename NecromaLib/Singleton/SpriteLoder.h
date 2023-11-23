//--------------------------------------------------------------------------------------
// File: SpriteLoder.h
//
// スプライト情報を保存するシングルトンクラス
// 繰り返し使うものや、シーンをまたいで使うものは定義して使いやすくしておく
// 
// Date: 2023.6.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

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
		CANCEL = 15,

		NUM

	};

	// テクスチャ情報
	struct TextureData
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex = nullptr;
		int width = 0;
		int height = 0;
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

	// ゲームスタートの文字を渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetGameStartTexture() { return m_gameStartTexture; }


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

	// ステージクリア、失敗を示すアイコンを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetStageClearTextTexture() { return m_stageClearTextTexture; }
	const wchar_t* GetStageClearTextTexturePath() { return L"Resources/Textures/SuccessFailureText.png"; }

	// デザイン数字テクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNumberTexture() { return m_NumberTexture; }
	const wchar_t* GetNumberTexturePath() { return L"Resources/Textures/number.png"; }

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

	// エネミー状態アイコンテクスチャを渡す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetEnemyStateTexture() { return m_enemyStateTexture; }

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

	// 床のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetFiledNormalMap() { return m_fieldNormalMap; }
	// 床のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetFiledTexture() { return m_fieldTexture; }

	// 塔[ベース]のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTowerBaseNormalMap() { return m_towerNormalMap; }
	// 塔[ベース]のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTowerBaseTexture() { return m_towerTexture; }

	// 塔[柱]のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTowerPillarNormalMap() { return m_towerPillarNormalMap; }
	// 塔[柱]のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTowerPillarTexture() { return m_towerPillarTexture; }


	// 0~3
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMagicRule(int index) { return m_transition[index]; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetPleaseMouseButton() { return m_pleaseMouseButtonText; }

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAuraBase() { return m_auraBaseTexture; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetAuraMask() { return m_auraMaskTexture; }


	// ゲージ(背景)を返す
	TextureData GetBaseGage() { return m_baseGageTex; }

	// ゲージ(中身)を返す
	TextureData GetMainGage() { return m_mainGageTex; }

	// マシンアイコンを渡す
	TextureData GetMachineIconTexture() { return m_machineIconTexture; }
	// マシンの名前を渡す
	TextureData GetMachineNameTexture() { return m_machineNameTextures; }

	/// <summary>
	/// 魔法陣テクスチャを渡す
	/// </summary>
	/// <param name="machineType">マシンID</param>
	/// <returns></returns>
	TextureData GetMachineMagicCircleTexture(int machineType) { return m_magicCircleTexture[machineType]; }

	/// <summary>
	/// マシンUIテクスチャを渡す
	/// </summary>
	/// <param name="machineType">マシンID</param>
	/// <returns></returns>
	TextureData GetMachineUIText(int machineType) { return m_machineUIText[machineType]; }

	// UI用のテキスト
	TextureData GetUIText() { return m_uiText; }

	// 画像を読み込む
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadingPngFile(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource);

	// 画像を読み込む
	TextureData LoadingPngFileData(const wchar_t* filename, TextureData& textuerData);

public:

	void PlaySceneLoad();
	void TitleSceneLoad();
	void ResultSceneLoad();

private:
	SpriteLoder();
	static SpriteLoder* instance;

	//　選択ボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectBoxTexture;

	//　ゲームスタートの文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gameStartTexture;

	//  エネミーアイコンテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyStateTexture;


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

	// エネミーの名前テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyNameTextures;

	// 製造の文字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ManufacturingTexture;




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

	// 床のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fieldNormalMap;
	// 床のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fieldTexture;

	// 塔[ベース]のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_towerNormalMap;
	// 塔[ベース]のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_towerTexture;

	// 塔[柱]のノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_towerPillarNormalMap;
	// 塔[柱]のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_towerPillarTexture;



	// エフェクト用のテクスチャ(オーラ表現)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_auraBaseTexture;
	// エフェクト用のテクスチャ(オーラ表現)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_auraMaskTexture;


	//　マシンの名前のテクスチャ
	TextureData m_machineNameTextures;
	// マシンアイコン
	TextureData m_machineIconTexture;

	// 魔法陣のテクスチャ
	TextureData m_magicCircleTexture[6 + 1];

	// ゲージ(背景)
	TextureData m_baseGageTex;

	// ゲージ(中身)
	TextureData m_mainGageTex;

	// マシンUIの説明テキスト
	TextureData m_machineUIText[6];

	// UIの説明テキスト
	TextureData m_uiText;

};