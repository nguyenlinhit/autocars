#include "Sign_Detection.hpp"

using namespace cv;
using namespace cv_lib;

Sign_Detection::Sign_Detection()
{
	assert(0 && "This constructor is not intended to be used");
}


Sign_Detection::Sign_Detection(const std::string relative_samples_path, const std::string relative_templates_path)
	: samples_path(relative_samples_path), templates_path(relative_templates_path)
{
	std::vector<std::string> v_picName;
	std::vector<cv::Mat> v_loaded_pics;
	loadStatus = load_from_folder(samples_path, v_picName, v_loaded_pics);
	
	for (unsigned i = 0; i<v_loaded_pics.size(); ++i)
	{
		Data_pic instance;
		instance.index = i;
		color_pic(v_loaded_pics[i], instance.Pic_color);
		fix_size(instance.Pic_color, instance.Pic_color);
		grey_pic(instance.Pic_color, instance.Pic_grey);
		th_pic(instance.Pic_grey, instance.Pic_th);
		HSV_pic(instance.Pic_color, instance.Pic_HSV);

		v_data.push_back(instance);
	}
}

int Sign_Detection::load_templates() {
	int return_value = 0;

	// Red templates

	std::vector<cv::String> v_paths;

	cv::String _pattern_red(templates_path + "red_templates/");
	cv::glob(_pattern_red, v_paths);

	v_templates_red.reserve(v_paths.size());
	v_picNames_red.reserve(v_paths.size());

	for (unsigned i = 0; i<v_paths.size(); ++i)
	{
		Mat myPic = imread(v_paths[i], CV_LOAD_IMAGE_UNCHANGED);

		if (myPic.empty())
		{
			std::cout << "Error opening picture file " << v_paths[i] << std::endl;
			// return -1;
			return_value = -1;
			continue;
		}
		v_templates_red.push_back(myPic);

		char chpicName[CHL];
		strcpy_s(chpicName, v_paths[i].c_str());

		std::string picName(chpicName);

		int n1 = picName.find("red_templates/");
		picName.erase(picName.begin(), picName.begin() + 25);
		v_picNames_red.push_back(std::string(picName));
	}

	// Blue templates

	cv::String _pattern_blue(templates_path + "blue_templates/");
	cv::glob(_pattern_blue, v_paths);

	v_templates_blue.reserve(v_paths.size());
	v_picNames_blue.reserve(v_paths.size());

	for (unsigned i = 0; i<v_paths.size(); ++i)
	{
		Mat myPic = imread(v_paths[i], CV_LOAD_IMAGE_UNCHANGED);

		if (myPic.empty())
		{
			std::cout << "Error opening picture file " << v_paths[i] << std::endl;
			// return -1;
			return_value = -1;
			continue;
		}
		v_templates_blue.push_back(myPic);

		char chpicName[CHL];
		strcpy_s(chpicName, v_paths[i].c_str());

		std::string picName(chpicName);


		picName.erase(picName.begin(), picName.begin()  + 28);
		v_picNames_blue.push_back(std::string(picName));
	}

	return return_value;
}

void Sign_Detection::normalize_templates() {
	fix_size(v_templates_red);
	fix_size(v_templates_blue);
}

void Sign_Detection::extract_red_HSV(Data_pic& data) {
	Mat red_hue;

	Mat red_hue_upper;
	cv::inRange(data.Pic_HSV, RED_HSV_UPPER_LOW, RED_HSV_UPPER_HIGH, red_hue_upper);
	Mat red_hue_lower;
	cv::inRange(data.Pic_HSV, RED_HSV_LOWER_LOW, RED_HSV_LOWER_HIGH, red_hue_lower);

	red_hue = (red_hue_lower | red_hue_upper);
	th_pic(red_hue, red_hue, 125, CV_THRESH_BINARY | CV_THRESH_OTSU);
	data.Pic_HSV_red = red_hue;

	// show_pic(red_hue, "HSV red th");
}

void Sign_Detection::extract_blue_HSV(Data_pic& data) {
	Mat blue_hue;

	cv::inRange(data.Pic_HSV, BLUE_HSV_LOW, BLUE_HSV_HIGH, blue_hue);
	th_pic(blue_hue, blue_hue, 125, CV_THRESH_BINARY | CV_THRESH_OTSU);
	data.Pic_HSV_blue = blue_hue;

	// show_pic(blue_hue, "HSV blue th");
}

