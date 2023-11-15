#pragma once

#include "NecromaLib/GameData/Animation.h"
#include "Scenes/PlayScene/Tutorial/CommandPattern/ICommand_Tutorial.h"
#include "Scenes/PlayScene/Tutorial/CommandPattern/Commander_Tutorial.h"

class PlayScene;
class DrawArrow;
class DrawBox;
class SelectionBox;
class Particle;

struct Tutorial_Status;

/// <summary>
/// チュートリアルステージの処理を総括します。
/// </summary>
class TutorialManager
{
public:

public:
	TutorialManager(PlayScene* pPlayScene);
	~TutorialManager();

	void Update(bool clearMission);

	void Render();
	void Render_Layer2();

	/// <summary>
	/// ウェーブが進んだ際、新たにテキスト画像を読み取り、
	/// チュートリアルの処理内容も変更する
	/// </summary>
	void ChangeWave(int wave);

	/// <summary>
	/// チュートリアルコマンドを生成する
	/// </summary>
	/// <param name="type">生成するチュートリアル</param>
	/// <returns></returns>
	ICommand_Tutorial* GenerateCommand(Tutorial_Status type);

public:

	PlayScene* GetPlayScene() { return m_PlayScene; }

	DrawBox* GetDisplayBox() { return m_displayBox.get(); }

	Particle* GetParticle() { return m_machineSelectParticle.get(); }


private:

	int m_tutorialNum;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tutorialTextBox;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tutorialTextTex;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;

	std::unique_ptr<DrawArrow> m_leftArrow, m_rightArrow;

	// チュートリアル表示用ボックス
	std::unique_ptr<DrawBox> m_displayBox;

	std::unique_ptr<SelectionBox> m_selectNextWaveBox;

	std::unique_ptr<TutorialCommander> m_tutorialCommander;

	// Noneマシンに視線誘導を行う為のパーティクル
	std::unique_ptr<Particle> m_machineSelectParticle;

	PlayScene* m_PlayScene;

	// 透明から不透明　下からゆっくり現れる表現に使用
	AnimationData m_transitionTime;
	bool m_transitionFlag;

	int m_texWidth, m_texHeight;

	// テクスチャを切る回数
	int m_maxCutting;
	// 現在切られた数
	int m_nowCutting;

	bool m_selectNextWave;

};