#include "norm_pch.h"

#include <Elysium.h>
#include <Elysium/Core/EntryPoint.h>

#include "Layers/NormsEditorLayer.h"

class SVisualizerApp : public Elysium::Application
{
public:
	SVisualizerApp()
		: Application("Normals Rotator", 1024, 600)
	{
		PushLayer(new NormsEditorLayer());
	}

	~SVisualizerApp()
	{
	}
};

Elysium::Application* Elysium::CreateApplication()
{
	return new SVisualizerApp();
}
