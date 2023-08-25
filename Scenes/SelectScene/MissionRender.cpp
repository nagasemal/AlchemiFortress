#include "pch.h"
#include "MissionRender.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define KERNING_X 64
#define KERNING_Y 48

MissionRender::MissionRender(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 rage)
{

	m_position = pos;
	m_rage = rage;
	m_lineCount = 0;


	m_number = std::make_unique<Number>(pos,DirectX::SimpleMath::Vector2(0.35f,0.25f));

}

MissionRender::~MissionRender()
{
}


void MissionRender::Render()
{
	m_lineCount = 0;

	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	Stage_Data missionData = pSJD.GetStageData();

	Render_MachineMission(missionData.condition_Machine);
	Render_EnemyMission(missionData.condition_Enemy);
	Render_TimerMission(missionData.condition_Time);
}

void MissionRender::Render_MachineMission(std::vector<Stage_Condition> stageData)
{
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	for (int i = 0; i < stageData.size(); i++)
	{

		DirectX::SimpleMath::Vector2 pos = m_position;
		pos.y += m_lineCount * KERNING_Y;

		LabelDraw(pos);

		// 文字の名前描画
		{
			RECT machineNameRect = SpriteCutter(128, 28, (int)Json::ChangeMachine(stageData[i].condition), 0);

			pSB->Draw(pSL.GetMachineNameTexture().Get(),
				pos,
				&machineNameRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(128 / 2, 28 / 2),
				m_rage);
		}

		// ミッション内容描画
		{
			RECT letterRect = SpriteCutter(28, 28, 0, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetLetterTexture().Get(),
				pos,
				&letterRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(28 / 2, 28 / 2),
				m_rage);
		}

		// ミッションの内容描画_2
		{
			RECT missionRect = SpriteCutter(112, 28, 0, 0);
			pos.x += KERNING_X;

			pSB->Draw(pSL.GetMissionTexture().Get(),
				pos,
				&missionRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(112 / 2, 28 / 2),
				m_rage);
		}

		// ミッションの達成数描画
		{
			pos.x += KERNING_X;
			m_number->SetPosition(pos);
			m_number->Render_SelectScene(stageData[i].progress, stageData[i].value);
		}

		m_lineCount++;
	}
}

void MissionRender::Render_EnemyMission(std::vector<Stage_Condition> stageData)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	for (int i = 0; i < stageData.size(); i++)
	{

		DirectX::SimpleMath::Vector2 pos = m_position;

		pos.y += m_lineCount * KERNING_Y;

		LabelDraw(pos);

		// 文字の名前描画
		{
			RECT machineNameRect = SpriteCutter(128, 28,(int)Json::ChangeMachine(stageData[i].condition), 0);
			
			pSB->Draw(pSL.GetEnemyNameTexture().Get(),
				pos,
				&machineNameRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(128 / 2, 28 / 2),
				m_rage);
		}

		// ミッション内容描画
		{
			RECT letterRect = SpriteCutter(28, 28, 0, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetLetterTexture().Get(),
				pos,
				&letterRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(28 / 2, 28 / 2),
				m_rage);
		}

		{
			RECT missionRect = SpriteCutter(112, 28, 2, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetMissionTexture().Get(),
				pos,
				&missionRect,
				DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f),
				0.0f,
				DirectX::XMFLOAT2(112 / 2, 28 / 2),
				m_rage);
		}

		{
			pos.x += KERNING_X;
			m_number->SetPosition(pos);
			m_number->Render_SelectScene(stageData[i].progress, stageData[i].value);
		}
		m_lineCount++;
	}

}

void MissionRender::Render_TimerMission(std::vector<Stage_Condition> stageData)
{

}

void MissionRender::LineReset()
{
	m_lineCount = 0;
}

void MissionRender::LabelDraw(DirectX::SimpleMath::Vector2 pos)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();


	RECT labelRect = SpriteCutter(360, 48, 0, 0);

	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		pos,
		&labelRect,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.8f),
		0.0f,
		DirectX::XMFLOAT2(180 / 2, 48 / 2),
		m_rage);

}
