#include "pch.h"
#include "MachineSelect.h"
#include "NecromaLib/Singleton/ShareData.h"

MachineSelect::MachineSelect()
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{

	m_data.pos = { 100,100 };
	m_data.rage = { 5,5 };

}

void MachineSelect::Update()
{
}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
}

void MachineSelect::DisplayObject(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::Model* model)
{
	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// �摜�̃T�C�Y
	RECT srcRect = { 0, 0, 64, 64 };

	// ���O�̐F
	DirectX::SimpleMath::Color colour = DirectX::SimpleMath::Color(0.8f, 0.8f, 0.8f, 0.8f);

	DirectX::SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y};

	// ����BOX (�I�u�W�F�N�g��z�u����ӏ�)
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, XMFLOAT2(), m_data.rage.x / 3.f);

	pSB->End();



}

