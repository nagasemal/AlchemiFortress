#include "pch.h"
#include "Gauge.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/DataManager.h"
#include "SelectionBox.h"

#include <math.h>

#define REDUCE 0.05f

Gauge::Gauge():
    m_difRedioHp(1.0f),
    m_difRedioMp(0.0f),
    m_difRedioCrystal(0.0f)
{
}

Gauge::~Gauge()
{
}

void Gauge::Initialize()
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    device;

    ShareJsonData& pSJD = ShareJsonData::GetInstance();
    UI_Data uiData = UI_Data();

    //　====================[　HPゲージ生成　]
    uiData = pSJD.GetUIData("GaugeHP");
    Add_Hp(uiData.pos, uiData.rage, UserInterface::ANCHOR::MIDDLE_CENTER);
    //　　|=>　所持数描画の位置を設定
    SimpleMath::Vector2 numberPos = 
        uiData.pos + SimpleMath::Vector2(
        m_base_Hp->GetTexture_W() / 2.2f * uiData.rage.x,
        m_base_Hp->GetTexture_H() / 2.2f * uiData.rage.y);

    m_resourceRenderHP = std::make_unique<Number>(numberPos, SimpleMath::Vector2(uiData.option["NUMBER_RAGE"]));
    m_resourceRenderHP->SetColor(SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));


    //　====================[　MPゲージ生成　]
    uiData = pSJD.GetUIData("GaugeMP");
    Add_MP(uiData.pos, uiData.rage, UserInterface::ANCHOR::MIDDLE_CENTER);

    //　　|=>　所持数描画の位置を設定
    numberPos =
        uiData.pos + SimpleMath::Vector2(
            m_base_Mp->GetTexture_W() / 2.2f * uiData.rage.x,
            m_base_Mp->GetTexture_H() / 2.2f * uiData.rage.y);

    m_resourceRenderMP = std::make_unique<Number>(numberPos, SimpleMath::Vector2(uiData.option["NUMBER_RAGE"]));
    m_resourceRenderMP->SetColor(SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

    //　====================[　Crystalゲージ生成　]
    uiData = pSJD.GetUIData("GaugeCrystal");
    Add_Crystal(uiData.pos, uiData.rage, UserInterface::ANCHOR::MIDDLE_CENTER);

    //　　|=>　所持数描画の位置を設定
    numberPos =
        uiData.pos + SimpleMath::Vector2(
            m_base_Crystal->GetTexture_W() / 2.2f * uiData.rage.x,
            m_base_Crystal->GetTexture_H() / 2.2f * uiData.rage.y);

    m_resourceRenderCrystal = std::make_unique<Number>(numberPos, SimpleMath::Vector2(uiData.option["NUMBER_RAGE"]));
    m_resourceRenderCrystal->SetColor(SimpleMath::Color(1.0f,1.0f,1.0f,1.0f));
}

void Gauge::Update()
{

    DataManager& pDataM = *DataManager::GetInstance();

    float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

    //　====================[　HPゲージの処理　]
    //　　|=>　割合
    float radio_Hp = (static_cast<float>(pDataM.GetNowBaseHP()) / static_cast<float>(pDataM.GetNowBaseHP_MAX()));
    m_gauge_Hp->SetRenderRatio(radio_Hp);

    //　　|=> 徐々に減るHPの処理
    m_difRedioHp -= REDUCE * deltaTime;
    m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);
    m_back_Hp->SetRenderRatio(m_difRedioHp);

    //　====================[　MPゲージの処理　]
    //　　|=>　割合
    float radio_Mp = (static_cast<float>(pDataM.GetNowMP()) / static_cast<float>(pDataM.GetNowMP_MAX()));
    m_gauge_Mp->SetRenderRatio(radio_Mp);

    //　　|=>　徐々に減るMPの処理
    m_difRedioMp -= REDUCE * deltaTime;
    m_difRedioMp = std::min(std::max(m_difRedioMp, radio_Mp), 1.0f);

    m_back_Mp->SetRenderRatio(m_difRedioMp);



    // クリスタルゲージ
    float radio_Crystal = (static_cast<float>(pDataM.GetNowCrystal()) / static_cast<float>(pDataM.GetNowCrystal_MAX()));
    m_gauge_Crystal->SetRenderRatio(radio_Crystal);

    // 徐々に減るクリスタルの処理
    m_difRedioCrystal -= REDUCE * deltaTime;
    m_difRedioCrystal = std::min(std::max(m_difRedioCrystal, radio_Crystal), 1.0f);

    m_back_Crystal->SetRenderRatio(m_difRedioCrystal);


}

