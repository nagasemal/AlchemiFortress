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
#include "Scenes/Commons/SelectionUI.h"

class DrawArrow : public SelectionUI
{
public:

	DrawArrow(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,int direciton);
	~DrawArrow();

	// �`��
	void Draw()					override;

private:

	int m_direction;

};