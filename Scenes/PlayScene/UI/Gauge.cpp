#include "pch.h"
#include "Gauge.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/DataManager.h"
#include "SelectionBox.h"

#include <math.h>

#define OFFSET_POS              SimpleMath::Vector2(100.0f,320.0f)

#define HP_GAUGE_POS            SimpleMath::Vector2(630.0f,360.0f) - OFFSET_POS
#define MP_GAUGE_POS            SimpleMath::Vector2(450.0f,400.0f) - OFFSET_POS
#define CRYSTAL_GAUGE_POS       SimpleMath::Vector2(810.0f,400.0f) - OFFSET_POS

#define BIG_GAUGE_RAGE          SimpleMath::Vector2(0.8f,0.2f)
#define SMALL_GAUGE_RAGE        SimpleMath::Vector2(0.4f,0.2f)

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

    Add_Hp( HP_GAUGE_POS, BIG_GAUGE_RAGE, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_MP( MP_GAUGE_POS, SMALL_GAUGE_RAGE, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_Crystal( CRYSTAL_GAUGE_POS, SMALL_GAUGE_RAGE, UserInterface::ANCHOR::MIDDLE_CENTER);


    m_resourceRenderHP = std::make_unique<Number>(HP_GAUGE_POS, BIG_GAUGE_RAGE);
    m_resourceRenderHP->SetColor(SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

    m_resourceRenderMP = std::make_unique<Number>(MP_GAUGE_POS, SMALL_GAUGE_RAGE);
    m_resourceRenderMP->SetColor(SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

    m_resourceRenderCrystal = std::make_unique<Number>(CRYSTAL_GAUGE_POS, SMALL_GAUGE_RAGE);
    m_resourceRenderCrystal->SetColor(SimpleMath::Color(1.0f,1.0f,1.0f,1.0f));
}

void Gauge::Update()
{

    DataManager& pDataM = *DataManager::GetInstance();

    float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

    // HPゲージ
    float radio_Hp = (static_cast<float>(pDataM.GetNowBaseHP()) / static_cast<float>(pDataM.GetNowBaseHP_MAX()));
    m_gauge_Hp->SetRenderRatio(radio_Hp);
    m_gauge_Hp->SetColor({ 0.5f,1.0f,0.7f,1.0f });

    // 徐々に減るHPの処理
    m_difRedioHp -= REDUCE * deltaTime;
    m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

    m_back_Hp->SetRenderRatio(m_difRedioHp);
    m_back_Hp->SetColor({ 0.8f,0.0f,0.0,1.0f });


    //　MPゲージ
    float radio_Mp = (static_cast<float>(pDataM.GetNowMP()) / static_cast<float>(pDataM.GetNowMP_MAX()));
    m_gauge_Mp->SetRenderRatio(radio_Mp);
    m_gauge_Mp->SetColor({0.6f,0.6f,1.0f,1.0f});

    // 徐々に減るMPの処理
    m_difRedioMp -= REDUCE * deltaTime;
    m_difRedioMp = std::min(std::max(m_difRedioMp, radio_Mp), 1.0f);

    m_back_Mp->SetRenderRatio(m_difRedioMp);
    m_back_Mp->SetColor({ 0.4f,0.4f,0.8,1.0f });


    // クリスタルゲージ
    float radio_Crystal = (static_cast<float>(pDataM.GetNowCrystal()) / static_cast<float>(pDataM.GetNowCrystal_MAX()));
    m_gauge_Crystal->SetRenderRatio(radio_Crystal);
    m_gauge_Crystal->SetColor({ 1.0f,0.2f,1.0f,1.0f });

    // 徐々に減るクリスタルの処理
    m_difRedioCrystal -= REDUCE * deltaTime;
    m_difRedioCrystal = std::min(std::max(m_difRedioCrystal, radio_Crystal), 1.0f);

    m_back_Crystal->SetRenderRatio(m_difRedioCrystal);
    m_back_Crystal->SetColor({ 0.7f,0.4f,0.0,1.0f });

}

void Gauge::Render()
{
    DataManager& pDataM = *DataManager::GetInstance();

    // 耐久値描画
    m_base_Hp   ->Render();
    m_back_Hp   ->Render();
    m_gauge_Hp  ->Render();

    m_resourceRenderHP->SetNumber(pDataM.GetNowBaseHP());
    m_resourceRenderHP->SetRage(SimpleMath::Vector2(0.5f,0.5f));
    m_resourceRenderHP->SetPosition(HP_GAUGE_POS + SimpleMath::Vector2(m_base_Hp->GetTexture_W() / 2.2f * BIG_GAUGE_RAGE.x,
                                                                     m_base_Hp->GetTexture_H() / 2.2f * BIG_GAUGE_RAGE.y));
    m_resourceRenderHP->Render();

    // 魔力リソース量描画
    m_base_Mp   ->Render();
    m_back_Mp   ->Render();
    m_gauge_Mp  ->Render();

    m_resourceRenderMP->SetNumber(pDataM.GetNowMP());
    m_resourceRenderMP->SetRage(SimpleMath::Vector2(0.4f, 0.4f));
    m_resourceRenderMP->SetPosition(MP_GAUGE_POS + SimpleMath::Vector2(m_base_Mp->GetTexture_W() / 2.2f * SMALL_GAUGE_RAGE.x,
                                                                     m_base_Mp->GetTexture_H() / 2.2f * SMALL_GAUGE_RAGE.y));
    m_resourceRenderMP->Render();


    // 結晶リソース量描画
    m_base_Crystal  ->Render();
    m_back_Crystal  ->Render();
    m_gauge_Crystal ->Render();

    m_resourceRenderCrystal->SetNumber(pDataM.GetNowCrystal());
    m_resourceRenderCrystal->SetRage(SimpleMath::Vector2(0.4f, 0.4f));
    m_resourceRenderCrystal->SetPosition(CRYSTAL_GAUGE_POS + SimpleMath::Vector2(m_base_Crystal->GetTexture_W() / 2.2f * SMALL_GAUGE_RAGE.x,
                                                                          m_base_Crystal->GetTexture_H() / 2.2f * SMALL_GAUGE_RAGE.y));
    m_resourceRenderCrystal->Render();

    ShareData& pSD = ShareData::GetInstance();
    SpriteBatch* pSB = pSD.GetSpriteBatch();
    SpriteLoder& pSL = SpriteLoder::GetInstance();

    pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

    // 魔力アイコンを描画する
    RECT rect = SpriteCutter(64, 64, 0, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), MP_GAUGE_POS - SimpleMath::Vector2((m_gauge_Mp->GetTexture_W() / 2) * SMALL_GAUGE_RAGE.x, 0.0f),
        &rect, Colors::White, 0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f), 0.5f);

    // 結晶アイコンを描画する
    rect = SpriteCutter(64, 64, 1, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), CRYSTAL_GAUGE_POS - SimpleMath::Vector2((m_gauge_Crystal->GetTexture_W() / 2) * SMALL_GAUGE_RAGE.x,0.0f),
        &rect, Colors::White, 0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f), 0.5f);

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

    m_base_Crystal = std::make_unique<UserInterface>();
    createGage(m_base_Crystal.get(),true);

    m_base_Crystal->SetColor({ 0.7f,0.4f,0.0,1.0f });

    m_back_Crystal = std::make_unique<UserInterface>();
    createGage(m_back_Crystal.get());

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

    m_base_Hp = std::make_unique<UserInterface>();
    createGage(m_base_Hp.get(),true);
    m_base_Hp->SetColor({ 0.5f,1.0f,0.7f,1.0f });

    // 背後に描画する赤いバー(徐々に減衰)
    m_back_Hp = std::make_unique<UserInterface>();
    createGage(m_back_Hp.get());

}

SimpleMath::Vector2 Gauge::GetHPGaugePos()
{
    return HP_GAUGE_POS;
}

SimpleMath::Vector2 Gauge::GetMPGaugePos()
{
    return MP_GAUGE_POS;
}

SimpleMath::Vector2 Gauge::GetCrystalGaugePos()
{
    return CRYSTAL_GAUGE_POS;
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

    m_base_Mp = std::make_unique<UserInterface>();
    createGage(m_base_Mp.get(), true);
    m_base_Mp->SetColor({ 0.4f,0.4f,0.8,1.0f });

    m_back_Mp = std::make_unique<UserInterface>();
    createGage(m_back_Mp.get());


}
