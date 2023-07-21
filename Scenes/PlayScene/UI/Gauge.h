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
	void Add_MP(DirectX::SimpleMath::Vector2 position
			,DirectX::SimpleMath::Vector2 scale
			, UserInterface::ANCHOR anchor);

	void Add_Crystal(DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

	void Add_Hp(DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

private:

	// MP
	// ��������p
	std::unique_ptr<UserInterface> m_gauge_Mp;
	// �w�i�p
	std::unique_ptr<UserInterface> m_base_Mp;
	// Mp�̉摜�\��
	std::unique_ptr<SelectionBox> m_box_mp;

	// �N���X�^��
	std::unique_ptr<UserInterface> m_gauge_Crystal;
	std::unique_ptr<UserInterface> m_base_Crystal;

	std::unique_ptr<SelectionBox> m_box_Crystal;


	// HP
	std::unique_ptr<UserInterface> m_gauge_Hp;
	std::unique_ptr<UserInterface> m_base_Hp;

	std::unique_ptr<SelectionBox> m_box_Hp;

};