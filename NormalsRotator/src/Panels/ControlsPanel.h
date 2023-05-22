#pragma once

#include "Elysium.h"

class ControlsPanel
{
public:
	ControlsPanel();
public:
	void OnImGuiRender();
	void OnEvent(Elysium::Event& _event);

	inline bool HasChange() const { return m_changed; }
	void FlushChangeState();

	bool OnKeyPressed(Elysium::KeyPressedEvent& _event);
public:
	inline bool GetRecenterFocus() const { return m_recenterFocus; }
	inline float GetRotationDegree() const { return m_rotationDegrees; }
	inline bool GetClipToOriginalDimensions() const { return m_clipOriginalDimensions; }
	inline bool GetFlipHorizontal() const { return m_flipHorizontal; }
	inline bool GetFlipVertical() const { return m_flipVertical; }
	inline bool GetFlipRed() const { return m_flipRed; }
	inline bool GetFlipGreen() const { return m_flipGreen; }
	inline bool GetFlipBlue() const { return m_flipBlue; }
private:
	bool m_focused;
	bool m_hovered;

	bool m_recenterFocus;
	float m_rotationDegrees;
	bool m_clipOriginalDimensions;

	bool m_flipHorizontal;
	bool m_flipVertical;
	bool m_flipRed;
	bool m_flipGreen;
	bool m_flipBlue;

	bool m_changed;
};
