#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int main(){
	
	Mat src, src_gray, dst, abs_dst;
	src = imread("闸刀识别图片7李伟拍.jpg");
	imshow("[原图]Laplacian2", src);
	//使用高斯滤波消除噪声
	GaussianBlur(src, src, Size(3,3), 0, 0 , BORDER_DEFAULT);

	cvtColor(src, src_gray, COLOR_RGB2GRAY);

	//使用Laplacian函数
	Laplacian(src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//计算绝对值，并将结果转化为8位
	convertScaleAbs(dst,abs_dst);

	imshow("[效果图] 整体方向Sobel 边缘检测2", abs_dst);

	waitKey(0);
	return 0;
}