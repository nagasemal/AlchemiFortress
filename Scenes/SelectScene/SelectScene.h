#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/SelectScene/SelectCamera.h"
#include "Scenes/SelectScene/MissionRender.h"

class SelectScene : public SceneObj
{
public:
	// �R���X�g���N�^
	SelectScene();

	// �f�X�g���N�^
	~SelectScene();

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

	int GetStageNumber() const { return m_selectStageNumber;}
	void SetStageNumber(int number) { m_selectStageNumber = number;}

private:

	// �I�������X�e�[�W�̔ԍ�
	int m_selectStageNumber;

	// �Z���N�g�V�[�����̃J����
	std::unique_ptr<SelectCamera> m_selectCamera;

	// �}�V���̕`��
	std::unique_ptr<DrawMachine> m_machineDraw;

	// �~�b�V�����̕`��
	std::unique_ptr<MissionRender> m_missionDraw;

	bool m_changeMachine;							// �}�V�����ύX���ꂽ���Ƃ�m�点��t���O

};