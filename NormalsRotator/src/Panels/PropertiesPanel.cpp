#include "norm_pch.h"
#include "PropertiesPanel.h"

#include <imgui.h>

PropertiesPanel::PropertiesPanel()
	: m_focused(false), m_hovered(false)
{
}

void PropertiesPanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (!ImGui::Begin("Properties Panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		ImGui::End();

	ImGui::Columns(1);

	ImGui::End();
	ImGui::PopStyleVar();
}
