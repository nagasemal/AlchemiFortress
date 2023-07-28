#include "pch.h"
#include "MachineGauge.h"
#include "NecromaLib/Singleton/ShareData.h"

MachineGauge::MachineGauge()
{
}

MachineGauge::~MachineGauge()
{
}

void MachineGauge::AddHPGauge(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    int width  = device->GetOutputSize().right;
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
}

void MachineGauge::Update(int hp, int maxHp)
{

    // HPÉQÅ[ÉW
    float radio_Hp = (static_cast<float>(hp) / static_cast<float>(maxHp));
    m_gauge_Hp->SetRenderRatio(radio_Hp);
    m_gauge_Hp->SetColor({ 0.9f,0.6f,0.7f,1.0f });

}

void MachineGauge::Render()
{
    m_base_Hp->Render();
    m_gauge_Hp->Render();
}
