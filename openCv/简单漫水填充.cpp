#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;

Mat g_srcImage, g_dstImage;

int main(){
	g_srcImage = imread("闸刀识别图片7李伟拍.jpg");
	namedWindow("[原始图]");
	imshow("[原始图]", g_srcImage);

	Rect ccomp;
	floodFill(g_srcImage, Point(10, 10), Scalar(255, 255, 255), &ccomp, Scalar(10, 10, 10), Scalar(10, 10, 10));

	namedWindow("[效果图]");
	imshow("[效果图]", g_srcImage);

	waitKey(0);
	waitKey(0);
	return 0;
}
