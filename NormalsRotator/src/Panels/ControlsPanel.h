#pragma once

#include "Elysium.h"

class ControlsPanel
{
public:
	ControlsPanel();
public:
	void OnImGuiRender();

	inline bool HasChange() const { return m_changed; }
	void FlushChangeState();
public:
	void SetClipDimensions(const Elysium::Math::iVec4& clipDims) { m_clipDimensions = clipDims; }

	inline float GetRotationDegree() const { return m_rotationDegrees; }
	inline bool GetIsClipped() const { return m_isClipped; }
	inline Elysium::Math::iVec4  GetClipDimensions() const { return m_clipDimensions; }
	inline bool GetFlipHorizontal() const { return m_flipHorizontal; }
	inline bool GetFlipVertical() const { return m_flipVertical; }
	inline bool GetFlipRed() const { return m_flipRed; }
	inline bool GetFlipGreen() const { return m_flipGreen; }
	inline bool GetFlipBlue() const { return m_flipBlue; }
private:
	bool m_focused;
	bool m_hovered;

	float m_rotationDegrees;

	bool m_isClipped;
	Elysium::Math::iVec4 m_clipDimensions;

	bool m_flipHorizontal;
	bool m_flipVertical;
	bool m_flipRed;
	bool m_flipGreen;
	bool m_flipBlue;

	bool m_changed;
};
