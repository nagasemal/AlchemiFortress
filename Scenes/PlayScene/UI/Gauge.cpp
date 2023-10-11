#include "pch.h"
#include "Gauge.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "Scenes/DataManager.h"
#include "SelectionBox.h"

#include <math.h>

#define HP_GAUGE_POS        { 345.0f,50.0f }
#define MP_GAUGE_POS        { 300.0f,100.0f }
#define CRYSTAL_GAUGE_POS   { 300.0f,130.0f}

#define REDUCE_HP 0.1f

Gauge::Gauge():
    m_difRedioHp(1.0f)
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

    Add_Hp( HP_GAUGE_POS, { 0.5f,0.5f }, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_MP( MP_GAUGE_POS, { 0.4f,0.4f }, UserInterface::ANCHOR::MIDDLE_CENTER);
    Add_Crystal( CRYSTAL_GAUGE_POS, { 0.4f,0.4f }, UserInterface::ANCHOR::MIDDLE_CENTER);

}

void Gauge::Update()
{

    DataManager& pDataM = *DataManager::GetInstance();

    // HPゲージ
    float radio_Hp = (static_cast<float>(pDataM.GetNowBaseHP()) / static_cast<float>(pDataM.GetNowBaseHP_MAX()));
    m_gauge_Hp->SetRenderRatio(radio_Hp);
    m_gauge_Hp->SetColor({ 0.5f,1.0f,0.7f,1.0f });

    // 徐々に減るHPの処理
    m_difRedioHp -= REDUCE_HP * DeltaTime::GetInstance().GetDeltaTime();
    m_difRedioHp = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);

    m_back_Hp->SetRenderRatio(m_difRedioHp);
    m_back_Hp->SetColor({ 0.8f,0.0f,0.0,1.0f });


    //　MPゲージ
    float radio_Mp = (static_cast<float>(pDataM.GetNowMP()) / static_cast<float>(pDataM.GetNowMP_MAX()));
    m_gauge_Mp->SetRenderRatio(radio_Mp);
    m_gauge_Mp->SetColor({0.6f,0.6f,1.0f,1.0f});

    // クリスタルゲージ
    float radio_Crystal = (static_cast<float>(pDataM.GetNowCrystal()) / static_cast<float>(pDataM.GetNowCrystal_MAX()));
    m_gauge_Crystal->SetRenderRatio(radio_Crystal);
    m_gauge_Crystal->SetColor({ 1.0f,0.2f,1.0f,1.0f });
}

void Gauge::Render()
{
    m_base_Hp->Render();
    m_back_Hp->Render();
    m_gauge_Hp->Render();

    m_base_Mp->Render();
    m_gauge_Mp->Render();

    m_base_Crystal->Render();
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

    m_gauge_Crystal = std::make_unique<UserInterface>();
    m_gauge_Crystal->Create(device
        , L"Resources/Textures/Gauge_main.png"
        , position
        , scale
        , anchor);
    m_gauge_Crystal->SetWindowSize(width, height);

    m_base_Crystal = std::make_unique<UserInterface>();
    m_base_Crystal->Create(device
        , L"Resources/Textures/Gauge_base.png"
        , position
        , scale
        , anchor);
    m_base_Crystal->SetWindowSize(width, height);

}

void Gauge::Add_Hp(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    int width = device->GetOutputSize().right;
    int height = device->GetOutputSize().bottom;

    m_gauge_Hp = std::make_unique<UserInterface>();
    m_gauge_Hp->Create(device
        , L"Resources/Textures/Gauge_main.png"
        , position
        , scale
        , anchor);
    m_gauge_Hp->SetWindowSize(width, height);

    m_base_Hp = std::make_unique<UserInterface>();
    m_base_Hp->Create(device
        , L"Resources/Textures/Gauge_base.png"
        , position
        , scale
        , anchor);
    m_base_Hp->SetWindowSize(width, height);

    // 背後に描画する赤いバー(徐々に減衰)
    m_back_Hp = std::make_unique<UserInterface>();
    m_back_Hp->Create(device
        , L"Resources/Textures/Gauge_main.png"
        , position
        , scale
        , anchor);
    m_back_Hp->SetWindowSize(width, height);

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

    m_gauge_Mp = std::make_unique<UserInterface>();
    m_gauge_Mp->Create(device
        , L"Resources/Textures/Gauge_main.png"
        , position
        , scale
        , anchor);
    m_gauge_Mp->SetWindowSize(width, height);

    m_base_Mp = std::make_unique<UserInterface>();
    m_base_Mp->Create(device
        , L"Resources/Textures/Gauge_base.png"
        , position
        , scale
        , anchor);
    m_base_Mp->SetWindowSize(width, height);

}
