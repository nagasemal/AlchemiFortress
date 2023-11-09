#pragma once
#include "Scenes/PlayScene/Tutorial/StatePattern/ITutorialState.h"

class SelectionUI;

class Tutorial_UISelectState : public ITutorialState
{
public:
	Tutorial_UISelectState();
	~Tutorial_UISelectState();

	void Update() override;

	void Render() override;

private:

};