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

EditScene::EditScene():
    m_stageData(),
    m_stageNum(),
    m_shaderFlag()
{
}

EditScene::~EditScene()
{
}

void EditScene::Initialize()
{

    m_ui_StageNumber = std::make_unique<Number>(SimpleMath::Vector2(1128.0f, 600.0f), SimpleMath::Vector2(1.0f, 1.0f));
    m_stageNum_puls = std::make_unique <DrawArrow>(SimpleMath::Vector2(1200.0f, 600.0f), SimpleMath::Vector2(1.0f, 1.0f),2);
    m_stageNum_minus = std::make_unique <DrawArrow>(SimpleMath::Vector2(1000.0f, 600.0f), SimpleMath::Vector2(1.0f, 1.0f),4);

    m_decisionButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(1100.0f,600.0f),SimpleMath::Vector2(2.0f,1.0f));

    m_missionRender = std::make_unique<MissionRender>(SimpleMath::Vector2(300.0f, 150.0f), SimpleMath::Vector2(1.0f, 1.0f));

    // 初期化
    m_stageData.condition_Alchemi   .push_back(Stage_Condition());
    m_stageData.condition_Enemy     .push_back(Stage_Condition());
    m_stageData.condition_BaseLv    .push_back(Stage_Condition());
    m_stageData.condition_Time      .push_back(Stage_Condition());
    m_stageData.crystalPos          .push_back(SimpleMath::Vector2());
    m_stageData.enemys_Spawn        .push_back(Enemys_Spawn());
    m_stageData.machine             .push_back(Stage_Machine());
    m_stageData.resource            = Stage_Resource();
    m_stageData.tutorial            = std::vector<int>();
    
    for (int i = 0; i < MACHINE_TYPE::NUM; i++)
    {
        m_machineMissions_puls[i]  = std::make_unique<DrawArrow>(SimpleMath::Vector2(550.0f, 150.0f + (i * 48)), SimpleMath::Vector2(0.6f, 0.6f),2);
        m_machineMissions_minus[i] = std::make_unique<DrawArrow>(SimpleMath::Vector2(490.0f, 150.0f + (i * 48)), SimpleMath::Vector2(0.6f, 0.6f),4);

        Stage_Condition machineData;

        machineData.condition   = Json::ChangeMachineString((MACHINE_TYPE)i);
        machineData.progress    = 0;
        machineData.value       = 0;

        m_stageData.condition_Machine.push_back(machineData);
    }

    m_resource_MP       = std::make_unique<DrawSlider>(SimpleMath::Vector2(SLIDER_POS_X, SLIDER_POS_Y    ), SimpleMath::Vector2(0.5f, 0.5f));
    m_resource_MP       ->Initialize();

    m_resource_Crystal  = std::make_unique<DrawSlider>(SimpleMath::Vector2(SLIDER_POS_X, SLIDER_POS_Y * 2), SimpleMath::Vector2(0.5f, 0.5f));
    m_resource_Crystal  ->Initialize();


    auto device = ShareData::GetInstance().GetDeviceResources();
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    SpriteLoder& pSL = SpriteLoder::GetInstance();

    m_titleLogo = std::make_unique<TitleLogo>();
    m_titleLogo->Create(pSL.GetTitleLogoPath());
    m_titleLogo->SetWindowSize(width, height);
    m_titleLogo->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.6f, 1.0f));
    m_titleLogo->SetPosition(SimpleMath::Vector2(width / 1.3f, height / 1.8f));

    m_machine = std::make_unique<AM_Attacker>();
    m_machine->Initialize();
    m_machine->SummonAM(SimpleMath::Vector3());

    ShareData& pSD = ShareData::GetInstance();

    std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
    fx->SetDirectory(L"Resources/Models");
    m_model = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Attacker.cmo", *fx);
    m_model_2 = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/AM_Ring.cmo", *fx);

    m_camera = std::make_unique<MoveCamera>();
    m_camera->Initialize();

}

