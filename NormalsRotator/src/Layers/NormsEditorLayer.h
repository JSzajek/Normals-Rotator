#pragma once

#include "Elysium.h"
#include "Elysium/Scene/Entity.h"

#include "ShaderPipeline.h"

class ViewerPanel;
class ControlsPanel;
class PropertiesPanel;

namespace cv
{
	class Mat;
}

class NormsEditorLayer : public Elysium::Layer
{
private:
	const std::string ExampleFilePath = "Content/textures/normal_map_example.png";
private:
	enum ModifierKeys : uint8_t
	{
		LeftShift	= 1,
		RightShift	= 2,
		LeftCtrl	= 3,
		RightCtrl	= 4,
	};
public:
	NormsEditorLayer();
	virtual ~NormsEditorLayer() override;
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Elysium::Event& _event) override;
private:
	bool OnWindowResize(Elysium::WindowResizeEvent& _event);
	bool OnKeyPressed(Elysium::KeyPressedEvent& _event);
	bool OnKeyReleased(Elysium::KeyReleasedEvent& _event);
private:
	void OpenFileDialog();
	void SaveFileDialog();
	void OpenFile(const std::string& filepath);
	void SaveCurrentImage(const std::string& outputFilepath);
	void CalculateOutputDimensions(bool clipToDimensions);
private:
	Elysium::Unique<ViewerPanel> m_viewerPanel;
	Elysium::Unique<ControlsPanel> m_controlsPanel;
	Elysium::Unique<PropertiesPanel> m_propertiesPanel;

	Elysium::Shared<Elysium::Texture2D> m_exampleNormalsMap;
	Elysium::Shared<Elysium::Texture2D> m_activeTexture;

	Elysium::Shared<Elysium::Scene> m_scene;

	Elysium::Shared<Elysium::FrameBuffer> m_fbo;
	Elysium::Shared<Elysium::FrameBuffer> m_normalsFBO;
	Elysium::Shared<Elysium::Shader> m_normalsRotatorShader;
	Elysium::Shared<Elysium::Shader> m_spriteShader;

	Elysium::Shared<Elysium::Shader> m_lineShader;

	Elysium::Entity m_sprite;

	std::string m_currentFilePath;
	bool m_isExampleFile;

	ShaderPipeline m_pipeline;
	uint32_t m_outputTextureId;

	Elysium::Math::iVec2 m_outputSize;
	Elysium::Math::iVec2 m_outputOffset;

	short m_modifierKeyFlag;
};
