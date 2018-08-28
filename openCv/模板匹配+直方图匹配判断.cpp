#include "stdafx.h"
#include "ProcessWhite.h"

//hs直方图匹配
CvHistogram* getHist1(IplImage *src){
	IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* planes[] = { h_plane, s_plane };
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	int h_bins = 180, s_bins = 255;
	int hist_size[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };
	CvHistogram *hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes, hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);
	return hist;
}
//sv直方图匹配
CvHistogram* getHist2(IplImage *src){
	IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* planes[] = { s_plane, v_plane };
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	int s_bins = 255, v_bins = 255;
	int hist_size[] = { s_bins, v_bins };
	float h_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, v_ranges };
	CvHistogram *hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes, hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);
	return hist;
}
//hsv直方图匹配
CvHistogram* getHist3(IplImage *src){
	IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* planes[] = { h_plane, s_plane, v_plane };
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	int h_bins = 180, s_bins = 255, v_bins = 255;
	int hist_size[] = { h_bins, s_bins, v_bins };
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges, v_ranges };
	CvHistogram *hist = cvCreateHist(3, hist_size, CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes, hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);
	return hist;
}

vector<CvPoint> ProcessWhite::CompareGetPoints(IplImage *tpl){
	IplImage    *res;   //保存对比结果
	IplImage    *mask;  //从图片中截取一个矩形
	CvPoint  maxloc;  //最佳匹配
	double   maxval;  //最佳匹配值
	double compareNum = 0.0; //  直方图比较值
	int res_width = Img->width - tpl->width + 1;
	int res_height = Img->height - tpl->height + 1;
	CvHistogram *hist1 = getHist3(tpl);
	res = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
	IplImage *imgcopy = cvCreateImage(cvGetSize(Img), 8, 3);
	cvCopy(Img,imgcopy);
	vector<CvPoint> points;
	for (int i = 0; i < 30; i++){
		cvMatchTemplate(imgcopy, tpl, res, CV_TM_CCORR_NORMED);  //CV_TM_CCOEFF_NORMED
		cvMinMaxLoc(res, 0, &maxval, 0, &maxloc, 0);
		//printf("%f\n", maxval);
		CvPoint pt1;
		CvPoint pt2;
		CvRect rect;
		rect = cvRect(maxloc.x, maxloc.y, tpl->width, tpl->height);//最佳的匹配区域
		cvRectangleR(Img, rect, cvScalar(0, 0, 255), 1, 8, 0);
		//cvWaitKey(0);

		mask = cvCreateImage(CvSize(tpl->width, tpl->height), 8, 3);
		cvSetImageROI(imgcopy, rect);
		cvCopy(imgcopy, mask, 0);
		cvZero(imgcopy);
		cvResetImageROI(imgcopy);
		CvHistogram *hist2 = getHist3(mask);
		compareNum = cvCompareHist(hist1, hist2, CV_COMP_INTERSECT);
		printf("%lf,", compareNum);
		if (compareNum < 0.04){
			break;
		}
		points.push_back(maxloc);
		cvZero(res);
		cvReleaseHist(&hist2);
		cvReleaseImage(&mask);
	}
	cvReleaseImage(&tpl);
	cvReleaseImage(&imgcopy);
	cvReleaseImage(&res);
	cvReleaseHist(&hist1);
	return points;
}



//.h

class ProcessWhite
{
public:
	IplImage *Img; //原始图像
	int width;
	int height;
public:
	ProcessWhite(IplImage *Img, int width, int  height){
		this->Img = Img;
		this->width = width;
		this->height = height;
	}
	vector<CvPoint> CompareGetPoints(IplImage *tpl);
};