#include "norm_pch.h"
#include "ShaderPipeline.h"

#include "Elysium/Factories/ShaderFactory.h"
#include "Elysium/Graphics/RenderCommands.h"

ShaderPipeline::ShaderPipeline()
{
	Elysium::FrameBufferSpecification bufferspecs;
	bufferspecs.Attachments = { Elysium::FrameBufferTextureFormat::RGBA8 };
	bufferspecs.Width = 200;
	bufferspecs.Height = 200;
	bufferspecs.SwapChainTarget = false;
	m_buffer = Elysium::FrameBuffer::Create(bufferspecs);

	m_shader = Elysium::ShaderFactory::Create("Content/Engine/shaders/default.shader");
}

ShaderPipeline::~ShaderPipeline()
{
}

void ShaderPipeline::Resize(uint32_t width, uint32_t height)
{
	m_buffer->Resize(width, height);
}

void ShaderPipeline::OnUpdate()
{

}

void ShaderPipeline::Render()
{
	Elysium::RenderCommands::DrawShader(m_buffer, m_shader);
}
