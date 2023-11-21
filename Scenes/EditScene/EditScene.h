

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/SelectScene/MissionRender.h"

#include "Scenes/Commons/DrawVariableNum.h"

#include <list>

class MissonRender;
class DrawArrow;
class Number;
class DrawSlider;
class TitleLogo;
class AM_Attacker;
class MoveCamera;

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

	void AddMission(std::vector<std::unique_ptr<SelectionBox>> &ui, SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);

	void WritingFile();

private:

private:

	// �������ސ��������f�[�^
	Stage_Data m_writingData;

	// �~�b�V�������e��`�悷��
	std::unique_ptr<MissionRender> m_missionRender;

	// �}�V���̐����֌W�̃��[�U�[�ϐ��l
	std::unique_ptr<DrawVariableNum> m_userVariable[MISSION_TYPE::MISSION_NUM];

	// ���\�[�X�Q
	std::unique_ptr<DrawVariableNum> m_resource_Machine[MACHINE_TYPE::NUM];
	std::unique_ptr<DrawSlider>		 m_resource_MP;
	std::unique_ptr<DrawSlider>		 m_resource_Crystal;

	// �ǂݍ��݃{�^��
	std::unique_ptr<SelectionBox> m_writingButton;

	// ����{�^��
	std::unique_ptr<SelectionBox> m_decisionButton;

	std::unique_ptr<MoveCamera> m_camera;

};