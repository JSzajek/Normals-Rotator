#pragma once

#include "Elysium.h"
#include "Elysium/Scene/Entity.h"

#include "ShaderPipeline.h"

class ViewerPanel;
class ControlsPanel;
class PropertiesPanel;

class NormsEditorLayer : public Elysium::Layer
{
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
private:
	Elysium::Unique<ViewerPanel> m_viewerPanel;
	Elysium::Unique<ControlsPanel> m_controlsPanel;
	Elysium::Unique<PropertiesPanel> m_propertiesPanel;

	Elysium::Shared<Elysium::Texture2D> m_exampleNormalsMap;

	Elysium::Shared<Elysium::Scene> m_scene;

	Elysium::Shared<Elysium::FrameBuffer> m_fbo;
	Elysium::Shared<Elysium::Shader> m_textureShader;

	Elysium::Entity m_sprite;

	ShaderPipeline m_pipeline;
	uint32_t m_outputTextureId;
};
