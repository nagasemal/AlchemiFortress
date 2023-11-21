//--------------------------------------------------------------------------------------
// File		: Gauge.h
//
//�@���݂̃��\�[�X�ʂ����_UI�Ƃ��ĕ\������
// 
// Use		: PlayScene
// Date		: 2023.6.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/UserInterfase.h"

class SelectionBox;
class Number;

class Gauge
{
private:

public:
	Gauge();
	~Gauge();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	// ������UserInterfase�𓯂��ʒu�ɏo�������邽�߂̊֐�
	void Add_MP(SimpleMath::Vector2 position
			,SimpleMath::Vector2 scale
			, UserInterface::ANCHOR anchor);

	void Add_Crystal(SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	void Add_Hp(SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	SimpleMath::Vector2 GetHPGaugePos();
	SimpleMath::Vector2 GetMPGaugePos();
	SimpleMath::Vector2 GetCrystalGaugePos();

private:

	// MP
	// ��������p
	std::unique_ptr<UserInterface> m_gauge_Mp;
	// �w�i�p
	std::unique_ptr<UserInterface> m_base_Mp;
	// ����ۂ̑J�ڗp
	std::unique_ptr<UserInterface> m_back_Mp;

	// �ڍׂȐ���`��
	std::unique_ptr<Number> m_resourceRenderMP;
	//// �OF�̖��͗�
	//float m_prevMP;

	// Mp�̉摜�\��
	std::unique_ptr<SelectionBox> m_box_mp;

	// �N���X�^��
	std::unique_ptr<UserInterface> m_gauge_Crystal;
	std::unique_ptr<UserInterface> m_base_Crystal;
	std::unique_ptr<UserInterface> m_back_Crystal;

	std::unique_ptr<SelectionBox> m_box_Crystal;

	// �ڍׂȐ���`��
	std::unique_ptr<Number> m_resourceRenderCrystal;
	//// �OF�̌�����
	//float m_prevCrystal;


	// HP
	std::unique_ptr<UserInterface> m_gauge_Hp;
	std::unique_ptr<UserInterface> m_base_Hp;
	std::unique_ptr<UserInterface> m_back_Hp;

	std::unique_ptr<SelectionBox> m_box_Hp;

	// �ڍׂȐ���`��
	std::unique_ptr<Number> m_resourceRenderHP;
	//// �OF��HP��
	//float m_prevHP;

	// �OF��HP������ێ�
	float m_difRedioHp;

	// �OF��MP������ێ�
	float m_difRedioMp;

	// �OF�̃N���X�^��������ێ�
	float m_difRedioCrystal;

};