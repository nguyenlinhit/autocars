
#include "cv_library.hpp"

using namespace cv;

namespace cv_lib
{
	void grey_pic(const Mat& Pic_original, Mat& Pic_grey) {
		if ((Pic_original.channels())>1) {
			cvtColor(Pic_original, Pic_grey, CV_BGR2GRAY);
		}
		else
			Pic_grey = Pic_original.clone();
	}

	void grey_pic(const std::vector<Mat>& v_Pic, std::vector<Mat>& v_Pic_grey) {
		v_Pic_grey.resize(v_Pic.size());

		for (int i = 0; i<v_Pic.size(); ++i)
		{
			grey_pic(v_Pic[i], v_Pic_grey[i]);
		}
	}

	void color_pic(const Mat& Pic_original, Mat& Pic_color) {
		if (Pic_original.channels() != 3) {
			cvtColor(Pic_original, Pic_color, CV_GRAY2BGR);
		}
		else
			Pic_color = Pic_original.clone();
	}

	void color_pic(const std::vector<Mat>& v_Pic, std::vector<Mat>& v_Pic_color) {
		v_Pic_color.resize(v_Pic.size());

		for (int i = 0; i<v_Pic.size(); ++i)
		{
			color_pic(v_Pic[i], v_Pic_color[i]);
		}
	}

	void th_pic(const Mat& Pic_original, Mat& Pic_th, const int _th, const int type) {  // def. type: OTSU, def. th: 125
		threshold(Pic_original, Pic_th, _th, 255, type);
	}

	void th_pic(const std::vector<Mat>& v_Pic_original, std::vector<Mat>& v_Pic_th, const int _th, const int type) {  // def. type: OTSU, def. _th: 125
		v_Pic_th.resize(v_Pic_original.size());

		for (int i = 0; i<v_Pic_original.size(); ++i)
		{
			th_pic(v_Pic_original[i], v_Pic_th[i], _th, type);
		}
	}

	void HSV_pic(const Mat& Pic_BGR, Mat& Pic_HSV) {
		if ((Pic_BGR.channels()) == 3) {
			cvtColor(Pic_BGR, Pic_HSV, CV_BGR2HSV);
		}
		else
			assert(0 && "Pic channels != 3");
	}

	void HSV_pic(const std::vector<Mat>& v_Pic_BGR, std::vector<Mat>& v_Pic_HSV) {
		v_Pic_HSV.resize(v_Pic_BGR.size());

		for (int i = 0; i<v_Pic_BGR.size(); ++i)
		{
			HSV_pic(v_Pic_BGR[i], v_Pic_HSV[i]);
		}
	}

	void manual_binarize(const Mat& myPic, Mat& binPic, const int threshold) {
		binPic = myPic.clone();

		for (int i = 0; i<binPic.rows; i++)
		{
			uchar* ptrRow = binPic.ptr<uchar>(i);
			for (int j = 0; j<binPic.cols; j++)
			{
				double grey_level = *(ptrRow + j);
				if (grey_level < threshold)
					*(ptrRow + j) = 0;
				else
					*(ptrRow + j) = 255;
				// std::cout<<grey_level<<" ";
			}
			// std::cout<<std::endl;
		}
	}

	void rescale_pic(const Mat& Pic_original, Mat& Pic_rescaled, const float coef_x, const float coef_y) {
		resize(Pic_original, Pic_rescaled, Size(), coef_x, coef_y, CV_INTER_LANCZOS4);
	}

	void rescale_pic(const Mat& Pic_original, Mat& Pic_rescaled, const float coef) {
		resize(Pic_original, Pic_rescaled, Size(), coef, coef, CV_INTER_LANCZOS4);
	}

	void fix_size(const Mat& Pic_original, Mat& Pic_rescaled, const int max_height, const int max_width) {     // def max_height= MAX_HEIGHT, def max_width= MAX_WIDTH
		Pic_rescaled = Pic_original.clone();
		while ((Pic_rescaled.rows > max_height) || (Pic_rescaled.cols > max_width))
		{
			rescale_pic(Pic_rescaled, Pic_rescaled, 0.9);
		}
	}

	void fix_size(Mat& Pic, const int max_height, const int max_width) {   // def max_height= MAX_HEIGHT, def max_width= MAX_WIDTH
		Mat Pic_aux = Pic.clone();
		fix_size(Pic, Pic_aux, max_height, max_width);
	}

