#pragma once

#include "Elysium.h"

class ShaderPipeline
{
private:
	Elysium::Shared<Elysium::FrameBuffer> m_buffer;
	Elysium::Shared<Elysium::Shader> m_shader;
public:
	ShaderPipeline();
	virtual ~ShaderPipeline();

	inline uint32_t GetOutput() const { return m_buffer->GetColorAttachment()->GetRenderID(); }
	inline uint32_t GetOutputWidth() const { return m_buffer->GetColorAttachment()->GetWidth(); }
	inline uint32_t GetOutputHeight() const { return m_buffer->GetColorAttachment()->GetHeight(); }

	void Resize(uint32_t width, uint32_t height);

	void OnUpdate();
	void Render();
};
