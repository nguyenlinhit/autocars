#pragma once
#ifndef BASIC_OPERATIONS_HPP_INCLUDED
#define BASIC_OPERATIONS_HPP_INCLUDED

#include "opencv2/opencv.hpp"
#include "cv_headers.hpp"

namespace cv_lib
{
	bool double_equals(double a, double b, double epsilon = 0.001);

	int init_pic(const int& argc, char* argv[], std::string& strpicName, cv::Mat& myPic);
	int init_pic(const int& argc, char* argv[], std::vector<std::string>& strpicName, std::vector<cv::Mat>& myPic);

	int load_from_folder(const std::string relative_path, std::vector<std::string>& v_picName, std::vector<cv::Mat>& v_Pic);

	void print_info(const cv::Mat& myPic);
	void print_info(const std::vector<cv::Mat>& v_Pic);
	void print_info(const cv::Mat& myPic, const std::string& picName);
	void print_info(const std::vector<cv::Mat>& v_Pic, const std::vector<std::string>& v_picName);


	void show_pic(const cv::Mat& myPic);
	void show_pic(const std::vector<cv::Mat>& v_Pic);
	void show_pic(const std::vector<cv::Mat>& v_Pic, std::string name);
	// void show_pic(const Mat& myPic, bool save);

	void show_pic(const cv::Mat& myPic, const std::string name);
	void show_pic(const std::vector<cv::Mat>& v_Pic, const std::vector<std::string>& name);

	void show_pic(const cv::Mat& myPic, const std::string name, const bool save);


	void save_pic(const cv::Mat& Pic);
	void save_pic(const cv::Mat& Pic, const std::string str);
}

#endif  // BASIC_OPERATIONS_HPP_INCLUDED
