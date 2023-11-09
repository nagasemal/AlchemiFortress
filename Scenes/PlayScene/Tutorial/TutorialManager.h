#pragma once

class PlayScene;

class TutorialManager
{
public:
	TutorialManager(std::vector<int> tutorialNumber, PlayScene* pPlayScene);
	~TutorialManager();

	void Update();

	void Render();

private:

};