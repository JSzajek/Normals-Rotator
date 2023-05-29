#pragma once

#include "Elysium.h"

class GroupExportPanel
{
public:
	GroupExportPanel();
public:
	void OnImGuiRender();

	inline bool IsOpen() const { return m_isOpen; }
	inline void OpenPanel() { m_isOpen = true; }
private:
	bool m_focused;
	bool m_hovered;

	bool m_isOpen;
};