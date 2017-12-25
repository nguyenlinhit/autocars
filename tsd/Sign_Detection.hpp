#pragma once
#ifndef Sign_Detection_HPP_INCLUDED
#define Sign_Detection_HPP_INCLUDED

#include "cv_headers.hpp"
#include "cv_basic_operations.hpp"
#include "cv_library.hpp"

#include "Sign_Detection_header.hpp"

template <typename Enumeration>                            // src: https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
constexpr auto as_integer(Enumeration const value)
-> typename std::underlying_type<Enumeration>::type
{
	static_assert(std::is_enum<Enumeration>::value, "parameter is not of type enum or enum class");
	return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}


enum class Shape { unknown_shape, circular, rectangular, triangular };

enum class Color { unknown_color, red, blue };

// enum class Sign_type {unknown_name, stop, prohibido, no_circulacion, circulacion_obligatoria, ceda, peaton, aparcamiento, paso_zebra, minusvalidos, taxi};

class Sign
{
public:
	Sign()
		:shape(Shape::unknown_shape), color(Color::unknown_color)//, sign_type(Sign_type::unknown_name)
	{}

	std::string name;
	Shape shape;
	Color color;
	// Sign_type sign_type;

	cv::Rect roi;

};

class Data_pic
{
public:
	Data_pic() {};

	int index;
	std::string picName;
	cv::Mat Pic_color;
	cv::Mat Pic_color_high_contrast;
	cv::Mat Pic_grey;
	cv::Mat Pic_th;         // Unused
	cv::Mat Pic_HSV;
	cv::Mat Pic_HSV_red;
	cv::Mat Pic_HSV_blue;

	std::vector<std::vector<cv::Point> >  v_red_contours;
	std::vector<std::vector<cv::Point> >  v_blue_contours;

	std::vector<Sign> v_signs;
};

class Sign_Detection
{
public:
	Sign_Detection();
	Sign_Detection(const std::string relative_samples_path, const std::string relative_templates_path);
	~Sign_Detection() {};

	// int load_templates(const int& argc, char* argv[]);
	int load_templates();

	void extract_red_HSV(Data_pic& data);
	void extract_blue_HSV(Data_pic& data);
	void filter_red_contours(Data_pic& data);
	void filter_blue_contours(Data_pic& data);
	void normalize_templates();
	void print_info(Sign sign);

	bool                      get_loadStatus()            const { return loadStatus; }
	std::vector<Data_pic>     get_v_data()                const { return v_data; }
	std::vector<cv::Mat>      get_v_templates_red()       const { return v_templates_red; }
	std::vector<cv::Mat>      get_v_templates_blue()      const { return v_templates_blue; }
	std::vector<std::string>  get_v_picNames_red()        const { return v_picNames_red; }
	std::vector<std::string>  get_v_picNames_blue()       const { return v_picNames_blue; }
private:
	bool loadStatus;
	std::vector<Data_pic>     v_data;
	std::vector<cv::Mat>      v_templates_red;
	std::vector<cv::Mat>      v_templates_blue;
	std::vector<std::string>  v_picNames_red;
	std::vector<std::string>  v_picNames_blue;
	std::string samples_path;
	std::string templates_path;
};


#endif // Sign_Detection_HPP_INCLUDED