#include "norm_pch.h"
#include "GroupExportPanel.h"

GroupExportPanel::GroupExportPanel()
	: m_focused(false), m_hovered(false), m_isOpen(false)
{
}

void GroupExportPanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (!ImGui::Begin("Group Export", &m_isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking))
		ImGui::End();

	ImGui::Text("WIP");

	ImGui::End();
	ImGui::PopStyleVar();
}