	void fix_size(std::vector<Mat>& v_Pic, const int max_height, const int max_width) {  // def max_height= MAX_HEIGHT, def max_width= MAX_WIDTH
		for (auto& Pic : v_Pic)
		{
			Mat Pic_aux = Pic.clone();
			fix_size(Pic, Pic_aux, max_height, max_width);
		}
	}

	void fix_size(std::vector<Mat>& v_Pic_src, std::vector<Mat>& v_Pic_dst, const int max_height, const int max_width) {  // def max_height= MAX_HEIGHT, def max_width= MAX_WIDTH
		v_Pic_dst.resize(v_Pic_src.size());
		for (int i = 0; i<v_Pic_src.size(); ++i)
		{
			fix_size(v_Pic_src[i], v_Pic_dst[i], max_height, max_width);
		}
	}

	void rotate_pic(const Mat& Pic_src, Mat& Pic_rotated, const Point2f center, const double angle) {
		Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);
		warpAffine(Pic_src, Pic_rotated, rot_mat, Pic_src.size());
	}

	void rotate_pic(Mat& Pic, const Point2f center, const double angle) {
		Mat aux_pic = Pic.clone();
		rotate_pic(Pic, aux_pic, center, angle);
	}

	void draw_rectangle(Mat& Pic, const Point2f(&vertices)[4], const Scalar color, const int thickness, const cv::LineTypes linetype) {    // def. color: BLACK, def. thickenss: 1, def. linetype= LINE_8
		for (int vert_1 = 0; vert_1<4; ++vert_1)
		{
			int vert_2 = (vert_1 + 1) % 4;
			line(Pic, vertices[vert_1], vertices[vert_2], color, thickness, linetype);
		}
	}

	std::unique_ptr<Rect> light_rectangle(const Mat& Pic_original, const int threshold) {
		Mat Pic_grey = Pic_original.clone();
		if ((Pic_original.channels())>1) {
			cvtColor(Pic_original, Pic_grey, CV_BGR2GRAY);
		}

		int righter = 0, lower = 0, lefter = 999, higher = 999, grey_level;

		for (int i = 0; i<Pic_grey.rows; ++i)      // from low to high
		{
			uchar *p_row = Pic_grey.ptr<uchar>(i);
			for (int j = 0; j<Pic_grey.cols; ++j)  // from left to right
			{
				grey_level = p_row[j];
				// std::cout<<i<<" "<<j<<" "<<grey_level<<std::endl;
				if (grey_level>threshold) {
					if (j>righter)   righter = j;
					if (i>lower)     lower = i;

					if (j<lefter)    lefter = j;
					if (i<higher)    higher = i;
				}
			}
		}

		// std::cout<<righter<<" "<<lower<<std::endl;
		// std::cout<<lefter<<" "<<higher<<std::endl;

		return std::unique_ptr<Rect>(new Rect(lefter, higher, +abs(lefter - righter), +abs(higher - lower))); // esquina_inferior.x, esquina_inferior.y, wide, height
	}

	void light_rectangle(const Mat& Pic_original, const int threshold, int& xleft, int& xright, int& yhigh, int& ylow) {
		Mat Pic_grey = Pic_original.clone();
		if ((Pic_original.channels())>1) {
			cvtColor(Pic_original, Pic_grey, CV_BGR2GRAY);
		}
		int righter = 0, lower = 0, lefter = Pic_grey.cols, higher = Pic_grey.rows, grey_level;

		for (int i = 0; i<Pic_grey.rows; ++i)      // from low to high
		{
			uchar *p_row = Pic_grey.ptr<uchar>(i);
			for (int j = 0; j<Pic_grey.cols; ++j)  // from left to right
			{
				grey_level = p_row[j];
				// std::cout<<i<<" "<<j<<" "<<grey_level<<std::endl;
				if (grey_level>threshold) {
					if (j>righter)   righter = j;
					if (i>lower)     lower = i;

					if (j<lefter)    lefter = j;
					if (i<higher)    higher = i;
				}
			}
		}

		// std::cout<<righter<<" "<<lower<<std::endl;
		// std::cout<<lefter<<" "<<higher<<std::endl;
		xleft = lefter;
		xright = righter;
		yhigh = lower;
		ylow = higher;
	}

	void fill_no_rectangle(const Mat& Pic_original, Mat& Pic_clean, const Rect& _rectangle, const Scalar& color) {   // def. color: BLACK
		Pic_clean = Pic_original.clone();

		for (int i = 0; i<Pic_clean.rows; ++i)      // from low to high
		{
			for (int j = 0; j<Pic_clean.cols; ++j)  // from left to right
			{
				Point2i Pixel(j, i);
				if (!Pixel.inside(_rectangle))
				{
					Point3_<uchar>* p = Pic_clean.ptr<Point3_<uchar> >(i, j);
					p->x = color[0];
					p->y = color[1];
					p->z = color[2];
				}
			}
		}
	}

	void fill_no_rectangle(Mat& Pic, const Rect& _rectangle, const Scalar& color) {   // def. color: BLACK
		Mat Pic_aux = Pic.clone();
		fill_no_rectangle(Pic, Pic_aux, _rectangle, color);
	}

	void fill_no_rectangles(const Mat& Pic_original, Mat& Pic_clean, const std::vector<Rect>& v_rectangle, const Scalar& color) {   // def. color: BLACK
		Mat total_pic(Pic_original.rows, Pic_original.cols, Pic_original.type(), WHITE);

		for (const auto& _rectangle : v_rectangle)
		{
			Mat _rectangle_content;

			fill_no_rectangle(Pic_original, _rectangle_content, _rectangle, color);

			Mat m_rectangle = _rectangle_content(_rectangle).clone();

			m_rectangle.copyTo(total_pic(cv::Rect(_rectangle.x, _rectangle.y, m_rectangle.cols, m_rectangle.rows)));
		}

		Pic_clean = total_pic.clone();
	}

	void fill_no_rectangles(Mat& Pic, const std::vector<Rect>& v_rectangle, const Scalar& color) {     // def. color: BLACK
		fill_no_rectangles(Pic, Pic, v_rectangle, color);
	}

	void get_roi_from_boundingRect(const Mat& Pic, const std::vector<Rect>& v_bounding_rect, Rect& roi, int& lefter, int& righter, int& lower, int& higher) {
		lefter = Pic.rows - 1;
		lower = Pic.cols - 1;
		righter = 0;
		higher = 0;

		for (int i = 0; i<Pic.rows; ++i)      // from low to high
		{
			for (int j = 0; j<Pic.cols; ++j)  // from left to right
			{
				Point2i Pixel(j, i);
				for (const auto& _rectangle : v_bounding_rect)
				{
					if (Pixel.inside(_rectangle))
					{
						// assert(i==Pixel.y);
						// assert(j==Pixel.x);

						if (j>righter)   righter = j;
						if (i>higher)    higher = i;

						if (j<lefter)    lefter = j;
						if (i<lower)     lower = i;
					}
				}
			}
		}
		roi = Rect(lefter - 1, lower - 1, 3 + abs(lefter - righter), 3 + abs(lower - higher));
	}

	void get_roi_from_boundingRect(const Mat& Pic, const std::vector<Rect>& v_bounding_rect, Rect& roi) {
		int lefter, righter, lower, higher;
		get_roi_from_boundingRect(Pic, v_bounding_rect, roi, lefter, righter, lower, higher);
	}

	void set_Brightness_Contrast(const Mat& Pic_original, const int& brightness, const int& contrast, Mat& Pic_final) {
		double alpha, beta, delta;
		if (contrast > 0)
		{
			delta = 127. * contrast / 100;
			alpha = 255. / (255. - delta * 2);
			beta = alpha*(brightness - delta);
		}
		else
		{
			delta = -128. * contrast / 100;
			alpha = (256. - delta * 2) / 255.;
			beta = alpha*brightness + delta;
		}

		Pic_original.convertTo(Pic_final, CV_8U, alpha, beta);    // imgGrisResult(x,y) = saturate_cast<uchar> (alpha*imgGris(x,y) + beta);
	}

	void create_contours(const Mat& Pic_src, Mat& Pic_dst, std::vector<std::vector<Point>>& contours) {
		Pic_dst = Pic_src.clone();

		findContours(
			Pic_src,
			contours,
			// std::vector<Vec4i> hierarchy; // Optional in OpenCV 3.2
			CV_RETR_EXTERNAL,    // TO-CHECK
								 // CV_RETR_LIST,
			CV_CHAIN_APPROX_NONE
			// 	Point  	offset = Point()
		);
		drawContours(Pic_dst, contours, 0, Scalar(255), CV_FILLED);

		// std::cout << "Located regions: " <<  contours.size() << std::endl;
		// show_pic(Pic_dst);
	}

	void create_contours(Mat& Pic, std::vector<std::vector<Point>>& contours) {
		Mat Pic_aux = Pic.clone();
		create_contours(Pic, Pic_aux, contours);
	}

	int count_objects(const Mat& Pic, Mat& Pic_dst) {
		Mat aux;
		std::vector<std::vector<Point>> v_contours;
		create_contours(Pic, aux, v_contours);

		for (int j = 0; j<v_contours.size(); ++j)
		{
			if (contourArea(v_contours[j], false) > 100)
				drawContours(Pic_dst, v_contours, j, RED, 1);

			Rect bounding_rect = boundingRect(v_contours[j]);
			rectangle(Pic_dst, bounding_rect, BLUE, 1);
		}

		return v_contours.size();
	}

	void morpho_pic(const Mat& src, Mat& dst, int iter, cv::MorphTypes type, Mat kernel, Point anchor) { // def. kernel, def. anchor
		morphologyEx(src, dst, type, kernel, anchor, iter);
	}

	void morpho_pic(const std::vector<Mat>& v_src, std::vector<Mat>& v_dst, int iter, cv::MorphTypes type, Mat kernel, Point anchor) {
		v_dst.resize(v_src.size());
		for (int i = 0; i<v_src.size(); ++i)
		{
			morpho_pic(v_src[i], v_dst[i], iter, type, kernel, anchor);
		}
	}

	void morpho_pic(const Mat& src, Mat& dst, int iter, cv::MorphTypes type, Point anchor, Mat kernel) { // def. anchor
		morphologyEx(src, dst, type, kernel, anchor, iter);
	}

	void morpho_pic(const std::vector<Mat>& v_src, std::vector<Mat>& v_dst, int iter, cv::MorphTypes type, Point anchor, Mat kernel) {
		v_dst.resize(v_src.size());
		for (int i = 0; i<v_src.size(); ++i)
		{
			morpho_pic(v_src[i], v_dst[i], iter, type, kernel, anchor);
		}
	}

	void create_histo(const Mat& Pic, Mat& hist, const Scalar color) { // General function
		if ((Pic.channels())>1 && color == BLACK) {
			std::cout << "Something's probably wrong" << std::endl;
		}

		int histSize[1];
		float hranges[2];
		const float* ranges[1];
		int channels[1];

		histSize[0] = 256;         // size
		hranges[0] = 0.0;          // min_value
		hranges[1] = 256.0;        // max_value
		ranges[0] = hranges;       // ranges
		channels[0] = 0;           // channel 0

		cv::calcHist(&Pic,
			1,		        // number of pics
			channels,	  // channels used
			cv::Mat(),   // no mask
			hist,		    // histograma resultante
			1,			      // 1D
			histSize,	  // number of elements in hist
			ranges		    // pixel range value
		);

		if (color == BLACK)
		{
			Mat Pic_histo(HISTO_HEIGHT, HISTO_WIDTH, CV_8UC3, WHITE);
			draw_histo(Pic_histo, hist, color);

			show_histo(Pic_histo);
		}
	}

	void create_histo(const Mat& Pic, Mat& hist) {   // 1 pic, 1 hist -> We can assume user wants a B&W histogram
		Mat Pic_grey = Pic.clone();

		if ((Pic_grey.channels())>1) {
			cvtColor(Pic, Pic_grey, CV_BGR2GRAY);
		}

		create_histo(Pic_grey, hist, BLACK);
	}

	void create_histo(const Mat& Pic, std::vector<Mat>& v_hist) {    // 1 pic, vector of hist -> We assume yser wabts a RGB histogram
		Mat Pic_color = Pic.clone();
		if ((Pic.channels()) != 3) {
			std::cout << "\n***BEWARE: not a color pic!***\n" << std::endl;
			cvtColor(Pic, Pic_color, COLOR_GRAY2RGB);     // Transform the B&W pic in RGB
		}

		Mat Pic_histo(HISTO_HEIGHT, HISTO_WIDTH, CV_8UC3, WHITE);
		std::vector<Mat> v_BGR;
		v_BGR.resize(3);
		v_hist.resize(3);
		split(Pic_color, v_BGR);

		create_histo(v_BGR[0], v_hist[0], BLUE);
		create_histo(v_BGR[1], v_hist[1], GREEN);
		create_histo(v_BGR[2], v_hist[2], RED);

		draw_histo(Pic_histo, v_hist[0], BLUE);
		draw_histo(Pic_histo, v_hist[1], GREEN);
		draw_histo(Pic_histo, v_hist[2], RED);

		show_histo(Pic_histo);
	}

	void create_histo(const std::vector<Mat>& v_Pic, std::vector<Mat>& v_hist) { // Does NOT return all hist
		bool BW = true;
		for (const auto& Pic : v_Pic) {
			if ((Pic.channels()) == 3)      // If one of them is color Pic, creates RGB histograms
			{
				BW = false;
				break;
			}
		}
		if (BW == true)
		{
			v_hist.resize(v_Pic.size());
			for (int iter = 0; iter<v_Pic.size(); ++iter)
			{
				create_histo(v_Pic[iter], v_hist[iter]);
			}
		}
		else    // Only case when not all hists are going to be returned - TO-CHECK
		{
			for (const auto& Pic : v_Pic)
			{
				create_histo(Pic, v_hist);
			}
		}
	}

	void create_histo(const Mat& Pic)
	{
		Mat hist;

		create_histo(Pic, hist);
	}

	void draw_histo(Mat& Pic_histo, const Mat& histo, const Scalar color) {
		int ratio = cvRound((double)HISTO_WIDTH / HISTO_SIZE);

		normalize(histo, histo, 0, HISTO_HEIGHT, NORM_MINMAX);  // histo -> [0, HISTO_HEIGHT]

		for (int i = 1; i < HISTO_SIZE; i++)   // Lines which join the ends of the bars
		{
			line(Pic_histo,
				Point((i - 1) * ratio, HISTO_HEIGHT - cvRound(histo.at<float>(i - 1))),
				Point(i * ratio, HISTO_HEIGHT - cvRound(histo.at<float>(i))),
				color, 2, 8, 0);
		}
	}

	void show_histo(Mat& Pic_histo) {
		static int n_pic;

		Rect rect(0, 0, HISTO_WIDTH - 1, HISTO_HEIGHT - 1);
		rectangle(Pic_histo, rect, Scalar(100, 100, 100));

		show_pic(Pic_histo, stringify(Pic_histo) + std::to_string(n_pic));
		n_pic++;  // To show one per picture
	}


	void rotation_trackbar(int, void* userdata) {
		UserData user_data = *((UserData*)userdata); // cast + de-reference
		user_data.angle -= 180;
		Point centro = Point(user_data.pt.x, user_data.pt.y);
		Mat matrizRot = getRotationMatrix2D(centro, (user_data.angle), 1);
		warpAffine(user_data.Pic_src, user_data.Pic_dst, matrizRot, user_data.Pic_src.size());
		imshow("Imagen Rotada", user_data.Pic_dst);
	}

	void brightness_contrast_trackbar(int, void* userdata) {
		UserData user_data = *((UserData*)userdata);   // cast + de-reference

		user_data.brightness -= 100;    // [-100, +100]
		user_data.contrast -= 100;      //  [-100, +100]

		set_Brightness_Contrast(user_data.Pic_src, user_data.brightness, user_data.contrast, user_data.Pic_dst);

		create_histo(user_data.Pic_dst, user_data.hist, PSEUDO_BLACK);  // BLACK would make it call to draw_histo & show_pic every time
		Mat Pic_histo(HISTO_HEIGHT, HISTO_WIDTH, CV_8UC3, WHITE);
		draw_histo(Pic_histo, user_data.hist, RED);

		show_pic(Pic_histo, "Histogram");
		show_pic(user_data.Pic_dst, "Pic");
	}

	void thresh_trackbar(int, void* userdata) {
		UserData user_data = *((UserData*)userdata); // cast + de-reference

		static cv::RNG rng(3.14159264);   // OpenCV own random number generator - fixed seed

		Canny(user_data.Pic_src, user_data.Pic_dst, user_data.thresh, user_data.thresh * 2, 3);    // Detect edges using canny

		show_pic(user_data.Pic_dst, "Canny");

		findContours(
			user_data.Pic_dst,
			user_data.contours,
			// std::vector<Vec4i> hierarchy; // Optional in OpenCV 3.2
			CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0) // Find contours
												// 	Point  	offset = Point()
		);

		Mat drawing = Mat::zeros(user_data.Pic_dst.size(), CV_8UC3); // Draw contours
		for (int i = 0; i< user_data.contours.size(); ++i)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, user_data.contours, i, color, 2);
		}

		show_pic(drawing, "Colored contours");
	}

	bool template_matching(const cv::Mat& pic, const cv::Rect& roi, const cv::Mat& templ, const double coef_correlation, const int match_method, const double reduction_coef, const int size_difference)
	{
		Mat aux_pic = pic.clone();
		rectangle(aux_pic, roi, WHITE, 1);
		fill_no_rectangle(pic, aux_pic, roi, WHITE);

		Mat comparer = templ.clone();
		do {
			rescale_pic(comparer, comparer, reduction_coef);
		} while (aux_pic.size().height < comparer.size().height + 5 || aux_pic.size().width < comparer.size().width + 5);

		double found_correlation = 0;

		grey_pic(comparer, comparer);
		th_pic(comparer, comparer, 100, CV_THRESH_BINARY);

		while (true)
		{
			Mat original = aux_pic.clone();

			if (found_correlation >coef_correlation)
			{
				// std::cout<<"Match!"<<std::endl;
				return true;
			}
			else if (size_difference*comparer.rows < original.rows || size_difference*comparer.cols < original.rows)
				return false;

			// show_pic(original, "or");

			Mat result;
			int result_cols = original.rows - comparer.rows + 1;
			int result_rows = original.rows - comparer.rows + 1;
			result.create(result_rows, result_cols, CV_32FC1);

			matchTemplate(original, comparer, result, match_method);

			/// Localizing the best match with minMaxLoc
			double minVal, maxVal;
			Point minLoc, maxLoc, matchLoc;

			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

			/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
			if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
				matchLoc = minLoc;
			else
				matchLoc = maxLoc;

			rectangle(original, matchLoc, Point(matchLoc.x + comparer.cols, matchLoc.y + comparer.rows), Scalar::all(125), 2, 8, 0);
			rectangle(result, matchLoc, Point(matchLoc.x + comparer.cols, matchLoc.y + comparer.rows), Scalar::all(125), 2, 8, 0);


			Rect r_isolated(matchLoc, Point(matchLoc.x + comparer.cols, matchLoc.y + comparer.rows));

			Mat pic_isolated = aux_pic(r_isolated).clone();

			found_correlation = cv_lib::match_percentage(pic_isolated, comparer);

			cv_lib::rescale_pic(comparer, comparer, reduction_coef);

			// std::cout<<std::fixed<<std::setprecision(2)<<found_correlation<<"%"<<std::endl;      // FOR DEBUGGING
			// show_pic(original, "original");
			// show_pic(comparer, "comparer");
			// waitKey(0);
		}
	}

	double match_percentage(const Mat& Pic1, const Mat& Pic2)
	{
		// show_pic(Pic2, "cmp2");
		Mat result;
		cv::absdiff(Pic1, Pic2, result);
		grey_pic(result, result);
		manual_binarize(result, result, 100);

		unsigned black_level, n_black_pixels = 0;
		for (unsigned i = 0; i<result.rows; ++i)      // from low to high
		{
			uchar *p_row = result.ptr<uchar>(i);
			for (unsigned j = 0; j<result.cols; ++j)  // from left to right
			{
				black_level = p_row[j];
				// std::cout<<i<<" "<<j<<" "<<grey_level<<std::endl;
				if (black_level == 0) {
					n_black_pixels++;
				}
			}
		}

		double match_percentage = static_cast<double> (n_black_pixels) / (static_cast<double>(result.rows*result.cols));

		// show_pic(result, "Comparing");     // FOR DEBUGGING

		return match_percentage;
	}

}
