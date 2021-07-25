#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

#include "Filters.h"

#include <iostream>
#include <vector>

using namespace cv;

const std::string ImagePath = "C:\\Study\\ANPR\\TrainingImages\\input\\";
const std::string ImageName = ImagePath + "Audi-A4-528794d.png";

void doFilter(Mat& inputImage, Mat& outputImage)
{
	//outputImage = applySobel(inputImage);
	outputImage = applyCanny(inputImage);

	imshow("Filter applied image", outputImage);
}

void doConnectedComponents(Mat& inputImage)
{
	Mat _labelImage;
	Mat _statImage, _centroidImage;
	int _nlabels = connectedComponentsWithStats(inputImage, _labelImage, _statImage, _centroidImage);

	for (int i = 0; i < _statImage.rows; i++)
	{
		int x = _statImage.at<int>(Point(0, i));
		int y = _statImage.at<int>(Point(1, i));
		int w = _statImage.at<int>(Point(2, i));
		int h = _statImage.at<int>(Point(3, i));

		std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;

		Scalar color(255, 0, 0);
		Rect rect(x, y, w, h);
		rectangle(inputImage, rect, color);

		size_t size = aspose::ocr::page_rect(image_path.c_str(), buffer, len, x, y, w, h);
	}
	imshow("Rectangles", inputImage);
}

void doContour(Mat& inputImage)
{
	std::vector<std::vector<Point>> contours;
	findContours(inputImage, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<Point>> contours_poly(contours.size());
	std::vector<Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], contours_poly[i], 3, true);
		boundRect[i] = boundingRect(contours_poly[i]);
	}
	Mat drawing = Mat::zeros(inputImage.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours_poly, (int)i, color);
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
	}
	imshow("Contours", drawing);
}

int main()
{
	std::string image_path = samples::findFile(ImageName);
	std::cout << "Image path " << image_path << "\n";
	Mat img = imread(image_path, IMREAD_COLOR);

	if (img.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
		return 1;
	}

	Mat _grayImage;
	cvtColor(img, _grayImage, COLOR_RGB2GRAY);
	imshow("Gray image", _grayImage);
	cv::Size sz = _grayImage.size();
	std::cout << "Width=" << sz.width << " height=" << sz.height;

	Mat _resizedImage;
	resize(_grayImage, _resizedImage, Size(200, 60));

	Mat _thresholdImage;
	adaptiveThreshold(_resizedImage, _thresholdImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 0);
	imshow("Threshold Image", _thresholdImage);

	Mat _connectedImage(_thresholdImage.size(), CV_8UC3);
	doConnectedComponents(_thresholdImage);

	int k = waitKey(0);

	return 0;
}
