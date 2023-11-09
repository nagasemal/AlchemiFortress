#pragma once
#include "Scenes/PlayScene/Tutorial/StatePattern/ITutorialState.h"

class SelectionUI;
class Tutorial_NoneState : public ITutorialState
{
public:
	Tutorial_NoneState();
	~Tutorial_NoneState();

	void Update() override;

	void Render() override;

private:

};