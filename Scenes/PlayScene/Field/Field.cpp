#include "pch.h"
#include "Field.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"

#define RAGE 20.f

Field::Field():
	m_hitMouseFlag()
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	m_Model = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Filed.cmo", *pSD.GetEffectFactory());

	m_data.pos		= DirectX::SimpleMath::Vector3(0,-3,0);
	m_data.rage		= DirectX::SimpleMath::Vector3(RAGE,3,RAGE);
}

void Field::Update()
{
	m_hitMouseFlag = false;

	DirectX::SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	// 1.5は丁度良い数字だったため
	Circle objectData = Circle(m_data.pos, RAGE * 1.5f);

	// フィールドとマウスポインターの当たり判定
	if (PointerToCircle(objectData,mouseWolrdPos)) m_hitMouseFlag = true;

}

void Field::Draw()
{

	ShareData& pSD = ShareData::GetInstance();

	/*===[ デバッグ文字描画 ]===*/
	std::wostringstream oss;
	oss << "Filed-";
	if (m_hitMouseFlag) oss << "Hit";
	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 80.f));

	// モデル情報(位置,大きさ)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);

	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	m_Model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

}

void Field::Finalize()
{
	m_Model.reset();
}
