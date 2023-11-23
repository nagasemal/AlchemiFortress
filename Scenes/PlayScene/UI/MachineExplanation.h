//--------------------------------------------------------------------------------------
// File: MachineExplanation.h
//
//　スプライトバッチ　使用
//　アルケミカルマシンの選択時、画面全体に出現させる魔法陣の処理
// 
// Use		: AlchemicalMachineManager
// Date		: 2023.7.17
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Camera.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/UI/MachineGauge.h"

class MachineExplanation : public GameObjct2D
{
public:
	MachineExplanation();
	~MachineExplanation();

	// 初期化
	void Initialize()			override;

	// 更新
	void Update()				override;

	// マシンデーター用のアップデート処理
	void Update_MachineData(AlchemicalMachineObject* object);

	// 描画
	void Draw()					override;

	// 終了処理
	void Finalize()				override;

	// モデルを描画するための関数
	void DisplayObject(DirectX::Model* model,DirectX::Model* secondModel = nullptr,AlchemicalMachineObject* object = nullptr);

	/// <summary>
	/// マシンをクリックし、メニューを開いた際に現れるボタンのポインターを返します
	/// </summary>
	/// <param name="buttonType">0 = 修繕 ,1 = 破壊 , 2 = レベルアップ</param>
	/// <returns></returns>
	SelectionBox* GetMenuButton(int buttonType);

	// 解体された瞬間を返すフラグ
	bool GetDismantlingFlag();

	// 修繕された瞬間を返すフラグ
	bool GetRepairFlag();

	// LvUPされた瞬間を返すフラグ
	bool GetLvUpFlag();

	void ResetMoveTime() { m_moveTime = 0; m_hitFlag = false;}

public:

	// マウスが説明ウィンドウに触れている
	bool OnMouse();

private:

	float m_moveTime;

	SimpleMath::Vector3 m_modelPos;

	std::unique_ptr<Camera> m_camera;

	// テキストボックスのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// LvUp用の選択ボックス
	std::unique_ptr<SelectionBox> m_selectLvUpBox;
	// 修理用の選択ボックス
	std::unique_ptr<SelectionBox> m_repairBox;
	// 解体用の選択ボックス
	std::unique_ptr<SelectionBox> m_dismantlingBox;

	// マシンのHPゲージ
	//std::unique_ptr<MachineGauge> m_machineGauge;

	bool m_hitFlag;

};