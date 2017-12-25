#pragma once
#ifndef CV_LIBRARY_HPP_INCLUDED
#define CV_LIBRARY_HPP_INCLUDED

#include "opencv2/opencv.hpp"
#include "cv_headers.hpp"
#include "cv_basic_operations.hpp"   // show_pic used in some functions
#include <memory>

namespace cv_lib
{
	// using namespace cv;   // We asume

	struct UserData {  // Used in trackbars
		cv::Mat Pic_src;
		cv::Mat Pic_dst;
		cv::Point2i pt;
		cv::Mat hist;
		std::vector< std::vector<cv::Point> > contours;
		int angle = 180;
		int brightness = 100;
		int contrast = 100;
		int thresh = 100;
	};

	void grey_pic(const cv::Mat& Pic_original, cv::Mat& Pic_grey);
	void grey_pic(const std::vector<cv::Mat>& v_Pic, std::vector<cv::Mat>& v_Pic_grey);

	void color_pic(const cv::Mat& Pic_original, cv::Mat& Pic_color);
	void color_pic(const std::vector<cv::Mat>& v_Pic, std::vector<cv::Mat>& v_Pic_color);

	void manual_binarize(const cv::Mat& myPic, cv::Mat& binPic, const int threshold);

	void th_pic(const cv::Mat& Pic_original, cv::Mat& Pic_th, const int _th = 125, const int type = CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	void th_pic(const std::vector<cv::Mat>& v_Pic_original, std::vector<cv::Mat>& v_Pic_th, const int _th = 125, const int type = CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

	void HSV_pic(const cv::Mat& Pic_BGR, cv::Mat& Pic_HSV);
	void HSV_pic(const std::vector<cv::Mat>& v_Pic_BGR, std::vector<cv::Mat>& v_Pic_HSV);

	void rescale_pic(const cv::Mat& Pic_original, cv::Mat& Pic_rescaled, const float coef);
	void rescale_pic(const cv::Mat& Pic_original, cv::Mat& Pic_rescaled, const float coef_x, const float coef_y);

	void fix_size(const cv::Mat& Pic_original, cv::Mat& Pic_rescaled, const int max_height = MAX_HEIGHT, const int max_width = MAX_WIDTH);
	void fix_size(cv::Mat& Pic, const int max_height = MAX_HEIGHT, const int max_width = MAX_WIDTH);
	void fix_size(std::vector<cv::Mat>& Pic, const int max_height = MAX_HEIGHT, const int max_width = MAX_WIDTH);
	void fix_size(std::vector<cv::Mat>& v_Pic_src, std::vector<cv::Mat>& v_Pic_dst, const int max_height = MAX_HEIGHT, const int max_width = MAX_WIDTH);

	void rotate_pic(const cv::Mat& Pic_src, cv::Mat& Pic_rotated, const cv::Point2f center, const double angle);
	void rotate_pic(cv::Mat& Pic, const cv::Point2f center, const double angle);

	void draw_rectangle(cv::Mat& Pic, const cv::Point2f(&vertices)[4], const cv::Scalar color = BLACK, const int thickness = 1, const cv::LineTypes linetype = cv::LINE_8);
	std::unique_ptr<cv::Rect> light_rectangle(const cv::Mat& myPic, const int threshold);
	void light_rectangle(const cv::Mat& Pic_original, const int threshold, int& xleft, int& xright, int& yhigh, int& ylow);

	void fill_no_rectangle(const cv::Mat& Pic_original, cv::Mat& Pic_clean, const cv::Rect& _rectangle, const cv::Scalar& color = BLACK);
	void fill_no_rectangle(cv::Mat& Pic, const cv::Rect& _rectangle, const cv::Scalar& color = BLACK);
	void fill_no_rectangles(const cv::Mat& Pic_original, cv::Mat& Pic_clean, const std::vector<cv::Rect>& v_rectangle, const cv::Scalar& color = BLACK);
	void fill_no_rectangles(cv::Mat& Pic, const std::vector<cv::Rect>& v_rectangle, const cv::Scalar& color = BLACK);

	void get_roi_from_boundingRect(const cv::Mat& Pic, const std::vector<cv::Rect>& v_bounding_rect, cv::Rect& roi, int& lefter, int& righter, int& lower, int& higher);
	void get_roi_from_boundingRect(const cv::Mat& Pic, const std::vector<cv::Rect>& v_bounding_rect, cv::Rect& roi);

	void set_Brightness_Contrast(const cv::Mat& Pic_original, const int& brightness, const int& contrast, cv::Mat& Pic_final);

	void create_contours(const cv::Mat& Pic_src, cv::Mat& Pic_dst, std::vector<std::vector<cv::Point>>& contours);
	void create_contours(cv::Mat& Pic, std::vector<std::vector<cv::Point>>& contours);

	int count_objects(const cv::Mat& Pic, cv::Mat& Pic_dst);

	void morpho_pic(const cv::Mat& src, cv::Mat& dst, int iter, cv::MorphTypes type, cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F), cv::Point anchor = cv::Point(-1, -1));
	void morpho_pic(const std::vector<cv::Mat>& v_src, std::vector<cv::Mat>& v_dst, int iter, cv::MorphTypes type, cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F), cv::Point anchor = cv::Point(-1, -1));
	void morpho_pic(const cv::Mat& src, cv::Mat& dst, int iter, cv::MorphTypes type, cv::Point anchor, cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F));
	void morpho_pic(const std::vector<cv::Mat>& v_src, std::vector<cv::Mat>& v_dst, int iter, cv::MorphTypes type, cv::Point anchor, cv::Mat kernel = cv::Mat::ones(3, 3, CV_32F));

	// Histograms
	void create_histo(const cv::Mat& Pic, cv::Mat& hist, const cv::Scalar color);
	void create_histo(const cv::Mat& Pic, cv::Mat& hist);
	void create_histo(const cv::Mat& Pic, std::vector<cv::Mat>& v_hist);
	void create_histo(const std::vector<cv::Mat>& v_Pic, std::vector<cv::Mat>& v_hist);
	void create_histo(const cv::Mat& Pic);
	void draw_histo(cv::Mat& Pic_histo, const cv::Mat& histo, const cv::Scalar color);
	void show_histo(cv::Mat& Pic_histo);

	// Trackbars
	void rotation_trackbar(int, void* userdata);
	void brightness_contrast_trackbar(int, void* userdata);
	void thresh_trackbar(int, void* userdata);

	bool template_matching(const cv::Mat& pic, const cv::Rect& roi, const cv::Mat& templ, const double coef_correlation = 0.75, const int match_method = CV_TM_CCORR_NORMED, const double reduction_coef = 0.9, const int size_difference = 10);

	double match_percentage(const cv::Mat& Pic1, const cv::Mat& Pic2);
}

#endif // CV_LIBRARY_HPP_INCLUDED
