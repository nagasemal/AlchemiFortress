#include "pch.h"
#include "Crystal.h"

Crystal::Crystal(DirectX::SimpleMath::Vector3 pos, float rotateY)
{

	m_data.pos = pos;
	m_data.rage = DirectX::SimpleMath::Vector3(0.8f, 0.8f, 0.8f);
	m_rotateY = rotateY;

	m_type = CRYSTAL;

}

Crystal::~Crystal()
{
}

void Crystal::Initialize()
{
}

void Crystal::Update()
{
}

void Crystal::Draw()
{
}

void Crystal::Render(Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	// ƒ‚ƒfƒ‹î•ñ(ˆÊ’u,‘å‚«‚³)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= DirectX::SimpleMath::Matrix::CreateRotationY(m_rotateY);
	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

}

bool Crystal::DeleteRequest()
{
	return false;
}

void Crystal::Finalize()
{
}
