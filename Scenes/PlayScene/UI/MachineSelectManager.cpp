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

#define NUM_RENDER_POS			SimpleMath::Vector2(1200, 670)
#define MP_RENDER_POS			SimpleMath::Vector2(1210, 590)
#define CRYSTAL_RENDER_POS		SimpleMath::Vector2(1210, 620)

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
	m_cursorMachineNumber(),
	m_prevSelectNumber()
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

	m_machineNumRender	= std::make_unique<Number>(NUM_RENDER_POS, SimpleMath::Vector2(0.6f, 0.6f));

	m_mpNumRender		= std::make_unique<Number>(MP_RENDER_POS, SimpleMath::Vector2(0.3f, 0.3f));

	m_crystalRender		= std::make_unique<Number>(CRYSTAL_RENDER_POS, SimpleMath::Vector2(0.3f, 0.3f));

}

void MachineSelectManager::Update(FieldObjectManager* fieldObjectManager)
{

	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	//m_selectMachineType = MACHINE_TYPE::NONE;

	auto datas = DataManager::GetInstance();
	// json����ǂݎ�����l���g�p����
	auto pSJD = &ShareJsonData::GetInstance();

	auto pINP = &InputSupport::GetInstance();

	m_selectionAlchemi	->HitMouse(true);

	m_selectionLeft		->HitMouse(true);
	m_selectionRight	->HitMouse(true);


	m_machineSelect[m_selectNumber]->Update();

	if (m_machineSelect[m_selectNumber]->GetHitMouseFlag())
	{
		m_textAnimation += DeltaTime::GetInstance().GetNomalDeltaTime();
	}
	else
	{
		m_textAnimation -= DeltaTime::GetInstance().GetNomalDeltaTime();
	}

	// �������̃N���X�^�����I������Ă���Ȃ�΁Atype�F�}�C�j���O�̑I���{�b�N�X�̐F��ς���
	m_machineSelect[MACHINE_TYPE::MINING]->SetChangeColorFlag(fieldObjectManager->GetCrystalToMouse());

	m_displayMagicCircle->Update();
	m_displayMagicCircle->TransparentUpdate(m_selectBoxAll);


	m_machineName->SetSavePos(pINP->GetMousePosScreen() - MACHINE_NAME_POS_SHIFT);

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
	MACHINE_TYPE machineType = (MACHINE_TYPE)m_selectNumber;

	//m_selectMachineType = machineType;

	// �v�f���琻���{�^���������ꂽ������󂯎�� ���\�[�X������Ȃ��ꍇ�͒e��
	m_selectionAlchemi->SetActiveFlag(
		datas->GetNowMP() - pSJD->GetMachineData(machineType).alchemi_mp >= 0 &&
		datas->GetNowCrystal() - pSJD->GetMachineData(machineType).alchemi_crystal >= 0);

	m_mpNumRender	->	SetNumber(pSJD->GetMachineData(machineType).alchemi_mp);
	m_crystalRender	->	SetNumber(pSJD->GetMachineData(machineType).alchemi_crystal);

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
	auto pSB = ShareData::GetInstance().GetSpriteBatch();
	ShareData& pSD = ShareData::GetInstance();
	auto pINP = &InputSupport::GetInstance();

	m_selectionLeft->Draw();
	m_selectionRight->Draw();

	RECT rect = { 0,0,120,120 };
	m_selectionAlchemi->DrawUI(pSL.GetAlchemiButtonTexture().Get(), rect, nullptr);

	// �}�V���̖��O�̕`����s��(���݂ǂ̃}�V����I�����Ă��邩�N���ɂ��邽��)
	SimpleMath::Color name_color = SimpleMath::Color(0.9f, 0.9f, 0.9f, 0.95f);

	if (machineNum[m_selectNumber] <= 0) name_color = SimpleMath::Color(1.0f, 0.0f, 0.0f, 0.95f);

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// �}�V���̖��O��`�悷��
	if (!pINP->GetHitUI())
	{
		rect = SpriteCutter(768 / 6, 28, m_selectNumber, 0);
		pSB->Draw(pSL.GetMachineNameTexture().tex.Get(), pINP->GetMousePosScreen() - MACHINE_NAME_POS_SHIFT,
			&rect, name_color, 0.0f, SimpleMath::Vector2((768.0f / 6.0f) / 2.0f, 28.0f / 2.0f));
	}

	// ���̓A�C�R����`�悷��
	rect = SpriteCutter(64,64,0,0);
	pSB->Draw(pSL.GetElementTexture().Get(), MP_RENDER_POS - SimpleMath::Vector2(40.0f,0.0f),
		&rect, Colors::White,0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f),0.5f);

	// �����A�C�R����`�悷��
	rect = SpriteCutter(64, 64, 1, 0);
	pSB->Draw(pSL.GetElementTexture().Get(), CRYSTAL_RENDER_POS - SimpleMath::Vector2(40.0f, 0.0f),
		&rect, Colors::White, 0.0f, SimpleMath::Vector2(64.0f / 2.0f, 64.0f / 2.0f),0.5f);

	// �}�V�������e�L�X�g��`�悷��
	SpriteLoder::TextureData textData = pSL.GetMachineUIText(m_selectNumber);
	rect = { 0,0,textData.width,textData.height };
	pSB->Draw(textData.tex.Get(), SimpleMath::Vector2(1000.0f, 500.0f - Easing::EaseOutQuint(0.0f,30.0f,m_textAnimation)),
			&rect, SimpleMath::Color(1.0f,1.0f,1.0f, m_textAnimation), 0.0f, SimpleMath::Vector2(textData.width / 2.0f, textData.height / 2.0f), 0.5f);

	pSB->End();

	// ��������`�悷��
	m_machineNumRender->SetNumber(machineNum[m_selectNumber]);
	m_machineNumRender->Render();

	// �K�v���\�[�X�ʂ�`�悷��
	m_mpNumRender	->	Render();
	m_crystalRender	->	Render();

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

	m_machineNumRender->SetPosition(SimpleMath::Vector2(NUM_RENDER_POS.x, NUM_RENDER_POS.y + sin(posWaveY)));

	m_mpNumRender->SetPosition(SimpleMath::Vector2(MP_RENDER_POS.x, MP_RENDER_POS.y + sin(posWaveY)));

	m_crystalRender->SetPosition(SimpleMath::Vector2(CRYSTAL_RENDER_POS.x, CRYSTAL_RENDER_POS.y + sin(posWaveY)));

	for (int i = -1; i < MACHINE_TYPE::NUM - 2; i++)
	{
		m_machineSelect[i + 2]->SetPos({ POS.x + cosf((radians * i - adjustmentRadian) - radians * rotateAngle) * DIRECTION ,
										 POS.y + sinf((radians * i - adjustmentRadian) - radians * rotateAngle) * DIRECTION });

		m_machineSelect[i + 2]->GetMachineBox()->SetSavePos(m_machineSelect[i + 2]->GetPos());
	}
}

void MachineSelectManager::LRButtonLock(bool flag)
{
	m_selectionLeft->SetActiveFlag(flag);
	m_selectionRight->SetActiveFlag(flag);
}
