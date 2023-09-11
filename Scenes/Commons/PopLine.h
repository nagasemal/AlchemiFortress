#pragma once
#include "Scenes/Commons/SelectionUI.h"
#include "NecromaLib/GameData/Animation.h"

class PopLine : public SelectionUI
{
public:
	PopLine(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, SimpleMath::Vector2 maxRage, float lineRage = 2.5f);
	~PopLine();

	void Update()				override;
	// 描画
	void Draw()					override;

	void ResetAnimationData() { m_animationData.time = 0.0f;}
	void SetColor(SimpleMath::Color color) { m_color = color; }
	void SetMaxRage(SimpleMath::Vector2 rage) { m_maxRage = rage * 2; }

private:

	AnimationData m_animationData;

	SimpleMath::Vector2 m_maxRage;
	SimpleMath::Vector2 m_popPos;
	SimpleMath::Vector2 m_lineRage;
	SimpleMath::Vector2 m_ancerRect;

	SimpleMath::Color m_color;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPosition>> m_batch;

};