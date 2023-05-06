#include "norm_pch.h"
#include "ControlsPanel.h"

#include <imgui.h>

ControlsPanel::ControlsPanel()
	: m_focused(false), m_hovered(false),
	m_recenterFocus(true),
	m_rotationDegrees(0),
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

	if (ImGui::Button("Recenter"))
	{
		m_recenterFocus = true;
		m_changed = true;
	}

	// Rotation
	ImGui::Text("Rotation Controls");
	ImGui::Text("Degrees ");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##rotation_degrees", &m_rotationDegrees, 0, 360, "%0.3f"))
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

	ImGui::Text("Image Flip");
	if (ImGui::Checkbox("Flip Horizontal", &m_flipHorizontal))
	{
		m_changed = true;
	}
	if (ImGui::Checkbox("Flip Vertical", &m_flipVertical))
	{
		m_changed = true;
	}

	ImGui::Spacing();
	ImGui::Text("Flip Channels");
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

void ControlsPanel::OnEvent(Elysium::Event& _event)
{
	Elysium::EventDispatcher dispatcher(_event);

	dispatcher.Dispatch<Elysium::KeyPressedEvent>(BIND_EVENT_FN(ControlsPanel::OnKeyPressed));
}

void ControlsPanel::FlushChangeState()
{
	m_recenterFocus = false;

	m_changed = false;
}

bool ControlsPanel::OnKeyPressed(Elysium::KeyPressedEvent& _event)
{
	if (_event.GetKeyCode() == Elysium::Key::F)
	{
		m_recenterFocus = true;
		m_changed = true;
	}

	return false;
}
