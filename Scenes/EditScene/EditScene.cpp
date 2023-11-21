#include "pch.h"
#include "EditScene.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/SelectScene/MissionRender.h"
#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/Commons/DrawSlider.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/TitleScene/TitleLogo/TitleLogo.h"

#include "Scenes/PlayScene/AlchemicalMachine/AM_Attacker.h"
#include "Scenes/PlayScene/Camera/MoveCamera.h"

#include "Scenes/DataManager.h"

#define SLIDER_POS_X 250

#define SLIDER_POS_Y 50

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
}

void EditScene::Initialize()
{
    SpriteLoder& pSL = SpriteLoder::GetInstance();
    ShareData& pSD = ShareData::GetInstance();

    m_decisionButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(1100.0f,600.0f),SimpleMath::Vector2(2.0f,1.0f));

    m_missionRender = std::make_unique<MissionRender>(SimpleMath::Vector2(300.0f, 150.0f), SimpleMath::Vector2(1.0f, 1.0f));

    // 初期化
    m_writingData.crystalPos          .push_back(SimpleMath::Vector2());
    m_writingData.enemys_Spawn        .push_back(Enemys_Spawn());
    m_writingData.machine             .push_back(Stage_Machine());
    m_writingData.resource            = Stage_Resource();
    m_writingData.tutorial            = std::vector<Tutorial_Status>();
    m_writingData.lastWave            = false;

    for (int i = 0; i < MISSION_TYPE::MISSION_NUM; i++)
    {
        m_writingData.condition[i].push_back(Stage_Condition());

        m_userVariable[i] = std::make_unique<DrawVariableNum>(SimpleMath::Vector2(600.0f, 150.0f + 48 * i), SimpleMath::Vector2(0.5f, 0.5f));
    }

    m_resource_MP       = std::make_unique<DrawSlider>(SimpleMath::Vector2(SLIDER_POS_X, SLIDER_POS_Y    ), SimpleMath::Vector2(0.5f, 0.5f));
    m_resource_MP       ->Initialize();

    m_resource_Crystal  = std::make_unique<DrawSlider>(SimpleMath::Vector2(SLIDER_POS_X, SLIDER_POS_Y * 2), SimpleMath::Vector2(0.5f, 0.5f));
    m_resource_Crystal  ->Initialize();


    auto device = ShareData::GetInstance().GetDeviceResources();
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
    fx->SetDirectory(L"Resources/Models");

    m_camera = std::make_unique<MoveCamera>();
    m_camera->Initialize();

}

GAME_SCENE EditScene::Update()
{
    ShareData& pSD = ShareData::GetInstance();
    // カメラを動かす
    m_camera->Update(true , true);
    m_camera->SetTargetProsition(SimpleMath::Vector3::Zero);

    for (int i = 0; i < MISSION_TYPE::MISSION_NUM; i++)
    {
        m_userVariable[i]->Update(0,999);

    }

    return GAME_SCENE();
}

void EditScene::Draw()
{
    ShareData& pSD = ShareData::GetInstance();
    auto device = pSD.GetDeviceResources();

    device;

}

void EditScene::DrawUI()
{

    ShareData& pSD = ShareData::GetInstance();
    auto pSB = pSD.GetSpriteBatch();

    pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
    m_missionRender->Render(m_writingData);
    pSB->End();

    for (int i = 0; i < MISSION_TYPE::MISSION_NUM; i++)
    {
        m_userVariable[i]->Render();
    }



}

void EditScene::Finalize()
{
}

void EditScene::AddMission(std::vector<std::unique_ptr<SelectionBox>>& ui,SimpleMath::Vector2 pos,SimpleMath::Vector2 rage)
{
    ui.push_back(std::make_unique<SelectionBox>(pos, rage));
}

// ステージデータの0要素を切ります
template <class T>
void ZeroCut(std::vector<T>& Val)
{

    Val.erase(std::remove_if(std::begin(Val),
        std::end(Val),
        [](Stage_Condition s) { return s.value == 0; }),
        std::cend(Val));

}

void EditScene::WritingFile()
{

    //m_decisionButton->HitMouse();
    //
    ////　ファイル生成　処理
    //if (m_decisionButton->ClickMouse())
    //{
    //
    //    Stage_Condition testEnemyMission;
    //    testEnemyMission.condition = "Slime";
    //    testEnemyMission.value = 1;
    //
    //    Enemys_Spawn spawn;
    //    spawn.spawnTime = 2;
    //    spawn.spawnPos.x = 10;
    //    spawn.spawnPos.y = 10;
    //    spawn.type = SLIME;
    //
    //    Stage_Condition testTimeMission;
    //    testTimeMission.condition = "True";
    //    testTimeMission.value = 20;
    //
    //    Stage_Data stageCondition = m_stageData;
    //    stageCondition.condition_Enemy.push_back(testEnemyMission);
    //    stageCondition.enemys_Spawn.push_back(spawn);
    //    stageCondition.condition_Time.push_back(testTimeMission);
    //
    //
    //    // 要素0を切る                  
    //    ZeroCut(stageCondition.condition_Machine);
    //    ZeroCut(stageCondition.condition_Enemy);  
    //    ZeroCut(stageCondition.condition_Time);   
    //
    //    DataManager* pDataM = DataManager::GetInstance();
    //
    //    pDataM->Initialize();
    //
    //    stageCondition.resource.crystal = (int)(m_resource_Crystal->GetValue() * (float)pDataM->GetNowCrystal_MAX());
    //    stageCondition.resource.mp      = (int)(m_resource_MP->GetValue()      * (float)pDataM->GetNowMP_MAX());
    //
    //    // 書き込み
    //    Json::WritingJsonFile_StageData(m_stageNum, stageCondition);
    //}

}
