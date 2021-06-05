#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

#include "Filters.h"

#include <iostream>

using namespace cv;

int main()
{
	std::string image_path = samples::findFile("C:\\Study\\ANPR\\TrainingImages\\archive\\in\\in\\Audi-A4-528794d.jpg_0359_0305_0250_0204_0066.png");
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
	//imshow("Gray image", _grayImage);

	Mat _sobelApplied = applySobel(_grayImage);
	imshow("Sobel applied image", _sobelApplied);

	Mat _cannyApplied = applyCanny(_grayImage);
	imshow("Canny applied image", _cannyApplied);

	int k = waitKey(0);

//	if (k == 's')
//	{
//		imwrite("starry_night.png", img);
//	}

	return 0;
}
