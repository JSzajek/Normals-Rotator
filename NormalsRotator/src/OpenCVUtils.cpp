#include "norm_pch.h"

#include "OpenCVUtils.h"

#include <opencv2/opencv.hpp>

void OpenCVUtils::SaveImage(cv::Mat* image, const std::string& outputFilepath,
							float rotation_deg, 
							const Elysium::Math::Vec2& pivot, 
							const Elysium::Math::iVec2& clipOffset,
							const Elysium::Math::iVec2& clipSize)
{
	cv::Point2f center(pivot.x, pivot.y);
	cv::Mat rotTranslationMat = cv::getRotationMatrix2D(center, -rotation_deg, 1);

	rotTranslationMat.at<double>(0, 2) += -clipOffset.x;
	rotTranslationMat.at<double>(1, 2) += -clipOffset.y;

	cv::Mat rotatedImage;
	cv::warpAffine(*image, rotatedImage, rotTranslationMat, cv::Size(clipSize.x, clipSize.y));

	cv::imwrite(outputFilepath, rotatedImage);
}
