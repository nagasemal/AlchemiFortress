#pragma once

class SelectionBox;

class ResultUIManager
{
public:
	ResultUIManager();
	~ResultUIManager();

	void Update();
	void Render();

private:

	std::unique_ptr<SelectionBox> m_selectionBox_Next;
	std::unique_ptr<SelectionBox> m_selectionBox_Retry;
	std::unique_ptr<SelectionBox> m_selectionBox_Back;

};