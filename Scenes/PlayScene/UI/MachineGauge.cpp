#include "pch.h"
#include "MachineGauge.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

MachineGauge::MachineGauge()
{
}

MachineGauge::~MachineGauge()
{
}

//void MachineGauge::AddHPGauge(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
//{
    //auto device = ShareData::GetInstance().GetDeviceResources();
    //int width  = device->GetOutputSize().right;
    //int height = device->GetOutputSize().bottom;
    //
    //m_gauge_Hp = std::make_unique<UserInterface>();
    //m_gauge_Hp->Create(device
    //    , L"Resources/Textures/Gauge_main.png"
    //    , position
    //    , scale
    //    , anchor);
    //m_gauge_Hp->SetWindowSize(width, height);
    //
    //m_base_Hp = std::make_unique<UserInterface>();
    //m_base_Hp->Create(device
    //    , L"Resources/Textures/Gauge_base.png"
    //    , position
    //    , scale
    //    , anchor);
    //m_base_Hp->SetWindowSize(width, height);
//}

void MachineGauge::Update(int hp, int maxHp)
{

    // HPÉQÅ[ÉW
    float radio_Hp = (static_cast<float>(hp) / static_cast<float>(maxHp));
    //m_gauge_Hp->SetRenderRatio(radio_Hp);
    //m_gauge_Hp->SetColor({ 0.9f,0.6f,0.7f,1.0f });

}

void MachineGauge::Render(AlchemicalMachineObject* gameObject)
{

    float radio_Hp = (static_cast<float>(gameObject->GetHP()) / static_cast<float>(gameObject->GetMAXHP()));

    ShareData& pSD = ShareData::GetInstance();
    SpriteLoder::TextureData baseTexData = SpriteLoder::GetInstance().GetBaseGage();

    SpriteLoder::TextureData mainTexData = SpriteLoder::GetInstance().GetMainGage();

    auto pSB = pSD.GetSpriteBatch();

    RECT rect = { 0,0,baseTexData.width,baseTexData.height };

    pSB->Draw(baseTexData.tex.Get(),
        gameObject->GetPos(),
        &rect,
        Colors::White,
        0.0f,
        SimpleMath::Vector2(static_cast<float>(baseTexData.width / 2),
                            static_cast<float>(baseTexData.height / 2)),
        0.4f);


    //rect = { 0,0,mainTexData.width * radio_Hp,mainTexData.height };

    //pSB->Draw(mainTexData.tex.Get(), 
    //    gameObject->GetPos(),
    //    &rect,
    //    Colors::HotPink,
    //    0.0f,
    //    SimpleMath::Vector2(mainTexData.width / 2, mainTexData.height / 2),
    //    0.4f);

}
