#pragma once

class PropertiesPanel
{
private:
	bool m_focused;
	bool m_hovered;
public:
	PropertiesPanel();
public:
	void OnImGuiRender();
};