GAME_SCENE EditScene::Update()
{
    ShareData& pSD = ShareData::GetInstance();
    // カメラを動かす
    m_camera->Update(true , true);
    m_camera->SetTargetProsition(m_machine->GetPos());

    if (InputSupport::GetInstance().GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED)
    {
        m_shaderFlag = !m_shaderFlag;
    }

    if (InputSupport::GetInstance().GetMouseState().rightButton == Mouse::ButtonStateTracker::PRESSED)
    {
    }

    pSD.GetCamera()->SetViewMatrix(m_camera->GetViewMatrix());
    pSD.GetCamera()->SetTargetPosition(m_camera->GetTargetPosition());
    pSD.GetCamera()->SetEyePosition(m_camera->GetEyePosition());

    m_machine->Update_Common();
    m_machine->Update();

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

    // ステージ番号の加減
    m_stageNum += m_stageNum_puls->ClickMouse();
    m_stageNum -= m_stageNum_minus->ClickMouse();

    // ステージ番号の上限下限設定
    m_stageNum = std::min(std::max(m_stageNum, 0), 99);

    m_ui_StageNumber->SetNumber(m_stageNum);

    m_resource_MP           ->Update();
    m_resource_Crystal      ->Update();

    // 書き込む
    WritingFile();

    m_titleLogo->Update();

    return GAME_SCENE();
}

void EditScene::Draw()
{
    ShareData& pSD = ShareData::GetInstance();
    auto device = pSD.GetDeviceResources();

    device;

    //pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

    //// ミッション内容の描画(マシンのみ)
    //m_missionRender->Render(m_stageData);

    //// マシンミッションの加減矢印の描画
    //for (int i = 1; i < MACHINE_TYPE::NUM; i++)
    //{
    //    m_machineMissions_puls[i]->Draw();
    //    m_machineMissions_minus[i]->Draw();
    //}

    //// ステージ指定の矢印描画
    //m_stageNum_puls     ->Draw();
    //m_stageNum_minus    ->Draw();

    //pSB->End();

    //// リソース描画
    //m_resource_MP       ->Render();
    //m_resource_Crystal  ->Render();


    m_machine->SetPos(SimpleMath::Vector3(0, 0, 7));
    m_machine->ModelRender(m_model.get(), m_model_2.get(), m_shaderFlag);

    //m_machine->Draw();

}

void EditScene::DrawUI()
{

    //m_decisionButton->Draw();

    //// ステージ番号を決める
    //m_ui_StageNumber->Render();

    //for (auto& missionEnemy : m_enemyMissions)
    //{
    //    missionEnemy->Draw();
    //}

    //for (auto& missionTime : m_timeMissions)
    //{
    //    missionTime->Draw();
    //}


    //m_titleLogo->Render();
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

    m_decisionButton->HitMouse();

    //　ファイル生成　処理
    if (m_decisionButton->ClickMouse())
    {

        Stage_Condition testEnemyMission;
        testEnemyMission.condition = "Slime";
        testEnemyMission.value = 1;

        Enemys_Spawn spawn;
        spawn.spawnTime = 2;
        spawn.spawnPos.x = 10;
        spawn.spawnPos.y = 10;
        spawn.type = SLIME;

        Stage_Condition testTimeMission;
        testTimeMission.condition = "True";
        testTimeMission.value = 20;

        Stage_Data stageCondition = m_stageData;
        stageCondition.condition_Enemy.push_back(testEnemyMission);
        stageCondition.enemys_Spawn.push_back(spawn);
        stageCondition.condition_Time.push_back(testTimeMission);


        // 要素0を切る                  
        ZeroCut(stageCondition.condition_Machine);
        ZeroCut(stageCondition.condition_Enemy);  
        ZeroCut(stageCondition.condition_Time);   

        DataManager* pDataM = DataManager::GetInstance();

        pDataM->Initialize();

        stageCondition.resource.crystal = (int)(m_resource_Crystal->GetValue() * (float)pDataM->GetNowCrystal_MAX());
        stageCondition.resource.mp      = (int)(m_resource_MP->GetValue()      * (float)pDataM->GetNowMP_MAX());

        // 書き込み
        Json::WritingJsonFile_StageData(m_stageNum, stageCondition);
    }

}
