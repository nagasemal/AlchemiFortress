#pragma once
#include "Scenes/PlayScene/Tutorial/StatePattern/ITutorialState.h"

class SelectionUI;

class Tutorial_ModelSelectState : public ITutorialState
{
public:
	Tutorial_ModelSelectState();
	~Tutorial_ModelSelectState();

	void Update() override;

	void Render() override;

private:

};