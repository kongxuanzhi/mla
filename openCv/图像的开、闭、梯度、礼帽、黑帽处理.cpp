#include "cv.h"  
#include "highgui.h"  

int main(int argc, char ** argv)
{
	cvNamedWindow("sourceImage");
	cvNamedWindow("open");
	cvNamedWindow("close");
	cvNamedWindow("gradient");
	cvNamedWindow("topHat");
	cvNamedWindow("blackHat");
	IplImage * src = cvLoadImage("cut.jpg",1);
	cvShowImage("sourceImage", src);
	IplImage * temp = cvCreateImage(cvGetSize(src), 8, 3);
	IplImage * img = cvCreateImage(cvGetSize(src), 8, 3);
	cvCopy(src, temp);
	cvCopy(src, img);
	//开运算  
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL, //default 3*3  
		CV_MOP_OPEN,
		4);
	cvShowImage("open", img);
	cvWaitKey(0);
	//闭运算  
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL, //default 3*3  
		CV_MOP_CLOSE,
		4);
	cvShowImage("close", img);
	//形态梯度  
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL, //default 3*3  
		CV_MOP_GRADIENT,
		3);
	cvShowImage("gradient", img);
	//cvWaitKey(0);  

	//"礼帽"  
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL, //default 3*3  
		CV_MOP_TOPHAT,
		3);
	cvShowImage("topHat", img);
	//cvWaitKey(0);  
	//“黑帽”  
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL, //default 3*3  
		CV_MOP_BLACKHAT,
		3);
	cvShowImage("blackHat", img);
	cvWaitKey(0);
	cvReleaseImage(&temp);
	cvReleaseImage(&src);
	cvReleaseImage(&img);
	cvDestroyAllWindows();
	return 0;
}
