#include "pch.h"
#include "MachineSelectManager.h"
#include "Scenes/DataManager.h"

#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/PlayScene/UI/DisplayMagicCircle.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#define POS 		SimpleMath::Vector2(1200, 640)
#define DIRECTION	120.0f

#define MACHINE_NAME_POS_SHIFT SimpleMath::Vector2(20, 60)

MachineSelectManager::MachineSelectManager() :
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_selectBoxAll(false),
	m_manufacturingFlag(false),
	m_selectNoneFlag(false),
	m_forwardOrderFlag(false),
	m_rotateTime(),
	m_selectNumber(1),
	m_cursorMachineNumber()
{
}

MachineSelectManager::~MachineSelectManager()
{
}

void MachineSelectManager::TextuerLoader()
{
}

void MachineSelectManager::Initialize()
{

	float deg = 360.0 / 5;
	float radians = (deg * XM_PI) / 360.0f;

	// None���Ȃ����߂�1�X�^�[�g
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i] = std::make_unique<MachineSelect>();

		m_machineSelect[i]->SetMachineType((MACHINE_TYPE)i);
		
		m_machineSelect[i]->SetPosition({ POS.x + cosf((radians * 3) * i) * DIRECTION ,
										  POS.y + sinf((radians * 3) * i) * DIRECTION });
		m_machineSelect[i]->Initialize();

		m_machineSelect[i]->GetMachineBox()->SetSavePos(m_machineSelect[i]->GetPos());

	}

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_displayMagicCircle = std::make_unique<DisplayMagicCircle>();

	// �}�V���̖��O�`��
	m_machineName = std::make_unique<SelectionBox>(SimpleMath::Vector2(), SimpleMath::Vector2(1.0f, 1.0f));
	m_machineName->Initialize();

	// �B���{�^���̕`��
	m_selectionAlchemi = std::make_unique<SelectionBox>(POS, SimpleMath::Vector2(1.3f,1.3f));
	m_selectionAlchemi->Initialize();
	m_selectionAlchemi->SetLayer(2);

	m_selectionLeft = std::make_unique<SelectionBox>(POS - SimpleMath::Vector2(100, -40), SimpleMath::Vector2(1.4f, 1.75f));
	m_selectionLeft->Initialize();
	m_selectionLeft->SetLayer(1);

	m_selectionRight = std::make_unique<SelectionBox>(POS - SimpleMath::Vector2(-40,100), SimpleMath::Vector2(1.75f, 1.4f));
	m_selectionRight->Initialize();
	m_selectionRight->SetLayer(1);

	m_machineNumRender = std::make_unique<Number>(POS, SimpleMath::Vector2(1.0f, 1.0f));

}

