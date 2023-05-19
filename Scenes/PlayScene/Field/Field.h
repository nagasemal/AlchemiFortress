#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

class Field : public GameObjct3D
{
public:
	Field();
	~Field();

	// ������
	void Initialize()	override;

	// �X�V
	void Update()		override;

	// �`��
	void Draw()			override;

	// �I������
	void Finalize()		override;

/*===[�@�A�N�Z�T�@]===*/
public:

	bool		GetHitMouse()	const { return m_hitMouseFlag; }

private:

	// �����f��
	std::unique_ptr<DirectX::Model> m_Model;

	// �}�E�X�Ƃ̏Փˎ�
	bool		m_hitMouseFlag;

};