#pragma once
#ifndef HEADERS_INCLUDED
#define HEADERS_INCLUDED

#include "opencv2/opencv.hpp"

#define stringify(name) # name  // returns a string with the name of the variable

namespace cv_lib
{
	const int CHL = 100;   // Char length
	const int MAX_HEIGHT(512);
	const int MAX_WIDTH(860);

	const int HISTO_SIZE(256);     // 256 grey levels
	const int HISTO_WIDTH(1024);    // 4 units/level
	const int HISTO_HEIGHT(512);

	const cv::Scalar BLACK(0, 0, 0);
	const cv::Scalar PSEUDO_BLACK(1, 1, 1);
	const cv::Scalar WHITE(255, 255, 255);
	const cv::Scalar RED(0, 0, 255);
	const cv::Scalar GREEN(0, 255, 0);
	const cv::Scalar BLUE(255, 0, 0);
	const cv::Scalar ORANGE(0, 128, 255);
	const cv::Scalar YELLOW(0, 255, 255);
	const cv::Scalar PINK(128, 0, 255);
	const cv::Scalar PURPLE(102, 0, 205);

	const double PI = 3.14159264;
}

#endif // HEADERS_INCLUDED
