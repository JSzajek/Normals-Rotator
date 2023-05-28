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

	const Elysium::Shared<Elysium::Texture2D> currImg = m_currentTexture.lock();
	if (currImg.get() != nullptr)
	{
		ImGui::Text("Filepath: ");
		ImGui::SameLine();
		ImGui::TextWrapped(m_currentFilepath.c_str());

		ImGui::Spacing();

		ImGui::Text("Details:");
		ImGui::Text("Width: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(currImg->GetWidth()).c_str());
		ImGui::SameLine();
		ImGui::Text("Height: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(currImg->GetHeight()).c_str());

		ImGui::Spacing();

		ImGui::Text("Bit Depth: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(currImg->GetByteDepth()).c_str());
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void PropertiesPanel::SetImageProperties(const std::string& imageFilepath, const Elysium::Weak<Elysium::Texture2D>& texture)
{
	m_currentFilepath = imageFilepath;
	m_currentTexture = texture;
}
