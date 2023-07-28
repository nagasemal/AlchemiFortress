#include "pch.h"
#include "TitileCall.h"
#include "NecromaLib/Singleton/DeltaTime.h"

// �~���̈�ԏ������ꍇ�ɂ�����}�V���̍ő吔
#define CIRCLE_MAX_MIN  4

// �~���̍ő僉�C��
#define CIRCLE_MAX_LINE 6

// ���C�����m�̊Ԋu
#define CIRCLE_LINE_DISTANCE 5.5f

TitleCall::TitleCall()
{
}

TitleCall::~TitleCall()
{
}

void TitleCall::Initialize()
{

	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	SettingAMMachine();

}

void TitleCall::Update()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{

		MovingMachine(i);

	}
}

void TitleCall::Render()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{
		// ���f���̕`��			�I�u�W�F�N�g�Ɋ��蓖�Ă�ꂽID�����ƂɃ��f���z�񂩂�f�[�^�����o��
		m_AMobjects[i]->ModelRender(
			m_AMFilter->HandOverAMModel(m_AMobjects[i]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMobjects[i]->GetModelID()));

		m_AMobjects[i]->Draw();
	}
}

void TitleCall::Finalize()
{
}

void TitleCall::MovingMachine(int number)
{

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0�𒆐S�ɉ�]�ړ�
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.1f)) * deltaTime;

	// ��]��̍��W����
	m_AMobjects[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMobjects[number]->GetPos(), matrix));

}

void TitleCall::SettingAMMachine()
{
	int counter = 0;

	for (int i = 1; i < CIRCLE_MAX_LINE; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{
			// �}�V����ǉ�
			m_AMobjects.push_back(m_AMFilter->HandOverAMClass(MACHINE_TYPE(i)));

			// ����������
			m_AMobjects[counter]->Initialize();

			// ���݂��郉�C������
			m_AMobjects[counter]->SetLine(i);
			m_AMobjects[counter]->SummonAM(SetVelocityCircle(j, CIRCLE_MAX_MIN * i, i * CIRCLE_LINE_DISTANCE));
			counter++;
		}
	}
}
