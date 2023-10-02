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
struct Stage_Data;

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

	// �B���n�̃~�b�V�������e��`��
	void Render_AlchemiMission(std::vector<Stage_Condition> stageData);

	// ���_Lv�̃~�b�V�������e��`��
	void Render_BaseLvMission(std::vector<Stage_Condition> stageData);

	// �^�C�}�[�n�̃~�b�V�������e��`��
	void Render_TimerMission(std::vector<Stage_Condition> stageData);

	// �s�̕ϐ���0�ɂ���
	void LineReset();

	// ���C���̃J�E���^�[��Ԃ�
	const int GetLineCount() const  { return m_lineCount; }

	const SimpleMath::Vector2 GetPos()  ;
	const SimpleMath::Vector2 GetRage() ;

private:

	void LabelDraw(SimpleMath::Vector2 pos);

private:

	std::unique_ptr<Number> m_number;

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	int m_lineCount;
	int m_missionNum;

};