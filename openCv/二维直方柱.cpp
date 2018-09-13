#include "cv.h"
#include "highgui.h"


int main(int argc, char** argv)
{
	IplImage *src;
	src = cvLoadImage("Lena.jpg");

	IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src,hsv, CV_BGR2HSV);

	IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage* planes[] = {h_plane, s_plane};
	cvSplit(hsv, h_plane, s_plane, v_plane,0);

	//Build the histogram and compute its contents
	int h_bins = 30, s_bins = 32;
	int hist_size[] = { h_bins, s_bins };
	float h_ranges[] = {0,180};
	float s_ranges[] = { 0, 255 };
	float* ranges[] = {h_ranges, s_ranges};
	CvHistogram *hist = cvCreateHist(2,hist_size,CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes,hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);

	//create an image to use to visualize our histogram
	int scale = 10;
	IplImage* hist_img = cvCreateImage(cvSize(h_bins*scale, s_bins*scale),
		8, 3
		);
	cvZero(hist_img);

	//populate our visualization with little gray squares.
	float max_value = 0;
	cvGetMinMaxHistValue(hist,0, &max_value,0,0);
	for (int h = 0; h < h_bins; h++){
		for (int s = 0; s < s_bins; s++){
			float bin_val = cvGetReal2D(hist->bins,h,s);
			int intensity = cvRound(bin_val* 255/max_value);
			cvRectangle(
				hist_img,
				cvPoint(h*scale, s*scale),
				cvPoint((h + 1)*scale - 1, (s + 1)*scale - 1),
				CV_RGB(intensity, intensity, intensity),
				CV_FILLED
				);
		}
	}

	cvNamedWindow("Source", 1);
	cvShowImage("Source", src);

	cvNamedWindow("H-S histogram", 1);
	cvShowImage("H-S histogram", hist_img);
	cvWaitKey(0);
	return 0;

}