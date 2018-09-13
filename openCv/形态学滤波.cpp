#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;

int main(){
	Mat image = imread("闸刀识别图片7李伟拍.jpg");
	namedWindow("[原始图]");
	namedWindow("[效果图]");
	imshow("[原始图]",image);

	cvtColor(image, image, COLOR_BGR2GRAY);
	////定义核
	Mat kernel = getStructuringElement(MORPH_RECT, Size(20,20));
	morphologyEx(image, image, MORPH_GRADIENT, kernel);
	imshow("[效果图]",image);
	waitKey(0);
	waitKey(0);
	return 0;
}
