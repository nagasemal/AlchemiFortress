#include "pch.h"
#include "Gauge.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "Scenes/DataManager.h"
#include "SelectionBox.h"

#include <math.h>

#define HP_GAUGE_POS        { 335.0f,50.0f }
#define MP_GAUGE_POS        { 335.0f,75.0f }
#define CRYSTAL_GAUGE_POS   { 335.0f,95.0f}

#define REDUCE_HP 0.1f

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
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    width;
    height;

	//m_gauge = std::make_unique<UserInterface>();
	//m_gauge->Create(device, L"Resources/Textures/Seizou.png",{100,300},{1,1},ANCHOR::MIDDLE_CENTER);

    Add_Hp( HP_GAUGE_POS, { 0.55f,0.35f }, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_MP( MP_GAUGE_POS, { 0.55f,0.25f }, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_Crystal( CRYSTAL_GAUGE_POS, { 0.55f,0.25f }, UserInterface::ANCHOR::MIDDLE_CENTER);

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
    m_difRedioHp -= REDUCE_HP * deltaTime;
    m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

    m_back_Hp->SetRenderRatio(m_difRedioHp);
    m_back_Hp->SetColor({ 0.8f,0.0f,0.0,1.0f });


    //　MPゲージ
    float radio_Mp = (static_cast<float>(pDataM.GetNowMP()) / static_cast<float>(pDataM.GetNowMP_MAX()));
    m_gauge_Mp->SetRenderRatio(radio_Mp);
    m_gauge_Mp->SetColor({0.6f,0.6f,1.0f,1.0f});

    // 徐々に減るMPの処理
    m_difRedioMp -= REDUCE_HP * deltaTime;
    m_difRedioMp = std::min(std::max(m_difRedioMp, radio_Mp), 1.0f);

    m_back_Mp->SetRenderRatio(m_difRedioMp);
    m_back_Mp->SetColor({ 0.4f,0.4f,0.8,1.0f });


    // クリスタルゲージ
    float radio_Crystal = (static_cast<float>(pDataM.GetNowCrystal()) / static_cast<float>(pDataM.GetNowCrystal_MAX()));
    m_gauge_Crystal->SetRenderRatio(radio_Crystal);
    m_gauge_Crystal->SetColor({ 1.0f,0.2f,1.0f,1.0f });

    // 徐々に減るクリスタルの処理
    m_difRedioCrystal -= REDUCE_HP * deltaTime;
    m_difRedioCrystal = std::min(std::max(m_difRedioCrystal, radio_Crystal), 1.0f);

    m_back_Crystal->SetRenderRatio(m_difRedioCrystal);
    m_back_Crystal->SetColor({ 0.7f,0.4f,0.0,1.0f });

}

void Gauge::Render()
{
    m_base_Hp->Render();
    m_back_Hp->Render();
    m_gauge_Hp->Render();

    m_base_Mp->Render();
    m_back_Mp->Render();
    m_gauge_Mp->Render();

    m_base_Crystal->Render();
    m_back_Crystal->Render();
    m_gauge_Crystal->Render();
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

    m_back_Mp = std::make_unique<UserInterface>();
    createGage(m_back_Mp.get());


}
