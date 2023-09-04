#include "pch.h"
#include "EditScene.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "Scenes/SelectScene/MissionRender.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/PlayScene/UI/Number.h"

EditScene::EditScene():
    m_stageData(),
    m_stageNum()
{
}

EditScene::~EditScene()
{
}

void EditScene::Initialize()
{

    m_ui_StageNumber = std::make_unique<Number>(DirectX::SimpleMath::Vector2(1128.0f, 600.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));
    m_stageNum_puls = std::make_unique <DrawArrow>(DirectX::SimpleMath::Vector2(1200.0f, 600.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f),2);
    m_stageNum_minus = std::make_unique <DrawArrow>(DirectX::SimpleMath::Vector2(1000.0f, 600.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f),4);

    m_decisionButton = std::make_unique<SelectionBox>(DirectX::SimpleMath::Vector2(1100.0f,600.0f),DirectX::SimpleMath::Vector2(2.0f,1.0f));

    m_missionRender = std::make_unique<MissionRender>(DirectX::SimpleMath::Vector2(300.0f, 150.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

    for (int i = 0; i < MACHINE_TYPE::NUM; i++)
    {
        m_machineMissions_puls[i]  = std::make_unique<DrawArrow>(DirectX::SimpleMath::Vector2(550.0f, 150.0f + (i * 48)), DirectX::SimpleMath::Vector2(0.6f, 0.6f),2);
        m_machineMissions_minus[i] = std::make_unique<DrawArrow>(DirectX::SimpleMath::Vector2(490.0f, 150.0f + (i * 48)), DirectX::SimpleMath::Vector2(0.6f, 0.6f),4);


        Stage_Condition machineData;

        machineData.condition = Json::ChangeMachineString((MACHINE_TYPE)i);
        machineData.progress = 0;
        machineData.value = 0;

        m_stageData.condition_Machine.push_back(machineData);
    
    }

}

GAME_SCENE EditScene::Update()
{
    for (int i = 1; i < MACHINE_TYPE::NUM;i++)
    {
        m_machineMissions_puls[i]->HitMouse();
        m_machineMissions_minus[i]->HitMouse();

        m_stageData.condition_Machine[i].value += m_machineMissions_puls[i]->ClickMouse();
        m_stageData.condition_Machine[i].value -= m_machineMissions_minus[i]->ClickMouse();

        m_stageData.condition_Machine[i].value = std::min(std::max(m_stageData.condition_Machine[i].value, 0), 99);
    }

    for (auto& missionEnemy : m_enemyMissions)
    {
        missionEnemy->HitMouse();
    }

    for (auto& missionTime : m_timeMissions)
    {
        missionTime->HitMouse();
    }

    m_stageNum_puls->HitMouse();
    m_stageNum_minus->HitMouse();
    m_stageNum += m_stageNum_puls->ClickMouse();
    m_stageNum -= m_stageNum_minus->ClickMouse();

    m_stageNum = std::min(std::max(m_stageNum, 0), 99);

    m_ui_StageNumber->SetNumber(m_stageNum);

    WritingFile();

    return GAME_SCENE();
}

void EditScene::Draw()
{
    ShareData& pSD = ShareData::GetInstance();
    auto pSB = pSD.GetSpriteBatch();
    auto device = pSD.GetDeviceResources();

    device;

    pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

    //m_missionRender->Render_MachineMission(m_stageData.condition_Machine);
    //m_missionRender->Render_EnemyMission(m_stageData.condition_Enemy);
    //m_missionRender->Render_TimerMission(m_stageData.condition_Time);

    m_missionRender->Render(m_stageData);

    for (int i = 1; i < MACHINE_TYPE::NUM; i++)
    {
        m_machineMissions_puls[i]->Draw();
        m_machineMissions_minus[i]->Draw();
    }

    m_stageNum_puls->Draw();
    m_stageNum_minus->Draw();


    pSB->End();
}

void EditScene::DrawUI()
{

    m_decisionButton->Draw();

    // ステージ番号を決める
    m_ui_StageNumber->Render();

    for (auto& missionEnemy : m_enemyMissions)
    {
        missionEnemy->Draw();
    }

    for (auto& missionTime : m_timeMissions)
    {
        missionTime->Draw();
    }

}

void EditScene::Finalize()
{
}

void EditScene::AddMission(std::vector<std::unique_ptr<SelectionBox>>& ui,DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage)
{
    ui.push_back(std::make_unique<SelectionBox>(pos, rage));
}

void EditScene::WritingFile()
{

    m_decisionButton->HitMouse();

    //　ファイル生成　処理
    if (m_decisionButton->ClickMouse())
    {



        // 書き込み
        Json::WritingJsonFile_StageData(m_stageNum,m_stageData);
    }

}
