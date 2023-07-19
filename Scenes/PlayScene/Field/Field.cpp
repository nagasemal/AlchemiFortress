#include "pch.h"
#include "Field.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#define RAGE 100.f

Field::Field():
	m_hitMouseFlag()
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	m_data.pos		= DirectX::SimpleMath::Vector3(0,-3,0);
	m_data.rage		= DirectX::SimpleMath::Vector3(RAGE,3,RAGE);
}

void Field::Update()
{
	m_hitMouseFlag = false;

	DirectX::SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	// 1.5�͒��x�ǂ���������������
	Circle objectData = Circle(m_data.pos, RAGE * 1.5f);

	// �t�B�[���h�ƃ}�E�X�|�C���^�[�̓����蔻��
	if (PointerToCircle(objectData,mouseWolrdPos)) m_hitMouseFlag = true;

}

void Field::Draw()
{

	ShareData& pSD = ShareData::GetInstance();

	/*===[ �f�o�b�O�����`�� ]===*/
	std::wostringstream oss;
	oss << "Filed-";
	if (m_hitMouseFlag) oss << "Hit";
	pSD.GetDebugFont()->AddString(oss.str().c_str(), DirectX::SimpleMath::Vector2(0.f, 80.f));

}

void Field::Render(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	// ���f�����(�ʒu,�傫��)
	DirectX::SimpleMath::Matrix modelData = DirectX::SimpleMath::Matrix::Identity;
	modelData = DirectX::SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= DirectX::SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());
}

void Field::Finalize()
{
}
