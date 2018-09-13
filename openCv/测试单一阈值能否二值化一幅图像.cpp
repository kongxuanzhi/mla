#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff)  

IplImage *img_edge1;
IplImage *img_edge;
int thredholdv = 0;

void change(int pos){
	cvThreshold(img_edge1, img_edge, thredholdv, 255, CV_THRESH_BINARY);   //单一的阈值化，得不到效果
	cvShowImage("origin", img_edge);
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	int Nc = cvFindContours(
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_CCOMP
		);
	int n = 0;
	cvNamedWindow("图片1", 1);
	printf("Total Contours Detected : %d\n", Nc);
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		cvDrawContours(img_edge, c,
			CVX_RED,
			CVX_BLUE,
			0,
			1,
			8
			);
		cvShowImage("图片1", img_edge);
		n++;
	}


	printf("Finished all contours.\n");
}

int main(int argc, char** argv)
//IplImage *image = cvLoadImage("闸刀识别图片1.jpg");
{
	//IplImage *img_8uc1 = cvLoadImage("闸刀识别图片7李伟拍.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *img_8uc1 = cvLoadImage("闸刀识别图片3.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	img_edge1 = cvCreateImage(cvGetSize(img_8uc1), 8, 1);
	IplImage *img_8uc3 = cvCreateImage(cvGetSize(img_8uc1), 8, 3);
	img_edge = cvCreateImage(cvGetSize(img_8uc1), 8, 1);
	IplImage *hsv = cvCreateImage(cvGetSize(img_8uc1), 8, 3);
	cvNamedWindow("origin", 1);
	cvWaitKey(0);
	//cvCvtColor(img_8uc1, hsv,CV_BGR2HSV);
	cvSplit(img_8uc1, 0, 0, img_edge1, 0);
	cvCreateTrackbar("阈值", "origin", &thredholdv, 255, change);
	change(0);

	cvWaitKey(0);
	return 0;
}