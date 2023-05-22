#include "norm_pch.h"

#include "NormsEditorLayer.h"

#include "Panels/ViewerPanel.h"
#include "Panels/ControlsPanel.h"
#include "Panels/PropertiesPanel.h"

#include "Elysium/Factories/ShaderFactory.h"

#include <imgui_internal.h>

#include <iconfontcppheaders/IconsFontAwesome5.h>

#include <opencv2/opencv.hpp>

NormsEditorLayer::NormsEditorLayer()
{
	m_isExampleFile = false;

	Elysium::TextureFormat format;
	format.Size = { 0, 0 };
	format.sRGB = false;
	format.FilePath = "";
	format.Format = Elysium::PixelFormat::R8G8B8;
	m_activeTexture = Elysium::Texture2D::Create(format);

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
	auto& gizmoRectComp = m_sprite.AddComponent<Elysium::GizmoRectComponent>();
	gizmoRectComp.SetTranslation({ 0, 0 });
	gizmoRectComp.SetDimensions({ 100, 100 });
	gizmoRectComp.Color = Elysium::Math::Vec4(0.75294f, 0.75294f, 0.75294f, 1.0f); 
	//gizmoRectComp.Color = Elysium::Math::Vec4(0.4f, 0.4f, 0.6f, 1.0f);
	gizmoRectComp.LineWidth = 3;

	Elysium::FrameBufferSpecification bufferspecs;
	bufferspecs.Attachments = { Elysium::FrameBufferTextureFormat::RGBA8 };
	bufferspecs.Width = 200;
	bufferspecs.Height = 200;
	bufferspecs.SwapChainTarget = false;

	m_fbo = Elysium::FrameBuffer::Create(bufferspecs);
	m_normalsFBO = Elysium::FrameBuffer::Create(bufferspecs);

	// Set the sprite component to render the results of the normals FBO
	spriteComp.Texture = Elysium::Texture2D::Create(m_normalsFBO->GetColorAttachementRendererID(), 0, 0);

	m_outputTextureId = m_fbo->GetColorAttachementRendererID();

	m_normalsRotatorShader = Elysium::ShaderFactory::Create("Content/shaders/normals_rotator.shader");
	m_spriteShader = Elysium::ShaderFactory::Create("Content/Engine/shaders/default_sprite.shader");
	m_lineShader = Elysium::ShaderFactory::Create("Content/Engine/shaders/default_line.shader");

	int samplers[Elysium::RendererCaps::MaxTextureSlots];
	for (int i = 0; i < Elysium::RendererCaps::MaxTextureSlots; ++i)
		samplers[i] = i;

	m_normalsRotatorShader->Bind();
	m_normalsRotatorShader->SetInt("normalMap", 0);
	m_normalsRotatorShader->Unbind();

	m_spriteShader->Bind();
	m_spriteShader->SetIntArray("textureMaps", samplers, Elysium::RendererCaps::MaxTextureSlots);
	m_spriteShader->Unbind();

	ELYSIUM_CORE_ASSERT(m_normalsRotatorShader->IsCompiled(), "Normals Rotator Shader Failed to Compile.");
	ELYSIUM_CORE_ASSERT(m_spriteShader->IsCompiled(), "Sprite Shader Failed to Compile.");
	ELYSIUM_CORE_ASSERT(m_lineShader->IsCompiled(), "Line Shader Failed to Compile.");
}

NormsEditorLayer::~NormsEditorLayer()
{
}

void NormsEditorLayer::OnAttach()
{
	OpenFile(Elysium::FileUtils::GetAssetPath_Str(ExampleFilePath));
	m_isExampleFile = true;
}

void NormsEditorLayer::OnDetach()
{
}

