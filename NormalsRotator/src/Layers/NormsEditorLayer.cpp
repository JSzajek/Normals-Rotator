#include "norm_pch.h"

#include "NormsEditorLayer.h"

#include "Panels/ViewerPanel.h"
#include "Panels/ControlsPanel.h"
#include "Panels/PropertiesPanel.h"

#include "Elysium/Graphics/GraphicsCalls.h"
#include "Elysium/Scene/Entity.h"
#include "Elysium/Scene/2DComponents.h"

#include "Elysium/Factories/ShaderFactory.h"

#include <imgui/imgui_internal.h>

#include <iconfontcppheaders/IconsFontAwesome5.h>

NormsEditorLayer::NormsEditorLayer()
{
	//m_outputTextureId = m_pipeline.GetOutput();

	m_exampleNormalsMap = Elysium::Texture2D::Create("Content/textures/normal_map_example.png");
	m_outputTextureId = m_exampleNormalsMap->GetRenderID();

	m_viewerPanel = Elysium::CreateUnique<ViewerPanel>(&m_outputTextureId);
	m_controlsPanel = Elysium::CreateUnique<ControlsPanel>();
	m_propertiesPanel = Elysium::CreateUnique<PropertiesPanel>();

	m_scene = Elysium::CreateShared<Elysium::Scene>();

	m_sprite = m_scene->CreateEntity("Sprite_01");
	auto& rectComp = m_sprite.AddComponent<Elysium::RectTransformComponent>();
	rectComp.SetTranslation({ 0, 0 });
	rectComp.SetDimensions({ 100, 100 });
	rectComp.SetPivotOffset({ 50, 50 });
	auto& spriteComp = m_sprite.AddComponent<Elysium::SpriteComponent>();
	spriteComp.Texture = m_exampleNormalsMap;

	Elysium::FrameBufferSpecification bufferspecs;
	bufferspecs.Attachments = { Elysium::FrameBufferTextureFormat::RGBA8 };
	bufferspecs.Width = 200;
	bufferspecs.Height = 200;
	bufferspecs.SwapChainTarget = false;
	m_fbo = Elysium::FrameBuffer::Create(bufferspecs);

	m_outputTextureId = m_fbo->GetColorAttachementRendererID();

	m_textureShader = Elysium::ShaderFactory::Create("Content/Engine/shaders/default_sprite.shader");

	int samplers[Elysium::RendererCaps::MaxTextureSlots];
	for (int i = 0; i < Elysium::RendererCaps::MaxTextureSlots; ++i)
		samplers[i] = i;

	m_textureShader->Bind();
	m_textureShader->SetIntArray("textureMaps", samplers, Elysium::RendererCaps::MaxTextureSlots);
	m_textureShader->Unbind();
}

NormsEditorLayer::~NormsEditorLayer()
{
}

void NormsEditorLayer::OnAttach()
{
}

void NormsEditorLayer::OnDetach()
{
}

void NormsEditorLayer::OnUpdate()
{
	m_scene->Computations();

	// TODO:: Move into appropriate pass?
	Elysium::CoreUniformBuffers::UploadDirtyData();

	if (!m_textureShader->IsCompiled())
	{
		ELYSIUM_WARN("Texture Shader Not Compiled!");
		return;
	}

	m_textureShader->Bind();

	m_fbo->Bind();

	Elysium::GraphicsCalls::ClearBuffers();
	Elysium::GraphicsCalls::SetColor(Elysium::Math::Vec4(0.3f));
	m_scene->Draw(Elysium::Scene::SceneMask::_2D);

	m_fbo->Unbind();

	m_textureShader->Unbind();

	if (m_controlsPanel->HasChange())
	{
		auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();

		// Recenter the camera onto the sprite
		if (m_controlsPanel->GetRecenterFocus())
		{
			m_viewerPanel->FocusOnRect(rectComp);
		}

		rectComp.SetRotationDegrees(m_controlsPanel->GetRotationDegree());

		m_controlsPanel->FlushChangeState();
	}
}

void NormsEditorLayer::OnImGuiRender()
{
	static bool opt_fullscreen = true;
	static bool dockspaceOpen = true;
	static bool viewportOpen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoCloseButton;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		window_flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse;
	}

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN "  Open...", "Ctrl + O"))
			{

			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save", "Ctrl + S"))
			{

			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save As...", "Ctrl + Shift + S"))
			{

			}
			if (ImGui::MenuItem(ICON_FA_ARCHIVE "  Open Example", "Ctrl + E"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	m_viewerPanel->OnImGuiRender();
	m_controlsPanel->OnImGuiRender();
	m_propertiesPanel->OnImGuiRender();

	ImGui::End();

#if 0
	static bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo);
#endif
}

void NormsEditorLayer::OnEvent(Elysium::Event& _event)
{
	//ELYSIUM_TRACE(_event.ToString());

	Elysium::EventDispatcher dispatcher(_event);
	dispatcher.Dispatch<Elysium::WindowResizeEvent>(BIND_EVENT_FN(NormsEditorLayer::OnWindowResize));

	m_controlsPanel->OnEvent(_event);

	if (m_viewerPanel->IsHovered() || m_viewerPanel->IsFocused())
		m_viewerPanel->OnEvent(_event);
}

bool NormsEditorLayer::OnWindowResize(Elysium::WindowResizeEvent& _event)
{
	return false; // Pass through
}
