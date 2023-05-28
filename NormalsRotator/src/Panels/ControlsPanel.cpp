#include "norm_pch.h"
#include "ControlsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

ControlsPanel::ControlsPanel()
	: m_focused(false), m_hovered(false),
	m_rotationDegrees(0),
	m_isClipped(false),
	m_clipDimensions(0),
	m_flipHorizontal(false),
	m_flipVertical(false),
	m_flipRed(false),
	m_flipGreen(false),
	m_flipBlue(false),
	m_changed(true)
{
}

void ControlsPanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (!ImGui::Begin("Controls Panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		ImGui::End();

	// Rotation
	ImGui::Text("Rotation Controls");
	ImGui::Text("Degrees ");
	ImGui::SameLine();
	if (ImGui::DragFloat("##rotation_degrees", &m_rotationDegrees, 1, 0, 360, "%0.3f"))
	{
		m_changed = true;
	}

	ImGui::Text("Rotation Presets");
	if (ImGui::Button("90-CCW"))
	{
		m_rotationDegrees = 270;
		m_changed = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("180"))
	{
		m_rotationDegrees = 180;
		m_changed = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("90-CW"))
	{
		m_rotationDegrees = 90;
		m_changed = true;
	}
	
	ImGui::Spacing();
	
	ImGui::Text("Clipping:");
	if (ImGui::Checkbox("Clip Output", &m_isClipped))
	{
		m_changed = true;
	}

	if (m_isClipped)
	{
		ImGui::Text("Crop Dimensions");

		ImGui::Columns(4, "dims_columns", false);
		ImGui::SetColumnWidth(-1, 30);
		ImGui::Text("X");
		ImGui::Text("W");
		ImGui::NextColumn();
		ImGui::SetColumnWidth(-1, 100);
		if (ImGui::DragInt("##x-offset", &m_clipDimensions[0]))
		{
			m_changed = true;
		}
		if (ImGui::DragInt("##width", &m_clipDimensions[2]))
		{
			m_changed = true;
		}
		ImGui::NextColumn();
		ImGui::SetColumnWidth(-1, 30);
		ImGui::Text("Y");
		ImGui::Text("H");
		ImGui::NextColumn();
		ImGui::SetColumnWidth(-1, 100);
		if (ImGui::DragInt("##y-offset", &m_clipDimensions[1]))
		{
			m_changed = true;
		}
		if (ImGui::DragInt("##height", &m_clipDimensions[3]))
		{
			m_changed = true;
		}
		
		ImGui::EndColumns();
	}

	ImGui::Text("Image Flip [WIP]");
	if (ImGui::Checkbox("Flip Horizontal", &m_flipHorizontal))
	{
		m_changed = true;
	}
	if (ImGui::Checkbox("Flip Vertical", &m_flipVertical))
	{
		m_changed = true;
	}

	ImGui::Spacing();
	ImGui::Text("Flip Channels [WIP]");
	if (ImGui::Checkbox("Flip Red Channel", &m_flipRed))
	{
		m_changed = true;
	}
	if (ImGui::Checkbox("Flip Green Channel", &m_flipGreen))
	{
		m_changed = true;
	}
	if (ImGui::Checkbox("Flip Blue Channel", &m_flipBlue))
	{
		m_changed = true;
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void ControlsPanel::FlushChangeState()
{
	m_changed = false;
}
