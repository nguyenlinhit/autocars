#pragma once
#ifndef Sign_Detection_Header_HPP_INCLUDED
#define Sign_Detection_Header_HPP_INCLUDED

#include "opencv2/opencv.hpp"

// For HSV,
// Hue range is       [0,179]
// Saturation range   [0,255]
// Value range        [0,255]

const int MAX_HUE(180);

const int MAX_VALUE(255);  // Range Max.
const int MAX_SATURATION(255);

const int MIN_VALUE(150);  // Not range Min, but filter parameter
const int MIN_SATURATION(150);


const int RED_RNG(15);
const int BLUE_RNG(20);

const cv::Scalar RED_HSV_UPPER_LOW(MAX_HUE - RED_RNG, MIN_VALUE, MIN_SATURATION);
const cv::Scalar RED_HSV_UPPER_HIGH(MAX_HUE, MAX_VALUE, MAX_SATURATION);
const cv::Scalar RED_HSV_LOWER_LOW(0, MIN_VALUE, MIN_SATURATION);
const cv::Scalar RED_HSV_LOWER_HIGH(0 + RED_RNG, MAX_VALUE, MAX_SATURATION);

const int HSV_BLUE(115);

const cv::Scalar BLUE_HSV_LOW(HSV_BLUE - BLUE_RNG, MIN_VALUE, MIN_SATURATION);
const cv::Scalar BLUE_HSV_HIGH(HSV_BLUE + BLUE_RNG, MAX_VALUE, MAX_SATURATION);


#endif // Sign_Detection_Header_HPP_INCLUDED
