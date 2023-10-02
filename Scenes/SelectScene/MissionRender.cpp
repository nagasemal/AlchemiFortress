#include "pch.h"
#include "MissionRender.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define KERNING_X 64
#define KERNING_Y 48

#define TEXT_COLOR SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.75f)
#define CLEAR_COLOR SimpleMath::Color(1.0f, 0.0f, 0.0f, 0.85f)

MissionRender::MissionRender(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{

	m_position = pos;
	m_rage = rage;
	m_lineCount = 0;


	m_number = std::make_unique<Number>(pos,SimpleMath::Vector2(0.35f,0.25f));


	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	Stage_Data missionData = pSJD.GetStageData();
	m_missionNum = static_cast<int>(missionData.condition_Enemy.size() + missionData.condition_Machine.size() + missionData.condition_Time.size());

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
	Render_AlchemiMission(missionData.condition_Alchemi);
	Render_EnemyMission(missionData.condition_Enemy);
	Render_BaseLvMission(missionData.condition_BaseLv);
	Render_TimerMission(missionData.condition_Time);
}


void MissionRender::Render(Stage_Data data)
{
	m_lineCount = 0;

	Render_MachineMission(data.condition_Machine);
	Render_AlchemiMission(data.condition_Alchemi);
	Render_EnemyMission(data.condition_Enemy);
	Render_BaseLvMission(data.condition_BaseLv);
	Render_TimerMission(data.condition_Time);

}

void MissionRender::Render_MachineMission(std::vector<Stage_Condition> stageData)
{

	// 目標値が-1以下ならば更新する必要が無い　(仕様上要素がなくてはならない為この形で実装をする)
	if (stageData[0].value <= -1) return;

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	for (int i = 0; i < stageData.size(); i++)
	{

		SimpleMath::Vector2 pos = m_position;
		pos.y += m_lineCount * KERNING_Y;

		SimpleMath::Color color = TEXT_COLOR;

		if (stageData[i].progress >= stageData[i].value) color = CLEAR_COLOR;

		LabelDraw(pos);

		// 文字の名前描画
		{
			RECT machineNameRect = SpriteCutter(128, 28, (int)Json::ChangeMachine(stageData[i].condition), 0);

			pSB->Draw(pSL.GetMachineNameTexture().Get(),
				pos,
				&machineNameRect,
				color,
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
				color,
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
				color,
				0.0f,
				DirectX::XMFLOAT2(112 / 2, 28 / 2),
				m_rage);
		}

		// ミッションの達成数描画
		{
			pos.x += KERNING_X;
			m_number->SetPosition(pos);
			m_number->SetColor(color);
			m_number->Render_SelectScene(stageData[i].progress, stageData[i].value);
		}

		m_lineCount++;
	}
}

void MissionRender::Render_EnemyMission(std::vector<Stage_Condition> stageData)
{

	// 目標値が-1以下ならば更新する必要が無い　(仕様上要素がなくてはならない為この形で実装をする)
	if (stageData[0].value <= -1) return;

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	for (int i = 0; i < stageData.size(); i++)
	{

		SimpleMath::Vector2 pos = m_position;

		pos.y += m_lineCount * KERNING_Y;

		SimpleMath::Color color = TEXT_COLOR;

		if(stageData[i].progress >= stageData[i].value) color = CLEAR_COLOR;

		// 背面に描画するテクスチャ
		LabelDraw(pos);

		// ミッション描画：目的語
		{
			RECT machineNameRect = SpriteCutter(128, 28,(int)Json::ChangeMachine(stageData[i].condition), 0);
			
			pSB->Draw(pSL.GetEnemyNameTexture().Get(),
				pos,
				&machineNameRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(128 / 2, 28 / 2),
				m_rage);
		}

		// ミッション描画：接続詞
		{
			RECT letterRect = SpriteCutter(28, 28, 0, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetLetterTexture().Get(),
				pos,
				&letterRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(28 / 2, 28 / 2),
				m_rage);
		}

		// ミッション描画：内容
		{
			RECT missionRect = SpriteCutter(112, 28, 2, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetMissionTexture().Get(),
				pos,
				&missionRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(112 / 2, 28 / 2),
				m_rage);
		}

		// 目的値と現在値の数字の描画
		{
			pos.x += KERNING_X;
			m_number->SetPosition(pos);
			m_number->SetColor(color);
			m_number->Render_SelectScene(stageData[i].progress, stageData[i].value);
		}
		m_lineCount++;
	}

}

void MissionRender::Render_AlchemiMission(std::vector<Stage_Condition> stageData)
{

	// 目標値が-1以下ならば更新する必要が無い　(仕様上要素がなくてはならない為この形で実装をする)
	if (stageData[0].value <= -1) return;

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	for (int i = 0; i < stageData.size(); i++)
	{

		SimpleMath::Vector2 pos = m_position;

		pos.y += m_lineCount * KERNING_Y;

		SimpleMath::Color color = TEXT_COLOR;

		if (stageData[i].progress >= stageData[i].value) color = CLEAR_COLOR;

		// 背面に描画するテクスチャ
		LabelDraw(pos);

		// ミッション描画：目的語
		{
			RECT machineNameRect = SpriteCutter(128, 28, (int)Json::ChangeMachine(stageData[i].condition), 0);

			pSB->Draw(pSL.GetMachineNameTexture().Get(),
				pos,
				&machineNameRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(128 / 2, 28 / 2),
				m_rage);
		}

		// ミッション描画：接続詞
		{
			RECT letterRect = SpriteCutter(28, 28, 0, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetLetterTexture().Get(),
				pos,
				&letterRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(28 / 2, 28 / 2),
				m_rage);
		}

		// ミッション描画：内容
		{
			RECT missionRect = SpriteCutter(112, 28, 3, 0);

			pos.x += KERNING_X;

			pSB->Draw(pSL.GetMissionTexture().Get(),
				pos,
				&missionRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(112 / 2, 28 / 2),
				m_rage);
		}

		// 目的値と現在値の数字の描画
		{
			pos.x += KERNING_X;
			m_number->SetPosition(pos);
			m_number->SetColor(color);
			m_number->Render_SelectScene(stageData[i].progress, stageData[i].value);
		}
		m_lineCount++;
	}


}

void MissionRender::Render_BaseLvMission(std::vector<Stage_Condition> stageData)
{

	// 目標値が1以下ならば更新する必要が無い
	if (stageData[0].value <= 1) return;

	SimpleMath::Vector2 pos = m_position;

	pos.y += m_lineCount * KERNING_Y;

	SimpleMath::Color color = TEXT_COLOR;

	if (stageData[0].progress >= stageData[0].value) color = CLEAR_COLOR;

	pos.x += KERNING_X;
	m_number->SetPosition(pos);
	m_number->SetColor(color);
	m_number->Render_SelectScene(stageData[0].progress, stageData[0].value);

	m_lineCount++;
}

void MissionRender::Render_TimerMission(std::vector<Stage_Condition> stageData)
{

}

void MissionRender::LineReset()
{
	m_lineCount = 0;
}

const SimpleMath::Vector2 MissionRender::GetPos()
{

	SimpleMath::Vector2 pos = SimpleMath::Vector2();


	pos.x = 190.0f;
	pos.y += m_position.y +  GetRage().y - KERNING_Y;

	return pos;
}

const SimpleMath::Vector2 MissionRender::GetRage()
{
	SimpleMath::Vector2 rage = SimpleMath::Vector2(190.0f, 24.0f * (float)m_missionNum);

	return rage;
}

void MissionRender::LabelDraw(SimpleMath::Vector2 pos)
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();


	RECT labelRect = SpriteCutter(360, 48, 0, 0);

	pSB->Draw(pSL.GetMissionLabelTexture().Get(),
		pos,
		&labelRect,
		TEXT_COLOR,
		0.0f,
		DirectX::XMFLOAT2(180 / 2, 48 / 2),
		m_rage);

}
