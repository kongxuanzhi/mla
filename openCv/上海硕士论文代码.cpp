
#include "cv.h"
#include "highgui.h"
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	IplImage* pImgOri = NULL;
	IplImage* pImg = NULL;
	IplImage* pImgGray = NULL;

	int resize_factor = 1;

	pImgOri = cvLoadImage("闸刀识别图片7李伟拍.jpg", 1);
	pImg = cvCreateImage(cvSize(pImgOri->width / resize_factor,
		pImgOri->height / resize_factor), pImgOri->depth, pImgOri->nChannels);
	cvResize(pImgOri, pImg, 1);
	cvReleaseImage(&pImgOri);
	pImgOri = cvCloneImage(pImg);

	//background elimination 

	//step 1 
	int minimum;
	for (int y = 0; y < pImg->height; y++)
	{
		uchar* ptr = (uchar*)pImg->imageData + y * pImg->widthStep;
		for (int x = 0; x < pImg->width; x++)
		{
			minimum = min(ptr[3 * x], min(ptr[3 * x + 1], ptr[3 * x + 2]));
			ptr[3 * x] -= minimum;
			ptr[3 * x + 1] -= minimum;
			ptr[3 * x + 2] -= minimum;
		}
	}

	//step 2 
	IplImage *R, *G, *B;
	R = cvCreateImage(cvGetSize(pImg), 8, 1);
	G = cvCreateImage(cvGetSize(pImg), 8, 1);
	B = cvCreateImage(cvGetSize(pImg), 8, 1);
	cvSplit(pImg, B, G, R, 0);
	IplImage* FGImg;
	FGImg = cvCreateImage(cvGetSize(pImg), 8, 1);
	cvZero(FGImg);
	for (int y = 0; y < pImg->height; y++)
	{
		uchar* ptrR = (uchar*)R->imageData + y * R->widthStep;
		uchar* ptrG = (uchar*)G->imageData + y * G->widthStep;
		uchar*  ptrF = (uchar*)FGImg->imageData + y  * FGImg->widthStep;

		for (int x = 0; x < pImg->width; x++)
		{

			if (ptrR[x] - ptrG[x] > 50)
			{
				ptrF[x] = 0xff;
			}

			if (abs(ptrG[x] - ptrR[x])  <  20 && ptrG[x] + ptrR[x]  >
				40)//abs(ptr G[x] - 30) < 10 && abs(ptr R[x] - 30) < 10) 
			{
				ptrF[x] = 0xff;
			}

		}
	}	cvNamedWindow("show_Img", CV_WINDOW_AUTOSIZE);
	cvShowImage("show_Img", FGImg);
	cvWaitKey(0);
	cvDestroyWindow("show_Img");

	IplConvKernel* kernel;
	kernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT,0);
	cvErode(FGImg, FGImg, kernel);
	cvSmooth(FGImg, FGImg, CV_MEDIAN, 7, 7);
	cvDilate(FGImg, FGImg, kernel);
	cvDilate(FGImg, FGImg, kernel);
	cvErode(FGImg, FGImg, kernel);

	//step3 
	//getParts(FGImg, pImgOri);
	/* * * * * * * * * * * * * * * *end of showing result* * * * * * * * * * *
	*/
	cvReleaseImage(&pImgOri);
	cvReleaseImage(&pImg);
	cvReleaseImage(&pImgGray);

	return 0;
}
 	