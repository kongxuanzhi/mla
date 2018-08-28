#include <highgui.h>
#include <cv.h>
#include <math.h>
int main()
{
	IplImage * src = cvLoadImage("cut.jpg");
	IplImage* gray_plane = cvCreateImage(cvGetSize(src), 8, 1);
	cvCvtColor(src, gray_plane, CV_BGR2GRAY);
	/*cvNamedWindow("origi",1);
	cvShowImage("origi", gray_plane);
*/
	//cvSmooth(gray_plane, gray_plane, CV_GAUSSIAN);
	//cvSmooth(gray_plane, gray_plane, CV_GAUSSIAN);
	//cvSmooth(gray_plane, gray_plane, CV_GAUSSIAN);
	//cvSmooth(gray_plane, gray_plane, CV_GAUSSIAN);

	/*cvNamedWindow("cvSmooth", 1);
	cvShowImage("cvSmooth", gray_plane);*/
	cvWaitKey(0);

	IplConvKernel *kernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_ELLIPSE);
	/*cvErode(gray_plane, gray_plane, kernel);
	cvDilate(gray_plane, gray_plane, kernel);
	cvDilate(gray_plane, gray_plane, kernel);
	cvDilate(gray_plane, gray_plane, kernel);
	cvDilate(gray_plane, gray_plane, kernel);*/
	//cvNamedWindow("cvErode1", 1);
	//cvShowImage("cvErode1", gray_plane);
	cvNamedWindow("cvErode1", 1);
	cvShowImage("cvErode1", gray_plane);
	cvWaitKey(0);
	while (1){
		//cvNamedWindow("cvErode", 1);
		//cvShowImage("cvErode", gray_plane);
		//cvWaitKey(0);
		IplImage* gray_plane1 = cvCloneImage(gray_plane);
		IplImage* temp = cvCreateImage(cvGetSize(gray_plane1), 8, 1);
		cvLaplace(gray_plane1, gray_plane1);
		cvNamedWindow("cvErode", 1);
		cvShowImage("cvErode", gray_plane1);
		cvWaitKey(0);
		cvMorphologyEx(gray_plane1, gray_plane1, temp, kernel, CV_MOP_TOPHAT, 1);
		cvNamedWindow("cvErode", 1);
		cvShowImage("cvErode", gray_plane1);
		cvWaitKey(0);

		cvSub(gray_plane, gray_plane1, gray_plane1);
		cvNamedWindow("cvErode", 1);
		cvShowImage("cvErode", gray_plane1);
		cvWaitKey(0);

		CvMemStorage* storage = cvCreateMemStorage(0);

		CvSeq *res = cvHoughCircles(
			gray_plane1,
			storage,
			CV_HOUGH_GRADIENT,
			2,
			gray_plane1->width/20
			);
		printf("total:%d\n",res->total);

		for (int i = 0; i < res->total; i++){
			float* p = (float*)cvGetSeqElem(res, i);
			CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
			cvCircle(gray_plane1,
				pt, cvRound(p[2]),
				CV_RGB(0xff,0xff,0xff)
				);
		}
		cvNamedWindow("cvErode", 1);
		cvShowImage("cvErode", gray_plane1);
		cvWaitKey(0);
	}
	
}