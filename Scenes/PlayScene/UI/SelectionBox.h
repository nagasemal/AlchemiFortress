//--------------------------------------------------------------------------------------
// File: SelectionBox.h
//
//�@�I�u�W�F�N�g�I�������Ɍ����I���\UI
// 
// Date: 2023.6.12
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class SelectionBox : public GameObjct2D
{
public:
	SelectionBox(DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 rage);
	~SelectionBox();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	// �}�E�X���G�ꂽ����
	bool HitMouse();
	// ���N���b�N���ꂽ����
	bool ClickMouse();
	// �}�E�X�����ꂽ����
	bool ExitMouse();

private:

	bool m_hitMouseFlag;

};
