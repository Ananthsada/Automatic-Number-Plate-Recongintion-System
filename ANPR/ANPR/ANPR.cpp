#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

#include "Filters.h"

#include <iostream>
#include <vector>

using namespace cv;

void doFilter(Mat& inputImage, Mat& outputImage)
{
	//outputImage = applySobel(inputImage);
	outputImage = applyCanny(inputImage);

	imshow("Filter applied image", outputImage);
}

void doContour(Mat& inputImage)
{
	std::vector<std::vector<Point>> contours;
	findContours(inputImage, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<Point>> contours_poly(contours.size());
	std::vector<Rect> boundRect(contours.size());
	//std::vector<Point2f>centers(contours.size());
	//std::vector<float>radius(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], contours_poly[i], 3, true);
		boundRect[i] = boundingRect(contours_poly[i]);
		//minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
	}
	Mat drawing = Mat::zeros(inputImage.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours_poly, (int)i, color);
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
		//circle(drawing, centers[i], (int)radius[i], color, 2);
	}
	imshow("Contours", drawing);
}

int main()
{
	std::string image_path = samples::findFile("C:\\Study\\ANPR\\TrainingImages\\input\\Audi-A4-528794d.png");
	std::cout << "Image path " << image_path << "\n";
	Mat img = imread(image_path, IMREAD_COLOR);

	if (img.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
		return 1;
	}

	//imshow("Color image", img);
	Mat _grayImage;
	cvtColor(img, _grayImage, COLOR_RGB2GRAY);
	imshow("Gray image", _grayImage);
	cv::Size sz = _grayImage.size();
	std::cout << "Width=" << sz.width << " height=" << sz.height;

	//Mat img_bw = _grayImage > 128;
	//imshow("B&W", img_bw);
	//Mat FilteredImage;
	//doFilter(_grayImage, FilteredImage);

	Mat _thresholdImage;
	//threshold(_grayImage, _thresholdImage, 190, 255, THRESH_BINARY + THRESH_OTSU);
	adaptiveThreshold(_grayImage, _thresholdImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 0);
	imshow("Threshold Image", _thresholdImage);
	
	//bitwise_not(_thresholdImage, _thresholdImage);
	//imshow("Reversed Image", _thresholdImage);

	Mat _element = getStructuringElement(MORPH_RECT, Size(2, 2));

	//Mat _erodedImage;
	//erode(_thresholdImage, _erodedImage, _element);
	//imshow("Eroded image", _erodedImage);
	//
	Mat _dilatedImage;
	dilate(_thresholdImage, _dilatedImage, _element);
	imshow("Dilated image", _dilatedImage);

	doContour(_thresholdImage);
	int k = waitKey(0);
//	if (k == 's')
//	{
//		imwrite("starry_night.png", img);
//	}

	return 0;
}
