#include "norm_pch.h"

#include "GroupExportPanel.h"

#include <imgui_internal.h>

GroupExportPanel::GroupExportPanel()
	: m_focused(false), 
	m_hovered(false), 
	m_isOpen(false), 
	m_hasExports(false)
{
}

void GroupExportPanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (!ImGui::Begin("Group Export", &m_isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
												 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}

	const float panelWidth = ImGui::GetWindowWidth();

	ImGui::Columns(1);

	ImGui::Spacing();
	ImGui::SameLine();
	ImGui::Text("Export with the same rotation & clipping modifications.");

	ImGui::PushID("##FileExportControlsGroup");
	ImGui::BeginGroup();

	const ImGuiWindowFlags child_flags = ImGuiWindowFlags_MenuBar;
	const ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);
	const bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(panelWidth, 300.0f), true, child_flags);
	if (ImGui::BeginMenuBar())
	{
		ImGui::Columns(3, "FileExportControls", false);
		ImGui::SetColumnWidth(0, 100.f);
		ImGui::SetColumnWidth(1, panelWidth - 100.f - 105.f);

		ImGui::TextUnformatted("Files");
		ImGui::NextColumn();
		ImGui::NextColumn();

		if (ImGui::Button(ICON_FA_PLUS, ImVec2(30, 26)))
		{
			AddFile();
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_TRASH, ImVec2(30, 25)))
		{
			DeleteSelected();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Columns(1);

	for (uint32_t i = 0; i < m_filesToExport.size(); ++i)
	{
		const std::string fileToExport = m_filesToExport[i];
		ImGui::Spacing();
		ImGui::SameLine();
		if (ImGui::Selectable(fileToExport.c_str(), (bool)m_filesSelected[i]))
		{
			if (!ImGui::GetIO().KeyCtrl)
				memset(m_filesSelected.data(), 0, m_filesToExport.size() * sizeof(int));
		
			m_filesSelected[i] ^= 1;
		}
	}

	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();

	bool disabled = false;
	if (m_filesToExport.empty())
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		disabled = true;
	}

	if (ImGui::Button("Export", ImVec2(panelWidth, 25)))
	{
		m_hasExports = !m_filesToExport.empty();
	}

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

bool GroupExportPanel::ExportFiles(std::vector<std::string>& toExport)
{
	if (m_hasExports)
	{
		toExport.clear();
		toExport.insert(toExport.begin(), m_filesToExport.begin(), m_filesToExport.end());
		
		m_hasExports = false;
		return true;
	}
	return false;
}

void GroupExportPanel::AddFile()
{
	const std::string openfile = Elysium::FileDialogs::OpenFile("All Supported Formats (*.png, *.jpg, *.jpeg, *.jpe, *.jp2, *.tiff, *.tif, *.exr, *.bmp)\0*.png;*.jpg;*.jpeg;*.jpe;*.jp2;*.tiff;*.tif;*.exr;*.bmp\0"
																"PNG Image (*.png)\0*.png\0"
																"JPEG Image (*.jpg, *.jpeg, *.jpe)\0*.jpg;*.jpeg;*.jpe\0"
																"JPEG-2000 Image (*.jp2)\0*.jp2\0"
																"TIFF Image (*.tiff, *.tif)\0*.tiff;*.tif\0"
																"OpenEXR (*.exr, *.EXR)\0*.exr\0"
																"Windows BMP Image (*.bmp)\0*.bmp\0");

	m_filesToExport.push_back(openfile);
	m_filesSelected.push_back(false);
}

void GroupExportPanel::DeleteSelected()
{
	std::vector<uint32_t> indicesToDelete;
	for (uint32_t i = 0; i < m_filesSelected.size(); ++i)
	{
		if (m_filesSelected[i])
			indicesToDelete.push_back(i);
	}

	for (auto iter = indicesToDelete.crbegin(); iter != indicesToDelete.crend(); ++iter)
	{
		m_filesSelected.erase(m_filesSelected.begin() + *iter);
		m_filesToExport.erase(m_filesToExport.begin() + *iter);
	}
}
