

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/SelectScene/MissionRender.h"

#include <list>

class MissonRender;
class DrawArrow;
class Number;

class EditScene : public SceneObj
{
public:
	// �R���X�g���N�^
	EditScene();

	// �f�X�g���N�^
	~EditScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update() override;

	// �`��
	void Draw() override;

	// UI�n�`��
	void DrawUI() override;

	// �I������
	void Finalize() override;

	void AddMission(std::vector<std::unique_ptr<SelectionBox>> &ui, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 rage);

	void WritingFile();

private:

	int m_stageNum;
	Stage_Data m_stageData;

	std::unique_ptr<MissionRender> m_missionRender;

	// �~�b�V�������e��`��
	std::unique_ptr<DrawArrow> m_machineMissions_puls [MACHINE_TYPE::NUM];
	std::unique_ptr<DrawArrow> m_machineMissions_minus[MACHINE_TYPE::NUM];

	std::vector<std::unique_ptr<SelectionBox>> m_enemyMissions;
	std::vector<std::unique_ptr<SelectionBox>> m_timeMissions;

	// �X�e�[�W�ԍ����Ǘ�
	std::unique_ptr<Number> m_ui_StageNumber;
	std::unique_ptr<DrawArrow> m_stageNum_puls;
	std::unique_ptr<DrawArrow> m_stageNum_minus;

	// ���\�[�X�Q
	std::unique_ptr<SelectionBox> m_resource_Machine[MACHINE_TYPE::NUM];
	std::unique_ptr<SelectionBox> m_resource_Lv;
	std::unique_ptr<SelectionBox> m_resource_MP;
	std::unique_ptr<SelectionBox> m_resource_Crystal;

	// �ǂݍ��݃{�^��
	std::unique_ptr<SelectionBox> m_writingButton;

	// ����{�^��
	std::unique_ptr<SelectionBox> m_decisionButton;

};