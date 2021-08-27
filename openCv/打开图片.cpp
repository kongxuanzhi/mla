#include <iostream>

#include <highgui.h>

int main(){
	IplImage *img = cvLoadImage("BingWallpaper-2016-07-18.jpg");
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
}
