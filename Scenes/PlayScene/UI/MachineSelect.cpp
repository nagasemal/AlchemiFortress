#include "pch.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"


#define IMAGE_WIGHT		 64
#define IMAGE_HEIGHT	 64

MachineSelect::MachineSelect():
	m_hitMouseFlag(),
	m_selectMachineType(AlchemicalMachineObject::MACHINE_TYPE::NONE)
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{
	m_data.rage = { 64,64 };
}

void MachineSelect::Update()
{
	InputSupport&	pIS = InputSupport::GetInstance();

	bool leftFlag = pIS.GetMouseState().leftButton == Mouse::ButtonStateTracker::PRESSED;

	if (leftFlag)
	{
		m_hitMouseFlag = false;
	}

	if (HitObject(pIS.GetMousePosScreen()) && leftFlag)
	{
		m_hitMouseFlag = true;
	}

}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
}

void MachineSelect::DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::Model* model, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pDR = pSD.GetDeviceResources();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	if(m_hitMouseFlag) colour = DirectX::SimpleMath::Color(0.9f, 0.9f, 0.8f, 0.9f);

	DirectX::SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y};

	// 内部BOX (オブジェクトを配置する箇所)
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(IMAGE_WIGHT / 2, IMAGE_HEIGHT / 2), 1.5f);

	pSB->End();


	// モデル情報(位置,大きさ)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(0.35f, 0.35f, 0.35f);

	modelData *= SimpleMath::Matrix::CreateRotationX(-20);

	// ワールド座標返還
	DirectX::SimpleMath::Vector3 worldPos = CalcScreenToXZN(
		m_data.pos.x,
		m_data.pos.y,
		(int)pDR->GetOutputSize().right,
		(int)pDR->GetOutputSize().bottom,
		view,
		proj);

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(worldPos);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, view, proj);

}

