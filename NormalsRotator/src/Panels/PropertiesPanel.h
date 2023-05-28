#pragma once

#include "Elysium.h"

class PropertiesPanel
{
public:
	PropertiesPanel();
public:
	void OnImGuiRender();

	void SetImageProperties(const std::string& imageFilepath,
							const Elysium::Weak<Elysium::Texture2D>& texture);
private:
	bool m_focused;
	bool m_hovered;

	std::string m_currentFilepath;
	Elysium::Weak<Elysium::Texture2D> m_currentTexture;
};
