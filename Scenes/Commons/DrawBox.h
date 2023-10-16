#pragma once

#include "Scenes/Commons/SelectionUI.h"
#include "NecromaLib/GameData/Animation.h"

class DrawBox : public SelectionUI
{
public:
	DrawBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,float lineRage = 2.5f);
	~DrawBox();

	// �X�V����
	void Update()				override;

	// �`��
	void Draw()					override;

	void ResetAnimationData() { m_animationData_Second.time = m_animationData_First.time = 0.0f; }
	void SetColor(SimpleMath::Color color) { m_color = color; }

private:

	AnimationData m_animationData_First;
	AnimationData m_animationData_Second;

	float m_rotate_easing;

	SimpleMath::Vector2 m_lineRage_Vertical;
	SimpleMath::Vector2 m_lineRage_Beside;
	float m_rotate;

	// �������o�p�̃|�W�V����
	SimpleMath::Vector2 m_boxPos;

	SimpleMath::Color m_color;

};