void Sign_Detection::filter_red_contours(Data_pic& data) {
	Mat Pic = data.Pic_color.clone();

	// std::vector<Mat> v_Pic_roi;
	// std::vector<RotatedRect>  v_rotated_rect;
	// std::vector<RotatedRect>  v_ellipse;
	// v_rotated_rect.reserve(data.v_red_contours.size());
	// v_ellipse.reserve(data.v_red_contours.size());
	// v_Pic_roi.reserve(data.v_red_contours.size());
	double min_area = 0.1 * sqrt(static_cast<double>(data.Pic_HSV_red.rows) * static_cast<double>(data.Pic_HSV_red.cols));
	const double coef_x = 0.5, coef_y = 0.5;
	const double min_circularity = 0.70;
	const double acceptable_circularity = 0.50;
	// std::cout<<min_area<<std::endl;

	for (int index = 0; index<data.v_red_contours.size(); ++index)
	{
		auto contour = data.v_red_contours[index];
		auto rotated_rect = minAreaRect(contour);

		double area = contourArea(contour, false);
		double perimeter = arcLength(contour, true);
		double circularity = 4 * PI*area / (perimeter*perimeter);

		if (area > min_area && rotated_rect.size.height > coef_y*min_area && rotated_rect.size.width > coef_x*min_area)
		{
			// std::cout<<"Circularity: "  << circularity << std::endl;

			std::vector<cv::Point> poly;
			cv::approxPolyDP(contour, poly, 0.02*perimeter, true);

			// std::cout<<"Lados: "<<poly.size()<<std::endl;

			if (circularity>min_circularity || poly.size() == 3 || poly.size() == 8)
			{
				if (circularity>min_circularity)
				{
					Sign found;
					found.color = Color::red;
					found.shape = Shape::circular;

					found.roi = boundingRect(contour);

					// if(poly.size()==8)                       // Ojalá
					//   found.sign_type= Sign_type::stop;

					data.v_signs.push_back(found);
				}
				else if (circularity>acceptable_circularity && poly.size() == 3)
				{
					Sign found;
					found.color = Color::red;
					found.shape = Shape::triangular;

					data.v_signs.push_back(found);
				}

				// for(int i=0; i<poly.size(); ++i)            // DEBUG
				// {
				//   line(Pic, poly[i], poly[i+1], GREEN, 1);
				//   if(i==poly.size()-1)
				//     line(Pic, poly[i], poly[0], GREEN, 1);
				// }
				// show_pic(Pic, "poly");
				// v_ellipse.push_back(fitEllipse(contour));   // DEBUG
				// v_rotated_rect.push_back(rotated_rect);
				//
				// drawContours(Pic, data.v_red_contours, index, GREEN, 1);      // Debug
				// ellipse(Pic, v_ellipse.back(), YELLOW, 1);                    // Debug
				// Point2f rect_points[4]; rotated_rect.points(rect_points);     // Debug
				// draw_rectangle(Pic, rect_points, BLUE);                       // Debug
				//
				// show_pic(Pic, "contours");
				// waitKey();
			}
		}
	}
	// destroyAllWindows();
}

void Sign_Detection::filter_blue_contours(Data_pic& data) {
	Mat Pic = data.Pic_color.clone();

	// std::vector<Mat> v_Pic_roi;
	// std::vector<RotatedRect>  v_rotated_rect;
	// std::vector<RotatedRect>  v_ellipse;
	// v_rotated_rect.reserve(data.v_blue_contours.size());
	// v_ellipse.reserve(data.v_blue_contours.size());
	// v_Pic_roi.reserve(data.v_blue_contours.size());
	double min_area = 0.1 * sqrt(static_cast<double>(data.Pic_HSV_blue.rows) * static_cast<double>(data.Pic_HSV_blue.cols));
	const double coef_x = 0.5, coef_y = 0.5;
	const double min_circularity = 0.70;
	const double acceptable_circularity = 0.50;
	// std::cout<<min_area<<std::endl;

	for (int index = 0; index<data.v_blue_contours.size(); ++index)
	{

		auto contour = data.v_blue_contours[index];
		auto rotated_rect = minAreaRect(contour);

		double area = contourArea(contour, false);
		double perimeter = arcLength(contour, true);
		double circularity = 4 * PI*area / (perimeter*perimeter);

		if (area > min_area && rotated_rect.size.height > coef_y*min_area && rotated_rect.size.width > coef_x*min_area)
		{
			// std::cout<<"Circularity: "  << circularity << std::endl;

			std::vector<cv::Point> poly;
			cv::approxPolyDP(contour, poly, 0.02*perimeter, true);

			// std::cout<<"Lados: "<<poly.size()<<std::endl;

			if (circularity>min_circularity || poly.size() == 4)
			{
				if (circularity>min_circularity)
				{
					Sign found;
					found.color = Color::blue;
					found.shape = Shape::circular;

					found.roi = boundingRect(contour);

					data.v_signs.push_back(found);
				}
				else if (circularity>acceptable_circularity && poly.size() == 4)
				{
					Sign found;
					found.color = Color::blue;
					found.shape = Shape::rectangular;

					data.v_signs.push_back(found);
				}

				// for(int i=0; i<poly.size(); ++i)            // DEBUG
				// {
				//   line(Pic, poly[i], poly[i+1], GREEN, 1);
				//   if(i==poly.size()-1)
				//     line(Pic, poly[i], poly[0], GREEN, 1);
				// }
				// show_pic(Pic, "poly");
				// v_ellipse.push_back(fitEllipse(contour));   // DEBUG
				// v_rotated_rect.push_back(rotated_rect);
				//
				// drawContours(Pic, data.v_blue_contours, index, GREEN, 1);     // Debug
				// ellipse(Pic, v_ellipse.back(), YELLOW, 1);                    // Debug
				// Point2f rect_points[4]; rotated_rect.points(rect_points);     // Debug
				// draw_rectangle(Pic, rect_points, BLUE);                       // Debug
				//
				// show_pic(Pic, "contours");
				// waitKey();
			}
		}
	}
	// destroyAllWindows();
}

void Sign_Detection::print_info(Sign sign) {
	std::string forma;
	switch (as_integer(sign.shape))
	{
	case 1:
		forma = "Circular";
		break;
	case 2:
		forma = "Rectangular";
		break;
	case 3:
		forma = "Triangular";
		break;
	default:
		forma = "Unknown";
	}

	std::string color = as_integer(sign.color) == 1
		? "Red"
		: "Blue";

	std::cout << "Name:\t" << sign.name << std::endl;
	std::cout << "Shape:\t" << forma << std::endl;
	std::cout << "Color:\t" << color << std::endl;
	std::cout << std::endl << std::endl;
}
