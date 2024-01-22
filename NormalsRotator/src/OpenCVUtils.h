#pragma once

#include "Elysium/Math/iVec2.h"
#include "Elysium/Math/Vec2.h"

namespace cv
{
	class Mat;
}

class OpenCVUtils
{
public:
	static void SaveImage(cv::Mat* image, const std::string& outputFilepath,
						  float rotation_deg,
						  const Elysium::Math::Vec2& pivot,
						  const Elysium::Math::iVec2& clipOffset,
						  const Elysium::Math::iVec2& clipSize);
};