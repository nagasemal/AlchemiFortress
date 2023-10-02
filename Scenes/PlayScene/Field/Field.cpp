#include "pch.h"
#include "Field.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#define RAGE 60.f

Field::Field():
	m_hitMouseFlag()
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	m_data.pos		= SimpleMath::Vector3(0,-3,0);
	m_data.rage		= SimpleMath::Vector3(RAGE,3,RAGE);
}

void Field::Update()
{
	m_hitMouseFlag = false;

	SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

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
	pSD.GetDebugFont()->AddString(oss.str().c_str(), SimpleMath::Vector2(0.f, 80.f));

}

void Field::Render(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	// ���f�����(�ʒu,�傫��)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false,[&](){});
}

void Field::Finalize()
{
}
