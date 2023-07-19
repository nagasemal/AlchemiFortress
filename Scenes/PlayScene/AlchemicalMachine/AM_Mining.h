//--------------------------------------------------------------------------------------
// File: AM_Mining.h
//
//�@�͈͓��̍z���ɑ΂���
//  ���ʂ𔭊�����
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Mining : public AlchemicalMachineObject
{
public:
	AM_Mining();
	~AM_Mining();

	// ������
	void Initialize()					override;

	// �X�V
	void Update()						override;

	// �I�����ɍX�V����鏈��
	void SelectUpdate()					override;

	// �`��@(UI�֌W)
	void Draw()							override;

	// �I������
	void Finalize()						override;

	void AllFieldObject(FieldObjectManager* fieldManager);

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	void LvUp() override;

public:

	// ����K�v�ƂȂ�N���X�^���̐�
	const int GetNextLvCrystal() const { return (15 * m_lv) * (5 * m_lv); }

public:

	// �������X�p��
	float m_timer;

private:

};