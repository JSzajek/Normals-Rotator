#include "norm_pch.h"
#include "PropertiesPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace PropUtils
{
	int32_t ConvertBitDepthToNum(Elysium::PixelBitDepth bitdepth)
	{
		switch (bitdepth)
		{
		case Elysium::PixelBitDepth::Bit8U:
		case Elysium::PixelBitDepth::Bit8S:
			return 8;
		case Elysium::PixelBitDepth::Bit16S:
		case Elysium::PixelBitDepth::Bit16F:
			return 16;
		case Elysium::PixelBitDepth::Bit32F:
		case Elysium::PixelBitDepth::Bit32S:
		case Elysium::PixelBitDepth::Bit32U:
			return 32;
		}
		return -1;
	}

	std::string ConvertFormatToString(Elysium::PixelFormat format)
	{
		switch (format)
		{
		case Elysium::PixelFormat::R:
			return "R";
		case Elysium::PixelFormat::RG:
			return "RG";
		case Elysium::PixelFormat::RGB:
			return "RGB";
		case Elysium::PixelFormat::RGBA:
			return "RGBA";
		}
		return "Unknown";
	}
}

PropertiesPanel::PropertiesPanel()
	: m_focused(false), m_hovered(false)
{
}

void PropertiesPanel::OnImGuiRender()
{
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (!ImGui::Begin("Properties Panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		ImGui::End();

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

		ImGui::Text("Channels: ");
		ImGui::SameLine();
		ImGui::Text(PropUtils::ConvertFormatToString(currImg->GetPixelFormat()).c_str());

		ImGui::Text("Bit Depth: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(PropUtils::ConvertBitDepthToNum(currImg->GetBitDepth())).c_str());
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void PropertiesPanel::SetImageProperties(const std::string& imageFilepath, const Elysium::Weak<Elysium::Texture2D>& texture)
{
	m_currentFilepath = imageFilepath;
	m_currentTexture = texture;
}
