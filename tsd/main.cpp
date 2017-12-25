#include <opencv2/opencv.hpp>

#include "cv_basic_operations.hpp"
#include "cv_library.hpp"
#include "Sign_Detection.hpp"
#include <vector>
#include <string>
#include <cassert>

using namespace cv;
using namespace cv_lib;

const std::string samples_relative_path("samples/");
const std::string templates_relative_path("templates/");

int main(int argc, char* argv[])
{
	// Sign_Detection instance(argc, argv);                      // Deprecated: loads samples from Filedata.txt
	Sign_Detection instance(samples_relative_path, templates_relative_path);

	// int loadStatus=instance.load_templates(argc, argv);
	int loadStatus = instance.load_templates();

	assert(!instance.get_v_templates_red().empty());
	assert(!instance.get_v_templates_blue().empty());
	instance.normalize_templates();

	if (instance.get_loadStatus() < 0 || loadStatus<0)
		return EXIT_FAILURE;

	for (auto& data : instance.get_v_data())
	{
		show_pic(data.Pic_color, "Image");

		data.Pic_color.convertTo(data.Pic_color_high_contrast, -1, 2, 0);     // 2x contrast
		HSV_pic(data.Pic_color_high_contrast, data.Pic_HSV);                  // Re-write
		show_pic(data.Pic_HSV, "Image_HSV");																	  // show_pic(data.Pic_color_high_contrast, "High contrast");
																			  show_pic(data.Pic_th, "th");

																			  // Red:
		instance.extract_red_HSV(data);
		 show_pic(data.Pic_HSV_red, "Red");

		create_contours(data.Pic_HSV_red, data.v_red_contours);
		instance.filter_red_contours(data);


		// Blue:
		instance.extract_blue_HSV(data);
		 show_pic(data.Pic_HSV_blue, "Blue");

		create_contours(data.Pic_HSV_blue, data.v_blue_contours);
		instance.filter_blue_contours(data);

		waitKey();

		//show_pic(data.Pic_color, "Color");
		for (auto& possible_sign : data.v_signs)
		{
			if (possible_sign.color == Color::red)
			{
				if (possible_sign.shape == Shape::circular)
				{
					unsigned index = 0;
					for (const auto& circular_template : instance.get_v_templates_red())
					{
						// std::cout<<"Template # "<<index+1<<"/"<<instance.v_templates_red.size()<<std::endl;
						Mat result;
						if (cv_lib::template_matching(data.Pic_HSV_red, possible_sign.roi, circular_template, 0.77) == true)
						{
							int pos = instance.get_v_picNames_red()[index].find("_template");
							std::string name(instance.get_v_picNames_red()[index], 0, pos);
							possible_sign.name = name;
							// std::cout<<"This is a "<<name<<std::endl<<std::endl<<std::endl;
							// instance.print_info(possible_sign);
							break;
						}
						else
						{
							int pos = instance.get_v_picNames_red()[index].find("_template");
							std::string name(instance.get_v_picNames_red()[index], 0, pos);
							possible_sign.name = name;
							// std::cout<<"This is NOT a "<<name<<std::endl;
						}

						++index;
						// waitKey();
					}
				}
				else if (possible_sign.shape == Shape::triangular)
				{
					// TO-DO
				}
				else
				{

				}
			}
			else if (possible_sign.color == Color::blue)
			{
				if (possible_sign.shape == Shape::circular)
				{
					unsigned index = 0;
					for (const auto& circular_template : instance.get_v_templates_blue())
					{
						// std::cout<<"Template # "<<index+1<<"/"<<instance.get_v_templates_blue().size()<<std::endl;
						Mat result;
						// waitKey();
						if (cv_lib::template_matching(data.Pic_HSV_blue, possible_sign.roi, circular_template, 0.80) == true)
						{
							int pos = instance.get_v_picNames_blue()[index].find("_template");
							std::string name(instance.get_v_picNames_blue()[index], 0, pos);
							possible_sign.name = name;
							// std::cout<<"This is a "<<name<<std::endl<<std::endl<<std::endl;
							// instance.print_info(possible_sign);
							break;
						}
						else
						{
							int pos = instance.get_v_picNames_blue()[index].find("_template");
							std::string name(instance.get_v_picNames_blue()[index], 0, pos);
							possible_sign.name = name;
							// std::cout<<"This is NOT a "<<name<<std::endl;
						}

						++index;
						// waitKey();
					}

				}
				else if (possible_sign.shape == Shape::rectangular)
				{
					// TO-DO
				}
				else
				{

				}
			}
			else
			{
				std::cout << "Unclasified colorr" << std::endl;
			}

			show_pic(data.Pic_color(possible_sign.roi), "Traffic sign");
			instance.print_info(possible_sign);

			waitKey();

			destroyWindow("Traffic sign");
		}
		// destroyAllWindows();
	}

	waitKey();

	return EXIT_SUCCESS;
}