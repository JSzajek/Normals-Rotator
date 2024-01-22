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
public:
	bool ExportFiles(std::vector<std::string>& toExport);
private:
	void AddFile();
	void DeleteSelected();
private:
	bool m_focused;
	bool m_hovered;

	bool m_isOpen;

	bool m_hasExports;

	std::vector<int> m_filesSelected;
	std::vector<std::string> m_filesToExport;
};