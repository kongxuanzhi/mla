/*===============================================//
功能：Template Matching with OpenCV
时间：3/22/2011 SkySeraph HQU
//===============================================*/
#include "iostream"
using namespace std;

#include "cv.h"
#include "highgui.h"

#include "math.h"


void Iterator(IplImage *img){
	for (int y = 0; y < img->height; y++){
		uchar* ptr = (uchar*)(img->imageData + y*img->widthStep);
		for (int x = 0; x < img->width; x++){
			//printf("(%d,%d,%d),", ptr[3 * x], ptr[3 * x + 1], ptr[3 * x + 2]);
		}
	}
}
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
	float* ranges[] = { h_ranges, s_ranges,v_ranges };
	CvHistogram *hist = cvCreateHist(3, hist_size, CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes, hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);
	return hist;
}
//RGB直方图匹配
CvHistogram* getHist4(IplImage *src){
	IplImage* RGB = cvCreateImage(cvGetSize(src), 8, 3);
	//cvCvtColor(src, hsv, CV_BGR2HSV);
	IplImage* R = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* G = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* B = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* planes[] = { R, G, B };
	cvSplit(RGB, B, G, R, 0);

	int h_bins = 255, s_bins = 255, v_bins = 255;
	int hist_size[] = { h_bins, s_bins, v_bins };
	float h_ranges[] = { 0, 255 };
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges, v_ranges };
	CvHistogram *hist = cvCreateHist(3, hist_size, CV_HIST_ARRAY, ranges, 1);

	cvCalcHist(planes, hist, 0, 0); //Compute histogram
	cvNormalizeHist(hist, 1.0);
	return hist;
}
//灰度直方图匹配
CvHistogram* getHist(IplImage *src){
	IplImage *histimg = 0;
	CvHistogram *hist = 0;                    //直方图

	int hdims = 255;
	float hranges_arr[] = { 0, 255 };
	float* hranges = hranges_arr;
	int bin_w;
	float max_val;
	float max_val2;
	int i;

	cvNamedWindow("Histogram", 0);
	cvNamedWindow("src", 0);

	hist = cvCreateHist(1, &hdims, CV_HIST_ARRAY, &hranges, 1);   //创建直方图

	histimg = cvCreateImage(cvSize(320, 200), 8, 3);

	cvZero(histimg);                                            //清零

	cvCalcHist(&src, hist, 0, 0);
	cvNormalizeHist(hist, 1);
	//cvThreshHist(hist, 0.003);

	// 计算直方图,即每个bin的大小
	cvGetMinMaxHistValue(hist, 0, &max_val, 0, 0);
//	printf("%f\n", max_val);
	// 只找最大值
	cvConvertScale(hist->bins,
		hist->bins, max_val ? 255. / max_val : 0., 0);             // 缩放 bin 到区间 [0,255]
	cvZero(histimg);
	bin_w = histimg->width / hdims;                               // hdims: 直方图竖条的个数，则 bin_w 为条的宽度

	// 画直方图
	for (i = 0; i < hdims; i++)
	{
		double val = (cvGetReal1D(hist->bins, i)*histimg->height / 255);
		CvScalar color = CV_RGB(255, 255, 0);                                 //(hsv2rgb(i*180.f/hdims);
		cvRectangle(histimg, cvPoint(i*bin_w, histimg->height),
			cvPoint((i + 1)*bin_w, (int)(histimg->height - val)),
			color, 1, 8, 0);
	}
	cvShowImage("Histogra2m", histimg);
	return hist;
}

const char* filename1 = "IMG_0502.JPG";   //IMG_0504  IMG_0505 IMG_0506
const char* filename2 = "xiaotemplate1.jpg";   //5差别太大xie2.jpg

/*=============================================*/
int main(int argc, char** argv)
{
	IplImage    *in;
	IplImage    *tpl;
	IplImage    *res;
	CvPoint     minloc, maxloc;
	double      minval, maxval;
	int         img_width, img_height;
	int         tpl_width, tpl_height;
	int         res_width, res_height;
	IplImage    *img;
	in = cvLoadImage(filename1, 1);
	tpl = cvLoadImage(filename2, 1);
	img = cvCreateImage(cvSize(800,600),8,3);
	cvResize(in, img);
	if (tpl == 0)
	{
		fprintf(stderr, "Cannot load file %s!\n", argv[2]);
		return 1;
	}
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", img);
	cvNamedWindow("template", CV_WINDOW_AUTOSIZE);
	cvShowImage("template", tpl);
	/* get image's properties */
	img_width = img->width;
	img_height = img->height;
	tpl_width = tpl->width;
	tpl_height = tpl->height;
	res_width = img_width - tpl_width + 1;
	res_height = img_height - tpl_height + 1;

	double premax = 0.0;
	CvHistogram *hist1 = getHist2(tpl);
	double compareNUm = 0.0;
	for (int i = 0; i < 100; i++){
		res = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
		//cvMatchTemplate(img, tpl, res, CV_TM_CCOEFF_NORMED);/**/
		cvMatchTemplate(img, tpl, res, CV_TM_CCORR_NORMED);
		cvMinMaxLoc(res, 0, &maxval, 0, &maxloc, 0);
		if (i == 0) premax = maxval;
		double gap = premax -maxval;
		
	//	printf("%f\n", premax);
		premax = maxval;
		CvPoint pt1;
		CvPoint pt2;
		CvRect rect;
		rect = cvRect(maxloc.x, maxloc.y, tpl->width, tpl->height);//最佳的匹配区域
		pt1 = cvPoint(rect.x, rect.y);
		pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);

		cvRectangle(img, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);
 		//cvWaitKey(0);
		IplImage *mask2 = cvCreateImage(CvSize(tpl->width, tpl->height), 8, 3);
		cvSetImageROI(img, rect);
		cvCopy(img, mask2, 0);
		cvZero(img);
		cvResetImageROI(img);
		//cvNamedWindow("mask2", CV_WINDOW_AUTOSIZE);
		//cvShowImage("mask2", mask2);
		CvHistogram *hist2 = getHist2(mask2);
		compareNUm = cvCompareHist(hist1, hist2, CV_COMP_INTERSECT);
  	// printf("%lf,", compareNUm);
		if (compareNUm < 0.2){
			break;
		}
		/* display images */
		
		//* wait until user press a key to exit */
		cvZero(res);
	}
	cvNamedWindow("reference", CV_WINDOW_AUTOSIZE);
	cvShowImage("reference", img);
	printf("fdf");
	cvWaitKey(0);
	/* free memory */
	cvDestroyWindow("reference");
	cvDestroyWindow("template");
	cvReleaseImage(&img);
	cvReleaseImage(&tpl);
	cvReleaseImage(&res);

	return 0;
}