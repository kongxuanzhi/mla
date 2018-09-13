#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff)  
int main(int argc, char** argv)
	//IplImage *image = cvLoadImage("闸刀识别图片1.jpg");
{
	cvNamedWindow("图片1", 1);
	//IplImage *img_8uc1 = cvLoadImage("闸刀识别图片7李伟拍.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *img_8uc1 = cvLoadImage("th.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	IplImage *img_edge = cvCreateImage(cvGetSize(img_8uc1), 8, 1);
	IplImage *img_8uc3 = cvCreateImage(cvGetSize(img_8uc1), 8, 3);

	cvThreshold(img_8uc1, img_edge, 128, 255, CV_THRESH_BINARY);
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	int Nc = cvFindContours(
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_LIST
		);
	int n = 0;
	printf("Total Contours Detected : %d\n", Nc);
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		cvCvtColor(img_8uc1,img_8uc3, CV_GRAY2BGR);
		cvDrawContours(img_8uc3, c,
			CVX_RED,
			CVX_BLUE,
			0,
			2,
			8
			);
		printf("Contour #%d\n", n);
		cvShowImage("图片1", img_8uc3);
		printf(" %d elements:\n", c->total);
		for (int i = 0; i < c->total; ++i){
			CvPoint* p = CV_GET_SEQ_ELEM(CvPoint,c,i);
			printf("  (%d,%d)\n", p->x, p->y);
		}
		cvWaitKey(100);
		n++;
	}
	

	printf("Finished all contours.\n");
	cvCvtColor(img_8uc1, img_8uc3, CV_GRAY2BGR);
	
	cvShowImage("图片1", img_8uc3);

	cvWaitKey(0);
	return 0;
}





6B581QW-D21-9049C8AA3N 