void Gauge::Render()
{
    DataManager& pDataM = *DataManager::GetInstance();

    //　====================[　耐久値描画　]
    m_base_Hp   ->Render();
    m_back_Hp   ->Render();
    m_gauge_Hp  ->Render();

    //　　|=>　耐久値数字描画
    m_resourceRenderHP->SetNumber(pDataM.GetNowBaseHP());
    m_resourceRenderHP->Render();

    //　====================[　魔力リソース描画　]
    m_base_Mp   ->Render();
    m_back_Mp   ->Render();
    m_gauge_Mp  ->Render();

    //　　|=>　リソース数字描画
    m_resourceRenderMP->SetNumber(pDataM.GetNowMP());
    m_resourceRenderMP->Render();

    //　====================[　結晶リソース描画　]
    m_base_Crystal  ->Render();
    m_back_Crystal  ->Render();
    m_gauge_Crystal ->Render();

    //　　|=>　結晶数字描画
    m_resourceRenderCrystal->SetNumber(pDataM.GetNowCrystal());
    m_resourceRenderCrystal->Render();

    ShareData& pSD = ShareData::GetInstance();
    SpriteBatch* pSB = pSD.GetSpriteBatch();
    SpriteLoder& pSL = SpriteLoder::GetInstance();
    ShareJsonData& pSJD = ShareJsonData::GetInstance();
    UI_Data uiData = UI_Data();

    pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

    //　====================[　魔力アイコンの描画　]
    uiData = pSJD.GetUIData("GaugeMP");
    RECT rect = SpriteCutter(64, 64, 0, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), uiData.pos - SimpleMath::Vector2((m_gauge_Mp->GetTexture_W() / 2) * uiData.rage.x, 0.0f),
        &rect, Colors::White, 0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f), uiData.option["NUMBER_RAGE"]);

    //　====================[　結晶アイコンの描画　]
    uiData = pSJD.GetUIData("GaugeCrystal");
    rect = SpriteCutter(64, 64, 1, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), uiData.pos - SimpleMath::Vector2((m_gauge_Crystal->GetTexture_W() / 2) * uiData.rage.x,0.0f),
        &rect, Colors::White, 0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f), uiData.option["NUMBER_RAGE"]);

    pSB->End();

}

void Gauge::Finalize()
{
}

void Gauge::Add_Crystal(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    auto createGage = [&](UserInterface* object, bool base = false) {

        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);

        object->SetWindowSize(width, height);

    };

    m_gauge_Crystal = std::make_unique<UserInterface>();
    createGage(m_gauge_Crystal.get());
    m_gauge_Crystal->SetColor(Colors::MediumPurple);

    m_base_Crystal = std::make_unique<UserInterface>();
    createGage(m_base_Crystal.get(),true);
    m_base_Crystal->SetColor(Colors::IndianRed);

    m_back_Crystal = std::make_unique<UserInterface>();
    createGage(m_back_Crystal.get());
    m_back_Crystal->SetColor(Colors::IndianRed);

}

void Gauge::Add_Hp(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    auto createGage = [&](UserInterface* object,bool base = false){
        
        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);

        object->SetWindowSize(width, height);

    };

    m_gauge_Hp = std::make_unique<UserInterface>();
    createGage(m_gauge_Hp.get());
    m_gauge_Hp->SetColor(Colors::PaleGreen);

    m_base_Hp = std::make_unique<UserInterface>();
    createGage(m_base_Hp.get(),true);
    m_base_Hp->SetColor(Colors::PaleGreen);

    // 背後に描画する赤いバー(徐々に減衰)
    m_back_Hp = std::make_unique<UserInterface>();
    createGage(m_back_Hp.get());
    m_back_Hp->SetColor(Colors::DarkRed);

}

SimpleMath::Vector2 Gauge::GetHPGaugePos()
{
    return m_gauge_Hp->GetPosition();
}

SimpleMath::Vector2 Gauge::GetMPGaugePos()
{
    return m_gauge_Mp->GetPosition();
}

SimpleMath::Vector2 Gauge::GetCrystalGaugePos()
{
    return m_gauge_Crystal->GetPosition();
}

void Gauge::Add_MP(SimpleMath::Vector2 position,SimpleMath::Vector2 scale,UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    int width  = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    auto createGage = [&](UserInterface* object, bool base = false) {

        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);

        object->SetWindowSize(width, height);

    };

    m_gauge_Mp = std::make_unique<UserInterface>();
    createGage(m_gauge_Mp.get());
    m_gauge_Mp->SetColor(Colors::RoyalBlue);

    m_base_Mp = std::make_unique<UserInterface>();
    createGage(m_base_Mp.get(), true);
    m_base_Mp->SetColor(Colors::DarkCyan);

    m_back_Mp = std::make_unique<UserInterface>();
    createGage(m_back_Mp.get());
    m_back_Mp->SetColor(Colors::DarkCyan);

}