void MachineSelectManager::Update(FieldObjectManager* fieldObjectManager)
{

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_selectMachineType = MACHINE_TYPE::NONE;

	auto datas = DataManager::GetInstance();
	// json����ǂݎ�����l���g�p����
	auto pSJD = &ShareJsonData::GetInstance();

	auto pINP = &InputSupport::GetInstance();

	m_selectionAlchemi	->HitMouse(true);

	m_selectionLeft		->HitMouse(true);
	m_selectionRight	->HitMouse(true);

	//// None���Ȃ����߂�1�X�^�[�g
	//for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	//{
	//	// �v�f�̍X�V����
	//	m_machineSelect[i]->Update();
	//
	//	// �G��Ă��镨�̏�������`�悷��
	//	if (m_machineSelect[i]->GetHitMouseFlag())
	//	{
	//		m_selectNumber = i;
	//	}
	//
	//	// �v�f���琻���{�^���������ꂽ������󂯎�� ���\�[�X������Ȃ��ꍇ�͒e��
	//	m_selectionAlchemi->SetActiveFlag(
	//		datas->GetNowMP() - pSJD->GetMachineData((MACHINE_TYPE)i).alchemi_mp >= 0 &&
	//		datas->GetNowCrystal() - pSJD->GetMachineData((MACHINE_TYPE)i).alchemi_crystal >= 0);
	//
	//	if (m_selectionAlchemi->ClickMouse())
	//	{
	//		m_manufacturingFlag = true;
	//		m_selectMachineType = m_machineSelect[i]->GetMachineType();
	//		ReduceResource((MACHINE_TYPE)i);
	//	}
	//
	//	// �����}�V�����I�����ꂽ
	//	if (m_machineSelect[i]->GetHitMouseFlag() || m_selectionAlchemi->ClickMouse())
	//	{
	//		m_selectBoxAll = true;
	//		m_selectMachineType = m_machineSelect[i]->GetMachineType();
	//
	//		// �I����ԈȊO�̃}�V���̑I����Ԃ�false�ɂ���
	//		for (int j = 1; j < MACHINE_TYPE::NUM; j++)
	//		{
	//			if (j == i) continue;
	//
	//			m_machineSelect[j]->GetMachineBox()->SetSelectFlag(false);
	//			m_machineSelect[j]->SetHitMouseFlag(false);
	//		}
	//	}
	//}

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		// �v�f�̍X�V����
		m_machineSelect[i]->Update();

		// �����}�V�����I�����ꂽ
		if (m_machineSelect[i]->GetHitMouseFlag())
		{
			m_selectBoxAll = true;
			m_selectMachineType = m_machineSelect[i]->GetMachineType();

			// �I����ԈȊO�̃}�V���̑I����Ԃ�false�ɂ���
			for (int j = 1; j < MACHINE_TYPE::NUM; j++)
			{
				if (j == i) continue;

				m_machineSelect[j]->GetMachineBox()->SetSelectFlag(false);
				m_machineSelect[j]->SetHitMouseFlag(false);
			}
		}
		
	}

	// �������̃N���X�^�����I������Ă���Ȃ�΁Atype�F�}�C�j���O�̑I���{�b�N�X�̐F��ς���
	m_machineSelect[MACHINE_TYPE::MINING]->SetChangeColorFlag(fieldObjectManager->GetCrystalToMouse());

	m_displayMagicCircle->Update();
	m_displayMagicCircle->TransparentUpdate(m_selectBoxAll);


	m_machineName->SetPos(pINP->GetMousePosScreen() - MACHINE_NAME_POS_SHIFT);

	RotationMachineList();

	if (m_selectionRight->ClickMouse() && m_rotateTime >= 1.0f)
	{
		m_rotateTime = 0.0f;

		m_prevSelectNumber = m_selectNumber;
		m_selectNumber++;

		m_forwardOrderFlag = true;

		if (m_selectNumber >= MACHINE_TYPE::NUM) m_selectNumber = 1;
	}

	if (m_selectionLeft->ClickMouse() && m_rotateTime >= 1.0f)
	{
		m_rotateTime = 0.0f;

		m_prevSelectNumber = m_selectNumber;
		m_selectNumber--;

		m_forwardOrderFlag = false;

		if (m_selectNumber <= 0) m_selectNumber = 5;
	}


	// �}�V��UI���I������Ă���Ȃ�ΑΏۂ�ID�����
	// �I������Ă��Ȃ���ԂȂ�΋K��ʒu�ɑ��݂���}�V��UI��ID�����
	MACHINE_TYPE machineType = m_selectMachineType != MACHINE_TYPE::NONE ? m_selectMachineType : (MACHINE_TYPE)m_selectNumber;

	// �v�f���琻���{�^���������ꂽ������󂯎�� ���\�[�X������Ȃ��ꍇ�͒e��
	m_selectionAlchemi->SetActiveFlag(
		datas->GetNowMP() - pSJD->GetMachineData(machineType).alchemi_mp >= 0 &&
		datas->GetNowCrystal() - pSJD->GetMachineData(machineType).alchemi_crystal >= 0);

	// �B���{�^���������ꂽ
	if (m_selectionAlchemi->ClickMouse())
	{

		// �B�����ꂽ�u�Ԃ��擾
		m_manufacturingFlag = true;

		// �B�����ꂽ�u�Ԃ̃}�V���^�C�v���擾����
		m_selectMachineType = m_machineSelect[machineType]->GetMachineType();

		// �}�V���^�C�v���擾�����\�[�X�Q�����炷
		ReduceResource(machineType);

	}

}

