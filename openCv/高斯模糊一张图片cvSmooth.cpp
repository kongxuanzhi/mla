#include <iostream>
using namespace std;
#include <highgui.h>
#include <cv.h>

void example2_4(IplImage *image){
	cvNamedWindow("Example4-in");
	cvNamedWindow("Example4-out");

	cvShowImage("Example4-in",image);
	
	IplImage* out = cvCreateImage(
		cvGetSize(image),  //当前图像的大小
		IPL_DEPTH_8U,   //每个像素点的数据类型
		3   // 通道的个数
		);  //输出图像是3个通道，每个通道8位，图像大小同image

	cvSmooth(image, out, CV_GAUSSIAN, 3, 3);//高斯模糊  每个像素周围3*3区域进行高斯平滑处理
	
	cvShowImage("Example4-out", out);
	cvReleaseImage(&out);

	cvWaitKey(0);

	cvDestroyWindow("Example4-in");
	cvDestroyWindow("Example4-out");
}

int main(){
	IplImage *img = cvLoadImage("th.jpg");
	example2_4(img);
}