void NormsEditorLayer::OnUpdate()
{
	m_scene->Computations();

	// TODO:: Move into appropriate pass?
	Elysium::CoreUniformBuffers::UploadDirtyData();

	m_normalsRotatorShader->Bind();
	const float rotation_rad = Elysium::Math::ToRadians(m_controlsPanel->GetRotationDegree());
	m_normalsRotatorShader->SetFloat("rotation_rad", rotation_rad);

	Elysium::GraphicsCalls::ClearBuffers();
	Elysium::RenderCommands::DrawTexture(m_normalsFBO, Elysium::TextureDrawType::Color, m_activeTexture, m_normalsRotatorShader);
	m_normalsRotatorShader->Unbind();

	m_fbo->Bind();
	m_spriteShader->Bind();

	Elysium::GraphicsCalls::ClearBuffers();
	Elysium::GraphicsCalls::SetColor(Elysium::Math::Vec4(0.3f));
	m_scene->Draw(Elysium::Scene::SceneMask::_2D);

	m_spriteShader->Unbind();

	m_lineShader->Bind();
	m_scene->Draw(Elysium::Scene::SceneMask::Gizmo2D);
	m_lineShader->Unbind();
	m_fbo->Unbind();

	if (m_viewerPanel->HasViewSizeChange())
	{
		Elysium::Math::iVec2 panelSize = m_viewerPanel->GetSize();

		if (panelSize.width > 0 && panelSize.height > 0)
			OnWindowResize(Elysium::WindowResizeEvent(panelSize.width, panelSize.height));
	}

	if (m_controlsPanel->HasChange())
	{
		auto& spriteComp = m_sprite.GetComponent<Elysium::SpriteComponent>();
		auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();

		// Recenter the camera onto the sprite
		if (m_controlsPanel->GetRecenterFocus())
		{
			if (spriteComp.Texture->GetWidth() > 0 && spriteComp.Texture->GetHeight() > 0) 
				m_viewerPanel->FocusOnRect(rectComp);
		}

		rectComp.SetRotationDegrees(m_controlsPanel->GetRotationDegree());
		CalculateOutputDimensions(m_controlsPanel->GetClipToOriginalDimensions());

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
				OpenFileDialog();
				m_isExampleFile = false;
			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save", "Ctrl + S", nullptr, !m_isExampleFile))
			{
				SaveCurrentImage(m_currentFilePath);
			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save As...", "Ctrl + Shift + S"))
			{
				SaveFileDialog();
			}
			if (ImGui::MenuItem(ICON_FA_ARCHIVE "  Open Example", "Ctrl + E"))
			{
				OpenFile(Elysium::FileUtils::GetAssetPath_Str(ExampleFilePath));
				m_isExampleFile = true;
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
	dispatcher.Dispatch<Elysium::KeyPressedEvent>(BIND_EVENT_FN(NormsEditorLayer::OnKeyPressed));
	dispatcher.Dispatch<Elysium::KeyReleasedEvent>(BIND_EVENT_FN(NormsEditorLayer::OnKeyReleased));

	m_controlsPanel->OnEvent(_event);

	if (m_viewerPanel->IsHovered() || m_viewerPanel->IsFocused())
		m_viewerPanel->OnEvent(_event);
}

bool NormsEditorLayer::OnWindowResize(Elysium::WindowResizeEvent& _event)
{
	Elysium::Math::iVec2 panelSize = m_viewerPanel->GetSize();

	uint32_t windowWidth = panelSize.width;
	uint32_t windowHeight = panelSize.height;

	// partially super-sample
	const float superSampleAmount = 1.25f;
	windowWidth = static_cast<uint32_t>(windowWidth * superSampleAmount);
	windowHeight = static_cast<uint32_t>(windowHeight * superSampleAmount);

	m_fbo->Resize(windowWidth, windowHeight);

	return false; // Pass through
}

bool NormsEditorLayer::OnKeyPressed(Elysium::KeyPressedEvent& _event)
{
	switch (_event.GetKeyCode())
	{
		case Elysium::Key::LeftShift:
			BIT_SET(m_modifierKeyFlag, ModifierKeys::LeftShift);
			break;
		case Elysium::Key::RightShift:
			BIT_SET(m_modifierKeyFlag, ModifierKeys::RightShift);
			break;
		case Elysium::Key::LeftControl:
			BIT_SET(m_modifierKeyFlag, ModifierKeys::LeftCtrl);
			break;
		case Elysium::Key::RightControl:
			BIT_SET(m_modifierKeyFlag, ModifierKeys::RightCtrl);
			break;
		case Elysium::Key::O:
		{
			if (BIT_CHECK(m_modifierKeyFlag, ModifierKeys::LeftCtrl) || BIT_CHECK(m_modifierKeyFlag, ModifierKeys::RightCtrl))
			{
				OpenFileDialog();
			}
			break;
		}
	}
	return false;
}

bool NormsEditorLayer::OnKeyReleased(Elysium::KeyReleasedEvent& _event)
{
	switch (_event.GetKeyCode())
	{
		case Elysium::Key::LeftShift:
			BIT_CLEAR(m_modifierKeyFlag, ModifierKeys::LeftShift);
			break;
		case Elysium::Key::RightShift:
			BIT_CLEAR(m_modifierKeyFlag, ModifierKeys::RightShift);
			break;
		case Elysium::Key::LeftControl:
			BIT_CLEAR(m_modifierKeyFlag, ModifierKeys::LeftCtrl);
			break;
		case Elysium::Key::RightControl:
			BIT_CLEAR(m_modifierKeyFlag, ModifierKeys::RightCtrl);
			break;
	}
	return false;
}

void NormsEditorLayer::OpenFileDialog()
{
	const std::string filepath = Elysium::FileDialogs::OpenFile("All Supported Formats (*.png, *.jpg, *.jpeg, *.jpe, *.jp2, *.tiff, *.tif, *.bmp)\0*.png;*.jpg;*.jpeg;*.jpe;*.jp2;*.tiff;*.tif;*.bmp\0"
																"PNG Image (*.png)\0*.png\0"
																"JPEG Image (*.jpg, *.jpeg, *.jpe)\0*.jpg;*.jpeg;*.jpe\0"
																"JPEG-2000 Image (*.jp2)\0*.jp2\0"
																"TIFF Image (*.tiff, *.tif)\0*.tiff;*.tif\0"
																"Windows BMP Image (*.bmp)\0*.bmp\0");

	if (!filepath.empty())
		OpenFile(filepath);
}

void NormsEditorLayer::SaveFileDialog()
{
	const std::string filepath = Elysium::FileDialogs::SaveFile("PNG Image (*.png)\0*.png\0"
																"JPEG Image (*.jpg, *.jpeg, *.jpe)\0*.jpg;*.jpeg;*.jpe\0"
																"JPEG-2000 Image (*.jp2)\0*.jp2\0"
																"TIFF Image (*.tiff, *.tif)\0*.tiff;*.tif\0"
																"Windows BMP Image (*.bmp)\0*.bmp\0");

	if (!filepath.empty())
		SaveCurrentImage(filepath);
}

void NormsEditorLayer::OpenFile(const std::string& filepath)
{
	if (!Elysium::FileUtils::FileExists(filepath))
	{
		ELYSIUM_WARN("Failed To Open File: {0}", filepath);
		return;
	}

	// Open the file in BGR format (no alpha channel) 8-bit.
	cv::Mat img = cv::imread(filepath, cv::IMREAD_COLOR);

	const uint32_t width = img.cols;
	const uint32_t height = img.rows;
	if (width <= 0 || height <= 0)
	{
		ELYSIUM_WARN("Invalid Image File Size: {0}", filepath);
		return;
	}

	cv::Mat convertedImg;
	cv::cvtColor(img, convertedImg, cv::COLOR_BGR2RGB);
	img.release();

	m_activeTexture->Resize(width, height);
	const uint32_t dataSize = width * height * 3;
	m_activeTexture->SetData((void*)convertedImg.data, dataSize);

	m_normalsFBO->Resize(width, height);

	if (width > 0 && height > 0)
	{
		auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();
		auto& spriteComp = m_sprite.GetComponent<Elysium::SpriteComponent>();

		rectComp.SetDimensions({ (float)width, (float)height });
		rectComp.SetPivotOffset({ width * 0.5f, height * 0.5f });
		m_viewerPanel->FocusOnRect(rectComp);

		CalculateOutputDimensions(m_controlsPanel->GetClipToOriginalDimensions());
	}

	m_currentFilePath = filepath;
}

void NormsEditorLayer::SaveCurrentImage(const std::string& outputFilepath)
{
	if (m_isExampleFile)
		return;

	// Retrieve the fbo data that represents the rotated normals
	const uint32_t img_width = m_normalsFBO->GetColorAttachment(0)->GetWidth();
	const uint32_t img_height = m_normalsFBO->GetColorAttachment(0)->GetHeight();

	cv::Mat currentImage(img_width, img_height, CV_8UC4, cv::Scalar(255, 255, 255, 0));
	
	const uint32_t datasize = img_width * img_height * 4;
	m_normalsFBO->Bind();
	uint8_t* pixelData = m_normalsFBO->ReadPixelBuffer(0, 0, 0, img_width, img_height);
	m_normalsFBO->Unbind();
	std::memcpy(currentImage.data, pixelData, datasize);
	delete[] pixelData;

	cv::Mat convertedImg;
	cv::cvtColor(currentImage, convertedImg, cv::COLOR_RGBA2BGRA);
	currentImage.release();

	auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();
	const Elysium::Math::Vec2 dimensions = rectComp.GetDimensions();

	cv::Point2f center(dimensions.x * 0.5f, dimensions.y * 0.5f);
	cv::Mat rotTranslationMat = cv::getRotationMatrix2D(center, -m_controlsPanel->GetRotationDegree(), 1);

	rotTranslationMat.at<double>(0, 2) += -m_outputOffset.x;
	rotTranslationMat.at<double>(1, 2) += -m_outputOffset.y;

	cv::Mat rotatedImage;
	cv::warpAffine(convertedImg, rotatedImage, rotTranslationMat, cv::Size(m_outputSize.x, m_outputSize.y));

	//ELYSIUM_INFO("Writing File to {0}", outputFilepath);
	cv::imwrite(outputFilepath, rotatedImage);
}

void NormsEditorLayer::CalculateOutputDimensions(bool clipToDimensions)
{
	if (clipToDimensions)
	{
		auto& gizmoBoundsComp = m_sprite.GetComponent<Elysium::GizmoRectComponent>();
		auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();

		const Elysium::Math::Vec2 translation = rectComp.GetTranslation();
		const Elysium::Math::Vec2 dimension = rectComp.GetDimensions();

		gizmoBoundsComp.SetTranslation(translation);
		gizmoBoundsComp.SetDimensions(dimension);

		m_outputSize.x = static_cast<int32_t>(dimension.x);
		m_outputSize.y = static_cast<int32_t>(dimension.y);

		m_outputOffset.x = static_cast<int32_t>(translation.x);
		m_outputOffset.y = static_cast<int32_t>(translation.y);
	}
	else
	{
		// Calculate the bounding box from the rotated sprite
		auto& rectComp = m_sprite.GetComponent<Elysium::RectTransformComponent>();
		const auto transform = rectComp.GetTransform();
		Elysium::Math::Vec3 bottomLeft = transform * Elysium::Math::Vec3(0, 0, 0);
		Elysium::Math::Vec3 bottomRight = transform * Elysium::Math::Vec3(1, 0, 0);
		Elysium::Math::Vec3 topRight = transform * Elysium::Math::Vec3(1, 1, 0);
		Elysium::Math::Vec3 topLeft = transform * Elysium::Math::Vec3(0, 1, 0);

		const float maxX = std::max(std::max(std::max(bottomLeft.x, topRight.x), topLeft.x), bottomRight.x);
		const float minX = std::min(std::min(std::min(bottomLeft.x, topRight.x), topLeft.x), bottomRight.x);
		const float maxY = std::max(std::max(std::max(bottomLeft.y, topRight.y), topLeft.y), bottomRight.y);
		const float minY = std::min(std::min(std::min(bottomLeft.y, topRight.y), topLeft.y), bottomRight.y);

		m_outputSize.x = static_cast<int32_t>(maxX - minX);
		m_outputSize.y = static_cast<int32_t>(maxY - minY);

		m_outputOffset.x = static_cast<int32_t>(minX);
		m_outputOffset.y = static_cast<int32_t>(minY);

		// Update the visualizer with the output dimensions
		auto& gizmoBoundsComp = m_sprite.GetComponent<Elysium::GizmoRectComponent>();
		gizmoBoundsComp.SetDimensions({ (float)m_outputSize.x, (float)m_outputSize.y });
		gizmoBoundsComp.SetTranslation({ (float)m_outputOffset.x, (float)m_outputOffset.y });
	}
}
