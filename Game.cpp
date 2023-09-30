//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SoundData.h"

#include <WICTextureLoader.h>


extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);

    DeltaTime::Create();
    InputSupport::Create();
    ShareData::Create();
    SpriteLoder::Create();
    ShareJsonData::Create();
    SoundData::Create();
}
Game::~Game()
{

    DeltaTime::Destroy();
    InputSupport::Destroy();
    ShareData::Destroy();

    SpriteLoder& pSL = SpriteLoder::GetInstance();
    pSL.Finalize();

    SpriteLoder::Destroy();

    ShareJsonData::Destroy();

    SoundData::Destroy();

    m_SceneManager.get()->Finalize();
    m_SceneManager.reset();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    SpriteLoder& pSL = SpriteLoder::GetInstance();
    pSL.Loading();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    // オーディオエンジンの作成
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    SoundData& pSound = SoundData::GetInstance();

    pSound.SoundLoad(m_audioEngine.get());

}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    DeltaTime* pDeltaT = &DeltaTime::GetInstance();
    InputSupport* pIS = &InputSupport::GetInstance();

    float elapsedTime = float(timer.GetElapsedSeconds());
    pDeltaT->SetDeltaTime(elapsedTime);

    m_SceneManager.get()->Update(timer);

    //===[ 更新処理 ]===//
        //->　キーボード
    auto keyboardState = Keyboard::Get().GetState();
    m_keyboardTracker.Update(keyboardState);
    pIS->SetKeybordState(m_keyboardTracker);

        //->　マウス
    auto mouseState = Mouse::Get().GetState();
    m_mouseTracker.Update(mouseState);
    pIS->SetMouseState(m_mouseTracker);
    // スクリーン座標からワールド空間座標への返還を常に行う
    pIS->Update();

    // オーディオエンジンの更新
    if (!m_audioEngine->Update())
    {
        // No audio device is active
        if (m_audioEngine->IsCriticalError())
        {
            OutputDebugString(L"AudioEngine Error!\n");
        }
    }


}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    SimpleMath::Matrix projection, view;

    // 射影行列の取得
    projection = mCamera->GetProjectionMatrix();

    // ビュー行列の取得
    view = mCamera->GetViewMatrix();

    // TODO: Add your rendering code here.
    context;

    /*===[ デバッグ文字描画 ]===*/
    std::wostringstream oss;

    oss << "fps:" << m_timer.GetFramesPerSecond();

    m_debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 0.f));

    m_SceneManager.get()->Render();
    m_debugFont->Render(m_commonStates.get());

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{

    auto device  = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    m_commonStates = std::make_unique<CommonStates>(device);
    m_SpriteBatch = std::make_unique<SpriteBatch>(context);
    m_debugFont = std::make_unique<Imase::DebugFont>(device,context,L"Resources/Font/SegoeUI_18.spritefont");
    m_SpriteBatch = std::make_unique<SpriteBatch>(context);
    m_EffectFactory = std::make_unique<DirectX::EffectFactory>(device);

    ShareData* pSD = &ShareData::GetInstance();

    pSD->SetDebugFont(m_debugFont.get());
    pSD->SetCommonStates(m_commonStates.get());
    pSD->SetDeviceResources(m_deviceResources.get()); 
    pSD->SetSpriteBatch(m_SpriteBatch.get());
    pSD->SetEffectFactory(m_EffectFactory.get());

    m_SceneManager = std::make_unique<SceneManager>();
    m_SceneManager.get()->Initialize();

    mCamera = std::make_unique<Camera>();

    pSD->SetCamera(mCamera.get());

    pSD->CreateStencilData();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