void MachineSelectManager::Render()
{
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = ShareData::GetInstance().GetSpriteBatch();
	ShareData& pSD = ShareData::GetInstance();


	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	RECT rect = { 0,0,84,84 };
	pSB->Draw(pSL.GetMachineUICursorTexture().Get(), SimpleMath::Vector2(1120, 560), &rect, Colors::White, 0.0f, SimpleMath::Vector2(84 / 2, 84 / 2));
	pSB->End();

}

void MachineSelectManager::RenderUI(int machineNum[])
{

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{

		 // �}�E�X���u����Ă���}�V���̖��O��`�悷��(�ݒu�\�Ȃ�D�F,�s�\�ł���ΐԐF�ŕ`��)
		if (m_machineSelect[i]->GetHitMouseFlag())
		{

			SimpleMath::Color name_color = SimpleMath::Color(0.9f, 0.9f, 0.9f, 0.95f);

			if(machineNum[i] <= 0) name_color = SimpleMath::Color(1.0f, 0.0f, 0.0f, 0.95f);

			m_machineName->DrawUI(pSL.GetMachineNameTexture().Get(),
				SpriteCutter(768 / 6,28,i,0),
				nullptr,
				RECT(),
				SimpleMath::Color(),
				name_color);
		}

	}

	m_selectionLeft->Draw();
	m_selectionRight->Draw();

	RECT rect = { 0,0,120,120 };
	m_selectionAlchemi->DrawUI(pSL.GetAlchemiButtonTexture().Get(),rect,nullptr);

	// ��������`�悷��
	m_machineNumRender->SetNumber(machineNum[m_selectMachineType != 0 ? m_selectMachineType : m_selectNumber]);
	m_machineNumRender->Render();

}

// �Ăяo�����ŗv�f���񂵂Ă���
void MachineSelectManager::ModelRender(int index)
{

	m_machineSelect[index]->DisplayObject();
}

void MachineSelectManager::MagicCircleRender()
{
	m_displayMagicCircle->SpritebatchRender();
}

void MachineSelectManager::Finalize()
{

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

}

void MachineSelectManager::ReduceResource(MACHINE_TYPE type)
{
	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();

	datas->SetNowMP(datas->GetNowMP() - pSJD->GetMachineData(type).alchemi_mp);

	datas->SetNowCrystal(datas->GetNowCrystal() - pSJD->GetMachineData(type).alchemi_crystal);

}

void MachineSelectManager::RotationMachineList()
{
	// �K��l�Ȃ�Ώ������s��Ȃ�
	if (m_rotateTime >= 1.0f)
	{
		m_rotateTime = 1.0f;
		return;
	}
	// ��]���s��
	m_rotateTime += DeltaTime::GetInstance().GetNomalDeltaTime() * 6.0f;

	float deg = 360.0 / 5;
	float radians	= (deg * XM_PI) / 180.0f;

	float adjustmentRadian = (95 * XM_PI) / 180.0f + (180 * XM_PI) / 180.0f;


	float rotateAngle = 0.0f;
	
	if(m_forwardOrderFlag) rotateAngle = Easing::EaseInOutQuint((float)m_selectNumber, (float)m_selectNumber + 1.0f, m_rotateTime);

	else				   rotateAngle = Easing::EaseInOutQuint((float)m_selectNumber + 1.0f, (float)m_selectNumber + 2.0f, 1.0f - m_rotateTime);

	float posWaveY = Easing::EaseInExpo(0.8f, 1.0f, m_rotateTime) * 50.0f;

	m_machineNumRender->SetPosition(SimpleMath::Vector2(POS.x, POS.y + sin(posWaveY)));

	for (int i = -1; i < MACHINE_TYPE::NUM - 2; i++)
	{
		m_machineSelect[i + 2]->SetPos({ POS.x + cosf((radians * i - adjustmentRadian) - radians * rotateAngle) * DIRECTION ,
										 POS.y + sinf((radians * i - adjustmentRadian) - radians * rotateAngle) * DIRECTION });

		m_machineSelect[i + 2]->GetMachineBox()->SetSavePos(m_machineSelect[i + 2]->GetPos());
	}
}
