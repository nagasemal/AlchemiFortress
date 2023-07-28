//--------------------------------------------------------------------------------------
// File: AM_Upper.h
//
//�@�͈͓��̃A���P�~�J���}�V���ɑ΂���
//  �������ʂ𔭊�����
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Upper : public AlchemicalMachineObject
{
public:
	AM_Upper();
	~AM_Upper();

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

	void RenderUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) override;

	void LvUp() override;

private:

	// �I���\�I�u�W�F�N�g
	std::unique_ptr<SelectionBox> m_selectBox[4];

};