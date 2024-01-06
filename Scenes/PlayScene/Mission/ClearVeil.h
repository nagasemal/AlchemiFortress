#pragma once
#include "NecromaLib/GameData/Animation.h"

class UserInterface;

class ClearVeil
{
public:
	ClearVeil();
	~ClearVeil();

	void Update(bool startFlag);
	void Render();

	// ����Wave�Ɉڍs������
	bool NextWave();

private:

	// NextWave�̕����`��
	std::unique_ptr<UserInterface> m_nextWave_Top,m_nextWave_Bottom;
	// WAVE�N���A�����ۂɗ����A�j���[�V�����p�ϐ�
	AnimationData m_waveAnimation;

};