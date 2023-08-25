//--------------------------------------------------------------------------------------
// File		: Number.h
//
// �X�v���C�g�摜�ɂ�鐔���̕`��Ɏg�p
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.17
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class Number
{
public:
	Number();
	Number(DirectX::SimpleMath::Vector2 position,DirectX::SimpleMath::Vector2 rage);
	~Number();

	// �\�����鐔���̐ݒ�
	void SetNumber(int num);
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
	void SetRage(DirectX::SimpleMath::Vector2 rage) { m_rage = rage; }


	// �����摜�`��
	void Render();

	// Missions
	void Render_SelectScene(int first,int next);

private:

	void Number_Render(int num,DirectX::SimpleMath::Vector2 pos);

	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_rage;

	int m_num;

};