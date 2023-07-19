

#pragma once

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineFilter.h"

class TitleCall
{
public:
	TitleCall();
	~TitleCall();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

private:

	// �~����ɕ��ׂ鏈��
	DirectX::SimpleMath::Vector3 SetVelocityCircle(int index, int max, float range)
	{

		float radian = XM_2PI / static_cast<float>(max);

		// ��̊p�x��������΂����S���킩��
		float x = range * cosf(radian * index);
		float z = range * sinf(radian * index);

		DirectX::SimpleMath::Vector3 pos = { x,0,z };

		return pos;
	}

	void SettingAMMachine();

	void MovingMachine(int index);


	std::unique_ptr<AlchemicalMachineFilter>				m_AMFilter;
	std::vector<std::shared_ptr<AlchemicalMachineObject>>	m_AMobjects;

};