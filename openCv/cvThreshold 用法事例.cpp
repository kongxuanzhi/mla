
#include "cv.h"
#include "highgui.h"

void sum_rgb(IplImage* src, IplImage* dst)  //该函数的作用就是转灰度后二值化
{
	IplImage* r = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* g = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* b = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	//函数 cvSplit 分割多通道数组成分离的单通道数组d
	cvSplit(src, r, g, b, NULL);
	IplImage* s = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	cvAddWeighted(r, 1. / 3, g, 1. / 3, 0.0, s);
	cvAddWeighted(s, 2. / 3, b, 1. / 3, 0.0, s);
	cvThreshold(s, dst, 100, 255, CV_THRESH_BINARY);

	cvReleaseImage(&r);
	cvReleaseImage(&g);
	cvReleaseImage(&b);
	cvReleaseImage(&s);


	s = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor(src, s, CV_BGR2GRAY);

	cvThreshold(s, dst, 100, 255, CV_THRESH_BINARY);
}
int main(int argc, char** argv)
{
	using namespace std;
	cvNamedWindow("aaa", 1);
	IplImage* src = cvLoadImage("Lena.jpg");
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, 1);

	sum_rgb(src, dst);
	cvShowImage("aaa", dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	return 0;
