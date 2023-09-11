//--------------------------------------------------------------------------------------
// File: MissionRender.h
//
//�@�~�b�V�������e��\������N���X
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/UI/Number.h"
class Stage_Condition;
class SpriteLoder;
class Stage_Data;

class MissionRender
{
public:

	MissionRender(SimpleMath::Vector2 pos,SimpleMath::Vector2 rage);
	~MissionRender();

	void Render();
	void Render(Stage_Data data);

	// �}�V���n�̃~�b�V�������e��`��
	void Render_MachineMission(std::vector<Stage_Condition> stageData);

	// �G�l�~�[�n�̂̃~�b�V�������e��`��
	void Render_EnemyMission(std::vector<Stage_Condition> stageData);

	// �^�C�}�[�n�̃~�b�V�������e��`��
	void Render_TimerMission(std::vector<Stage_Condition> stageData);

	// �s�̕ϐ���0�ɂ���
	void LineReset();

	// ���C���̃J�E���^�[��Ԃ�
	int GetLineCount() { return m_lineCount; }

private:

	void LabelDraw(SimpleMath::Vector2 pos);

private:

	std::unique_ptr<Number> m_number;

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	int m_lineCount;

};