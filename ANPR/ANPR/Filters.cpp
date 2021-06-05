#include "Filters.h"

using namespace cv;
cv::Mat applySobel(const cv::Mat& input)
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	int ddepth = CV_16S;
	int ksize = 3;
	int scale = 2;
	int delta = 1;

	Sobel(input, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
	Sobel(input, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	Mat grad;
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	return grad;
}


cv::Mat applyCanny(const cv::Mat& input)
{
	int lowThreshold = 0;
	const int max_lowThreshold = 100;
	const int ratio = 3;
	const int kernel_size = 3;
	Mat dst, detected_edges;

	blur(input, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	return detected_edges;
}
