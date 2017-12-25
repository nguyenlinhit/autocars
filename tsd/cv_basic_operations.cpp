#include "cv_basic_operations.hpp"
#include <string>

using namespace cv;

namespace cv_lib
{
	bool double_equals(double a, double b, double epsilon)
	{
		return std::abs(a - b) < epsilon;
	}

	int init_pic(const int& argc, char* argv[], std::string& strpicName, Mat& myPic) {
		if (argc != 2) {
			std::cout << "Missing picture name" << std::endl;
			return -1;
		}

		myPic = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);

		if (myPic.empty()) {
			std::cout << "Error opening picture file " << argv[1] << std::endl;
			return -1;
		}

		char chpicName[CHL];
		strcpy_s(chpicName, argv[1]);
		std::string picName(chpicName);
		strpicName = picName;

		size_t n = strpicName.find("Pictures/");
		strpicName.erase(strpicName.begin(), strpicName.begin() + n + 9);

		return 0;
	}
	int init_pic(const int& argc, char* argv[], std::vector<std::string>& strpicName, std::vector<Mat>& v_Pic) {   // TO-IMPROVE

		static int n_pic = 0;

		//v_Pic.resize(argc-1);

		for (int i = 1; i<argc; ++i)
		{
			++n_pic;

			if (argc < n_pic + 1) {
				std::cout << "Missing picture name" << std::endl;
				return -1;
			}

			Mat pic = imread(argv[n_pic], CV_LOAD_IMAGE_UNCHANGED);
			// myPic[i-1]= imread( argv[n_pic], CV_LOAD_IMAGE_GRAYSCALE);

			if (pic.empty()) {
				std::cout << "Error opening picture file " << argv[n_pic] << std::endl;
				return -1;
			}
			else
				v_Pic.push_back(pic);

			char chpicName[CHL];
			strcpy_s(chpicName, argv[n_pic]);
			std::string picName(chpicName);

			int n = picName.find("Pictures/");
			picName.erase(picName.begin(), picName.begin() + n + 9);
			strpicName.push_back(picName);


			//show_pic(pic, strpicName[i-1]);
		}

		return 0;
	}

	int load_from_folder(const std::string relative_path, std::vector<std::string>& v_picName, std::vector<Mat>& v_Pic)
	{
		int return_value = 0;

		std::vector<cv::String> v_paths;
		cv::String _pattern(relative_path);
		cv::glob(_pattern, v_paths);

		v_Pic.reserve(v_paths.size());
		v_picName.reserve(v_paths.size());

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
			v_Pic.push_back(myPic);

			char chpicName[CHL];
			strcpy_s(chpicName, v_paths[i].c_str());

			std::string picName(chpicName);

			//int n = picName.find("Pictures/");
			int n = 3;
			picName.erase(picName.begin(), picName.begin() + 8);
			v_picName.push_back(picName);
		}

		return return_value;
	}

	void print_info(const Mat& myPic) {
		print_info(myPic, "Unknown name");
	}
	void print_info(const std::vector<Mat>& v_Pic) {
		for (int i = 0; i<v_Pic.size(); ++i)
		{
			print_info(v_Pic.at(i), "Unknown name " + std::to_string(i));
		}
	}
	void print_info(const Mat& myPic, const std::string& picName) {

		std::cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << std::endl;
		// std::cout << "OpenCV: " << getBuildInformation();

		std::cout << std::endl;
		std::cout << "Name:\t\t" << picName << std::endl;
		std::cout << "Rows:\t\t" << myPic.rows << std::endl;
		std::cout << "Columns:\t" << myPic.cols << std::endl;
		std::cout << "Channels:\t" << myPic.channels() << std::endl;
		std::cout << "Type:\t\t";

		if (myPic.type() == CV_8UC1)  std::cout << "CV_8UC1" << std::endl;
		else if (myPic.type() == CV_8UC3)  std::cout << "CV_8UC3" << std::endl;
		else if (myPic.type() == CV_32FC1) std::cout << "CV_32FC1" << std::endl;
		else if (myPic.type() == CV_32FC3) std::cout << "CV_32FC3" << std::endl;
		else std::cout << "Unknown" << std::endl;
		std::cout << "Depth:\t\t" << myPic.depth();

		std::cout << std::endl;
	}
	void print_info(const std::vector<Mat>& v_Pic, const std::vector<std::string>& v_picName) {
		for (int iter = 0; iter<v_Pic.size(); ++iter)
		{
			print_info(v_Pic[iter], v_picName[iter]);
		}
	}

	void show_pic(const Mat& myPic) {
		std::string name(stringify(myPic));
		namedWindow(name, WINDOW_AUTOSIZE);
		imshow(name, myPic);
	}

	void show_pic(const std::vector<Mat>& v_Pic) {
		for (int i = 0; i<v_Pic.size(); ++i)
		{
			std::string name(stringify(v_Pic));
			show_pic(v_Pic.at(i), std::to_string(i + 1) + "_");
		}
	}

	void show_pic(const std::vector<Mat>& v_Pic, std::string name) {
		for (int i = 0; i<v_Pic.size(); ++i)
		{
			show_pic(v_Pic.at(i), name + std::to_string(i + 1) + "_");
		}
	}

	void show_pic(const Mat& myPic, const std::string name) {
		namedWindow(name, WINDOW_AUTOSIZE);
		imshow(name, myPic);
	}

	void show_pic(const std::vector<Mat>& v_Pic, const std::vector<std::string>& v_name) {
		static size_t n_pic = 1;
		for (int iter = 0; iter<v_Pic.size(); ++iter)
		{
			std::string window_name(std::to_string(n_pic) + "_" + v_name[iter]);
			namedWindow(window_name, WINDOW_AUTOSIZE);
			imshow(window_name, v_Pic[iter]);
			++n_pic;
		}
	}

	// void show_pic(const Mat& myPic, bool save)  {     // string in void show_pic(const Mat& myPic, std::string name) is interpreted as bool
	//     std::cout<<"ssff"<<std::endl;
	//   show_pic(myPic);
	//   if(save==true)
	//     save_pic(myPic);
	// }
	void show_pic(const Mat& myPic, const std::string name, const bool save) {
		namedWindow(name, WINDOW_AUTOSIZE);
		imshow(name, myPic);
		if (save == true)
			save_pic(myPic, name);
	}

	void save_pic(const Mat& Pic) {
		std::string newName(stringify(Pic));
		newName += ".jpg";  // Fundamental to add the extension
		imwrite(newName, Pic);
	}

	void save_pic(const Mat& Pic, const std::string str) {
		std::string newName(stringify(Pic));
		newName += "_" + str + ".jpg";  // Fundamental to add the extension
		imwrite(newName, Pic);
	}
}