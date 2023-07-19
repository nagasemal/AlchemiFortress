#include "pch.h"
#include "Crystal.h"

#include "NecromaLib/Singleton/InputSupport.h"

Crystal::Crystal(DirectX::SimpleMath::Vector3 pos, float rotateY)
{

	m_data.pos = pos;
	m_data.rage = DirectX::SimpleMath::Vector3(0.8f, 0.8f, 0.8f);
	m_rotateY = rotateY;

	m_type = CRYSTAL;

	m_selectFlag = false;

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

bool Crystal::GetHitMouse()
{
	DirectX::SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	Circle mouseCircle(mouseWolrdPos, 1.2f);

	InputSupport& pINP = InputSupport::GetInstance();
	auto mouse = pINP.GetMouseState();
	bool leftRelease = mouse.leftButton == mouse.RELEASED;
	bool hitFlag = CircleCollider(GetCircle(), mouseCircle);

	if (leftRelease)
	{

		if (!hitFlag && m_selectFlag)
		{
			m_selectFlag = false;
		}
		else if (hitFlag && !m_selectFlag)
		{
			m_selectFlag = true;
		}
	}

	return 	m_selectFlag;
}

void Crystal::Finalize()
{
}
