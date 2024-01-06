#include "pch.h"
#include "ClearVeil.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"

ClearVeil::ClearVeil()
{
	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	//　====================[　Waveクリア時演出クラスの設定　]
	//　　|=>　上部
	m_nextWave_Top = std::make_unique<UserInterface>();
	m_nextWave_Top->Create(device,
		L"Resources/Textures/WaveClear_Top.png",
		SimpleMath::Vector2(width / 2.0f, 0.0f),
		SimpleMath::Vector2(1.0f, 0.5f),
		UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWave_Top->SetColor(Colors::Aqua);
	//　　|=>　下部
	m_nextWave_Bottom = std::make_unique<UserInterface>();
	m_nextWave_Bottom->Create(device,
		L"Resources/Textures/WaveClear_Bottom.png",
		SimpleMath::Vector2(width / 2.0f, height),
		SimpleMath::Vector2(1.0f, 0.5f),
		UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWave_Bottom->SetColor(Colors::Aqua);
	m_nextWave_Bottom->SetSharpenW(true);

	m_waveAnimation = 0.0f;
}

ClearVeil::~ClearVeil()
{
}

void ClearVeil::Update(bool startFlag)
{

	if (startFlag)
	{
		m_waveAnimation += DeltaTime::GetInstance().GetDeltaTime() * 0.5f;

		m_nextWave_Top->SetSharpenW(true);
		m_nextWave_Bottom->SetSharpenW(false);

	}
	else
	{
		m_waveAnimation -= DeltaTime::GetInstance().GetDeltaTime();

		m_nextWave_Top->SetSharpenW(false);
		m_nextWave_Bottom->SetSharpenW(true);
	}

	//　====================[　表示割合を設定　]
	m_nextWave_Top		->SetRenderRatio(m_waveAnimation);
	m_nextWave_Bottom	->SetRenderRatio(m_waveAnimation);

}

void ClearVeil::Render()
{
	//　====================[　描画　]
	m_nextWave_Top->Render();
	m_nextWave_Bottom->Render();
}

bool ClearVeil::NextWave()
{
	return m_waveAnimation.MaxCheck();
}
