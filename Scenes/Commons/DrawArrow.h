//--------------------------------------------------------------------------------------
// File: DrawArrow.h
//
//�@����UI�\��
//  
// 
// Date: 2023.8.25
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class DrawArrow : public GameObjct2D
{
public:
	DrawArrow();
	~DrawArrow();

	// ������
	void Initialize()			override;

	// �X�V
	void Update()				override;

	// �`��
	void Draw()					override;

	// �I������
	void Finalize()				override;

private:

};