#include <cv.h>
#include <highgui.h>

int main(){
	IplImage *img = cvLoadImage("RGB.jpg", 1);
	IplImage *channel_r = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage *channel_g = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage *channel_b = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage *img_r = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	IplImage *img_g = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	IplImage *img_b = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

	cvSplit(img, channel_b, channel_g, channel_r, NULL); //分开
	cvMerge(channel_b, 0, 0, 0, img_b); //合成
	cvMerge(0, channel_g, 0, 0, img_g);
	cvMerge(0, 0, channel_r, 0, img_r);

	cvNamedWindow("ImageOrigin", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_R", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_G", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_B", CV_WINDOW_AUTOSIZE);
	cvShowImage("ImageOrigin", img);
	cvShowImage("Image_R", img_r);
	cvShowImage("Image_G", img_g);
	cvShowImage("Image_B", img_b);

	cvSaveImage("img_red.bmp", img_r);
	cvSaveImage("img_green.bmp", img_g);
	cvSaveImage("img_blue.bmp", img_b);

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&img_r);
	cvReleaseImage(&img_g);
	cvReleaseImage(&img_b);
	cvDestroyAllWindows();
}