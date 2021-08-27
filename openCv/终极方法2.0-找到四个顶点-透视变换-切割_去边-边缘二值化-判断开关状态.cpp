#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <string>
#include <iostream>
#include <stdio.h>  
#include <direct.h> 
using namespace std;

IplImage** gethImg(IplImage* src){
	IplImage* src_float = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	cvConvertScale(src, src_float);
	IplImage* hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	IplImage **hsv3 = new IplImage*[3];
	hsv3[0] = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	hsv3[1] = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	hsv3[2] = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	cvCvtColor(src_float, hsv, CV_BGR2HSV);
	cvSplit(hsv, hsv3[0], hsv3[1], hsv3[2], 0);
	cvReleaseImage(&src_float);
	cvReleaseImage(&hsv);
	return hsv3;
}

int GetDis2P(CvPoint a, CvPoint b){
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}
/*输入三通道图像(in) 返回二值化的单通道边缘图像
*/
IplImage *doCanny(IplImage *in,
	double lowThresh,
	double highThresh,
	double aperture){
	if (in->nChannels == 1){
		return 0;
	}
	IplImage *out = cvCreateImage(cvGetSize(in),8,1);
	cvCanny(in, out, lowThresh, highThresh, aperture);//边缘检测
	return out;
}
// 四舍五入
int Round(float x)
{
	return (int)(x < 0 ? x - 0.5 : x + 0.5);
}

float getHValueXY(int x, int y, IplImage *h){
	return ((float*)(h->imageData + y*h->widthStep))[x];
}
float getSValueXY(int x, int y, IplImage *s){
	return 255 * ((float*)(s->imageData + y*s->widthStep))[x];
}
bool isRedColor(float h_val, float s_val){
	if ((h_val >= 340 || h_val < 30) && s_val > 60) {  //饱和度很重要
		return true;
	}
	return false;
}
bool isYellowColor(float h_val, float s_val){
	if ((h_val > 45 && h_val < 70) && s_val > 100) {  //饱和度很重要
		//if ((h_val > 45 && h_val < 70) && s_val > 130) {  //饱和度很重要
		return true;
	}
	return false;
}
/*
description:根据一系列点拟合一条直线
输入：点集
输出：由直线上一点和直线单位向量 得到的直线两端点
*/
CvPoint* GetLine(vector<CvPoint> points,IplImage *src){
	CvPoint* points2 = (CvPoint*)malloc(points.size() * sizeof(points[0]));
	for (int i = 0; i < points.size(); i++){
		CvPoint p = points[i];
		points2[i] = p;
	}
	CvMat pointMat = cvMat(1, points.size(), CV_32SC2, points2); //点集, 存储count个随机点points  
	float line[4];
	cvFitLine(&pointMat, CV_DIST_L1, 1, 0.001, 0.001, line); //核心拟合函数，也很耗时

   	printf("line4:%f,%f,%f,%f\n", line[0], line[1], line[2], line[3]);//0、1存储直线上一点， 2和3存储直线的单位向量
	CvPoint pt1, pt2; //直线的两个端点  
	CvPoint* pts = (CvPoint*)malloc(2 * sizeof(points[0]));
	float d, t;
	d = sqrt((double)line[0] * line[0] + (double)line[1] * line[1]);  //line[0 & 1]存储的是单位向量，所以d=1  
	//printf("\n %f\n", d);  
	line[0] /= d;
	line[1] /= d;
	//画出线段的两个端点(避免线太短，以线上一个随机点向两侧延伸line[0]*t )  
	t = (float)(src->width / 2 + src->height / 2);
	pts[0].x = cvRound(line[2] - line[0] * t);
	pts[0].y = cvRound(line[3] - line[1] * t);
	pts[1].x = cvRound(line[2] + line[0] * t);
	pts[1].y = cvRound(line[3] + line[1] * t);
	//cvLine(src, pts[0], pts[1], CV_RGB(0, 255, 0), 3, CV_AA, 0); //在图像上画出直线
	return pts;  //返回端点
}
//扫描左边的红色边缘，调用GetLine拟合一条直线，返回线段的两端点
CvPoint* getLeftLine(IplImage *src, IplImage *h, IplImage *s){
	vector<CvPoint> points;
	int minx = -1;
	int x_limit = h->width;
	for (int x = 0; x < x_limit; x++){  // 25   h->width  0  --
		int num = 0;
		for (int y = 0; y < h->height; y++){ // h->height;  //f
			float h_val = getHValueXY(x, y, h);
			float s_val = getSValueXY(x, y, s);
			if (isRedColor(h_val, s_val)) {
				num++;
				if (x_limit==h->width && num > 25){
					x_limit = x + 25;
					break;
				}
				if (x_limit != h->width){
					points.push_back(CvPoint(x, y));
				}
			}
			else{
				num = 0;
			}
		}
		printf("%d,", num);
	}
	//cvDrawLine(src, CvPoint(minx, 0), CvPoint(minx, h->height), CvScalar(255, 255, 0));  //参考直线
	return GetLine(points, src);
}
//扫描右边的红色边缘，调用GetLine拟合一条直线，返回线段的两端点
CvPoint* getRightLine(IplImage *src, IplImage *h, IplImage *s){
	vector<CvPoint> points;
	int minx = -1;
	int x_limit = 0;
	for (int x = h->width - 1; x >= x_limit; x--){  // 25   h->width  0  --
		int num = 0;
		for (int y = 0; y < h->height; y++){ // h->height;  //f
			float h_val = getHValueXY(x, y, h);
			float s_val = getSValueXY(x, y, s);
			if (isRedColor(h_val, s_val)) {
				num++;
				if (x_limit == 0 && num > 25){
					minx = x;
					x_limit = x - 25;
					break;
				}
				if (x_limit != 0){
					points.push_back(CvPoint(x, y));
				}
			}
			else{
				num = 0;
			}
		}
		printf("%d,", num);
	}
	//cvDrawLine(src, CvPoint(minx, 0), CvPoint(minx, h->height), CvScalar(255, 255, 0));
	return GetLine(points, src);
}
//扫描上边的红色边缘，调用GetLine拟合一条直线，返回线段的两端点
CvPoint* getTopLine(IplImage *src, IplImage *h, IplImage *s){
	vector<CvPoint> points;
	int miny = -1;
	int y_limit = h->height;
	for (int y = 0; y < y_limit; y++){
		int num = 0;
		for (int x = 0; x < h->width; x++){  // 25   h->width  0  --
			float h_val = getHValueXY(x, y, h);
			float s_val = getSValueXY(x, y, s);
			if (isRedColor(h_val, s_val)) {
				num++;
				if (y_limit == h->height && num > 25){
					miny = y;
					y_limit = y + 25;
					break;
				}
				if (y_limit != h->height){
					points.push_back(CvPoint(x, y));
				}
			}
			else{
				num = 0;
			}
		}
		printf("%d,", num);
	}
	//cvDrawLine(src, CvPoint(0, miny), CvPoint(h->width, miny), CvScalar(255, 255, 0));
	return GetLine(points, src);
}
//扫描下边的红色边缘，调用GetLine拟合一条直线，返回线段的两端点
CvPoint* getBottomLine(IplImage *src, IplImage *h, IplImage *s){
	vector<CvPoint> points;
	int maxy = -1;
	int y_limit = 0;
	for (int y = h->height-1; y >= y_limit; y--){
		int num = 0;
		for (int x = 0; x < h->width; x++){  // 25   h->width  0  --
			// h->height;  //f
			float h_val = getHValueXY(x, y, h);
			float s_val = getSValueXY(x, y, s);
			//printf("(%f, %f)\n",h_val, s_val);
			//cvSetReal2D(h, y, x, 0);
			if (isRedColor(h_val, s_val)) {
				num++;
				if (y_limit == 0 && num > 25){
					maxy = y;
					y_limit = y - 25;
					break;
				}
				if (y_limit != 0){
					points.push_back(CvPoint(x, y));
				}
			}
			else{
				num = 0;
			}
		}
		printf("%d,", num);
	}
	//cvDrawLine(src, CvPoint(0, maxy), CvPoint(h->width, maxy), CvScalar(255, 255, 0));
	return GetLine(points, src);
}
/*
description:求两直线交点
知道两线段的顶点，根据三点组成一条直线，求得未知交点
*/
CvPoint getTouchP(CvPoint* lp, CvPoint* tp){
	double x0 = lp[0].x, y0 = lp[0].y;
	double x1 = lp[1].x, y1 = lp[1].y;
	double x2 = tp[0].x, y2 = tp[0].y;
	double x3 = tp[1].x, y3 = tp[1].y;
	printf("%f \t %f\n", x0, y0);
	printf("%f \t %f\n", x1, y1);
	printf("%f \t %f\n", x2, y2);
	printf("%f \t %f\n", x3, y3);

	double k1 = x0 == x1 ? INT_MAX :(y0 - y1) / (x0 - x1);//斜率等于无穷大
	double k2 = x2 == x3 ? INT_MAX : (y2 - y3) / (x2 - x3);//斜率等于无穷大
	printf("%f \t %f\n", k1, k2);

	double x = k1 == k2 ? 0: (k1*x0 - k2*x2 + y2 - y0) / (k1 - k2);
	double y = y0 + (x - x0)*k1;
	printf("(%f %f)\n",  x,y);
	return CvPoint(x, y);
}


#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff) 


//整体平均值法
int getThreshVal_1(IplImage *s_8){
	//确定阈值 //需要改进
	CvScalar avea = cvAvg(s_8);
	printf("avea:%f\n", avea.val[0]);
	return  avea.val[0];
}

//轮廓边界像素点平均值法
int getThreshVal_2(IplImage *s_8, IplImage *edges){
	//确定阈值 //需要改进
	CvScalar avea2 = cvAvg(s_8, edges);
	printf("avea2:%f\n", avea2.val[0]);
	return  avea2.val[0];
}

//除去轮廓边缘平均值法
int getThreshVal_3(IplImage *src_8, CvRect rect,  IplImage *edges){
	//确定阈值 //需要改进
	int sum = 0, sk = 0;
	for (int ii =5; ii < rect.height - 5; ii++){
		uchar* ptr = (uchar*)(edges->imageData + edges->widthStep * ii);
		uchar* ptrs8 = (uchar*)(src_8->imageData + src_8->widthStep * ii);
		for (int jj = 6; jj < rect.width - 6; jj++){
			int val = ptr[jj];
			int vals = ptrs8[jj];
			if (val == 255){
				sum += vals;
				//	printf("%d ",sum);
				sk++;
			}
		}
	}
	int avea = sk == 0? 255: sum / sk;
	//printf("avea1:%d\n", avea);
	return  avea;
}
IplImage* ResizeImg(char* filename){
	IplImage* in = cvLoadImage(filename, 1);
	//IplImage* in = cvLoadImage("IMG_1118.JPG", 1);
	IplImage *src = cvCreateImage(cvSize(1008, 756), 8, 3);
	//IplImage *src = cvCreateImage(cvSize(504, 378), 8, 3);
	cvResize(in, src);
	cvShowImage("src", src);
	cvReleaseImage(&in);
	return src;
}

CvPoint* getVertexs(IplImage *src){
	IplImage **hs = gethImg(src);
	IplImage *h = hs[0];
	IplImage *s = hs[1];

	CvPoint* lp = getLeftLine(src, h, s);
	CvPoint* rp = getRightLine(src, h, s);
	CvPoint* tp = getTopLine(src, h, s);
	CvPoint* bp = getBottomLine(src, h, s);
	printf("1(%d %d)\n", rp[0].x, rp[0].y);
	printf("1(%d %d)\n", rp[1].x, rp[1].y);
	printf("2(%d %d)\n", tp[0].x, tp[0].y);
	printf("3(%d %d)\n", tp[1].x, tp[1].y);
	CvPoint *vexs = (CvPoint*)malloc(4 * sizeof(CvPoint));

	vexs[0] = getTouchP(lp, tp);  //lt
	vexs[1] = getTouchP(rp, tp);  //rt
	vexs[2] = getTouchP(rp, bp);	//rb
	vexs[3] = getTouchP(lp, bp);	//lb
	//printf("xx(%d %d)\n", lt.x, lt.y);
	for (int i = 0; i < 4; i++){
		cvDrawCircle(src, vexs[i], 5, CvScalar(0, 0, 255), 3);
	}
	//cvShowImage("src", src);
	//cvWaitKey(0);
	cvReleaseImage(hs);
	return vexs;
}

IplImage* PersTrans(CvPoint* vexs, IplImage* src){
	CvPoint2D32f srcTri[4], dstTri[4];
	CvMat*  warp_mat = cvCreateMat(3, 3, CV_32FC1);
	IplImage* dst = cvCloneImage(src);
	cvZero(dst);
	//查找四个点逆时针 左上，右上，右下，左下
	for (int i = 0; i < 4; i++){
		srcTri[i].x = vexs[i].x;
		srcTri[i].y = vexs[i].y;
	}
	dstTri[0].x = 0;
	dstTri[0].y = 0;
	dstTri[1].x = dst->width - 1;
	dstTri[1].y = 0;
	dstTri[2].x = dst->width - 1;
	dstTri[2].y = dst->height - 1;
	dstTri[3].x = 0;
	dstTri[3].y = dst->height - 1;
	cvGetPerspectiveTransform(srcTri, dstTri, warp_mat);
	cvWarpPerspective(src, dst, warp_mat);
	//cvShowImage("src", src);
	//cvShowImage("Affine_Transform", dst);
	//cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseMat(&warp_mat);
	return dst;
}

void GetTbBound(IplImage * dst, vector<int> &rY, vector<int> &yY){
	IplImage *dst_copy = cvCloneImage(dst);
	IplImage **hsdst = gethImg(dst_copy);
	IplImage *hdst = hsdst[0];
	IplImage *sdst = hsdst[1];
	printf("------------------\n");
	for (int y = dst_copy->height - 20; y >= 5; y--){
		int h[5] = { 0 };
		int ys = yY.size();
		int rs = rY.size();
		//printf("ys:%d, rs:%d\n",ys,rs);
		for (int x = 0; x < dst_copy->width; x++){
			for (int dy = y + 2; dy > y - 3; dy--){ //  12 - 8  y=10
				float valh = getHValueXY(x, dy, hdst);
				float vals = getSValueXY(x, dy, sdst);
				if (ys == rs){
					if (isYellowColor(valh, vals)){
						h[(y + 2) - dy]++;
					}
				}
				else{
					if (isRedColor(valh, vals)){
						h[(y + 2) - dy]++;
					}
				}
			}
		}
		if (ys == rs && h[0] > dst->width *0.5 && h[1] > dst->width *0.5 && h[2] > dst->width *0.5 && h[3] < dst->width / 2 && h[4] < dst->width / 2){
			cvDrawLine(dst_copy, CvPoint(0, y), CvPoint(dst->width, y), CvScalar(0, 225, 0), 2);
			yY.push_back(y);
		}
		else if (ys != rs){
			if (h[0] <  dst->width / 2 && h[1] < dst->width / 2 && h[2] < dst->width / 2 && h[3] > dst->width*0.45 && h[4] > dst->width *0.45){
				cvDrawLine(dst_copy, CvPoint(0, y), CvPoint(dst->width, y), CvScalar(0, 229, 255), 2);
				rY.push_back(y);
			}
		}
	}
	rY.push_back(5);
	/*for (int i = 1; i < 9; i++)
	{
	cvDrawLine(dst_copy, CvPoint(i*dst->width / 9, 0), CvPoint(i*dst->width / 9, dst->height), CvScalar(0, 229, 255), 2);
	}*/
	cvShowImage("dst_copy", dst_copy);
	//cvWaitKey(0);
	//去边框
	//cvLine(dst, CvPoint(200, 200), CvPoint(2, 200),CvScalar(255,0,255), 6);
	for (int x = 0; x <dst->width; x++){
		int rnum = 0;
		for (int y = 0; y < dst->height; y++){
			float valh = getHValueXY(x, y, hdst);
			float vals = getSValueXY(x, y, sdst);
			if (isRedColor(valh, vals)){
				rnum++;
				if (rnum > dst->height / 4){
					for (int k = 0; k < dst->height; k++){
						cvSet2D(dst, k, x, CvScalar(0, 0, 0));
					}
					break;
				}
			}
			else{
				rnum = 0;
			}
		}
	}
	//cvShowImage("切割图片", dst);
	printf("(%d,%d)\n", rY.size(), yY.size());  //(4,4)
	cvReleaseImage(&dst_copy);
	cvReleaseImage(hsdst);
}

void GetTbBound1(IplImage * dst, vector<int> &rY, vector<int> &yY){
	IplImage *dst_copy = cvCloneImage(dst);
	IplImage **hsdst = gethImg(dst_copy);
	IplImage *hdst = hsdst[0];
	IplImage *sdst = hsdst[1];
	int preyMaxNum = 0;
	int prerMaxNum = 0;

	for (int y = dst_copy->height - 20; y >= 20; y--){
		int ynum = 0;
		int rnum = 0;
		for (int x = 0; x < dst_copy->width; x++){
			float valh = getHValueXY(x, y, hdst);
			float vals = getSValueXY(x, y, sdst);
			if (isYellowColor(valh, vals)){
				ynum++;
				cvDrawCircle(dst_copy, CvPoint(x, y), 1, CvScalar(255, 255, 255), 1);
			}
			/*else{
			cvDrawCircle(dst_copy, CvPoint(x, y), 1, CvScalar(0, 0, 0), 1);
			}*/
			if (isRedColor(valh, vals)){
				rnum++;
				cvDrawCircle(dst_copy, CvPoint(x, y), 1, CvScalar(255, 255, 255), 1);
			}
			/*else{
			cvDrawCircle(dst_copy, CvPoint(x, y), 1, CvScalar(0, 0, 0), 1);
			}*/
		}
		prerMaxNum = prerMaxNum < rnum ? prerMaxNum : rnum;
		if (prerMaxNum < dst->width / 5 && rnum > dst->width / 2){
			printf("r:%d\n", y);
			cvDrawLine(dst, CvPoint(0, y), CvPoint(dst->width, y), CvScalar(255, 100, 244), 2);
			prerMaxNum = rnum;
			rY.push_back(y);
		}
		preyMaxNum = preyMaxNum > ynum ? preyMaxNum : ynum;
		if (preyMaxNum > dst->width * 2 / 3 && ynum < dst->width / 2){
			printf("y:%d\n", y);
			cvDrawLine(dst, CvPoint(0, y), CvPoint(dst->width, y), CvScalar(255, 100, 244), 2);
			preyMaxNum = 0;
			yY.push_back(y);
		}
	}
	cvShowImage("dst_copy", dst_copy);
	//去边框
	//cvLine(dst, CvPoint(200, 200), CvPoint(2, 200),CvScalar(255,0,255), 6);
	for (int x = 0; x <dst->width; x++){
		int rnum = 0;
		for (int y = 0; y < dst->height; y++){
			float valh = getHValueXY(x, y, hdst);
			float vals = getSValueXY(x, y, sdst);
			if (isRedColor(valh, vals)){
				rnum++;
				if (rnum > dst->height / 4){
					for (int k = 0; k < dst->height; k++){
						cvSet2D(dst, k, x, CvScalar(0, 0, 0));
					}
					break;
				}
			}
			else{
				rnum = 0;
			}
		}
	}
	cvShowImage("切割图片", dst);
	rY.push_back(0);
	printf("(%d,%d)\n", rY.size(), yY.size());  //(4,4)
	cvReleaseImage(&dst_copy);
	cvReleaseImage(hsdst);
}				

IplImage* Threshold(IplImage* dst, CvRect rect){
	IplImage* s = gethImg(dst)[1];
	cvShowImage("swichrect", dst);
	IplImage *s_8 = cvCreateImage(cvGetSize(dst), 8, 1);
	IplImage *s_dis = cvCreateImage(cvGetSize(dst), 8, 1);
	cvConvertScale(s, s_8, 255);
	cvShowImage("s_8图片", s_8);

	//二值化开关
	IplImage *edges = doCanny(dst, 100, 130, 3);
	//cvShowImage("doCanny图片", edges);
	int avea1 = getThreshVal_3(s_8, rect, edges);
	//int avea1 = getThreshVal_2(s_8, edges);
	cvThreshold(s_8, s_dis, avea1, 255, CV_THRESH_BINARY);

	cvReleaseImage(&s);
	cvReleaseImage(&s_8);
	cvReleaseImage(&edges);
	return s_dis;
}
char foldname[20] = "r_49";   //r_26 歪   39 
int n;


bool getSwichState1(IplImage *s_dis, IplImage *dst, CvRect rect, int n){
	int maxX = 0;
	int maxSum = 0;
	for (int x = 0; x < rect.width; x++){
		int h[30] = { 0 };
		int sum = 0;
		for (int y = 0; y < rect.height; y++){
			uchar* ptr = (uchar*)(s_dis->imageData + s_dis->widthStep * y);
			for (int dx = x - 15; dx < x + 15; dx++){
				if (dx >= 0 && dx < rect.width){
					int val = ptr[dx];
					if (val == 255){
						h[dx - (x - 15)]++;
					}
				}
			}
		}
		for (int i = 0; i < 30; i++){
			sum += h[i];
		}
		//printf("x:%d:  ",x);
		if (sum > maxSum){
			maxSum = sum;
			//printf("maxSum:%d ", maxSum);
			maxX = x;
		}
		//printf("\n");
	}
	/*printf("maxX:%d\n", maxX);
	printf("maxSum/10:%d\n", maxSum/20);
	printf("rect.height:%d\n", maxSum/rect.height);*/

	char nu[34];
	char nums[200];
	double avg = (double)maxSum / ((double)rect.height*30);
	sprintf_s(nu, "%.1f", avg);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 30, 2, 8);
	if (avg <= 0.55){
		sprintf_s(nums, "%s/%f_close_%d.jpg", foldname, avg, n);
		//printf("num:%s\n", nums);
		cvSaveImage(nums, s_dis);
		//printf("关");
		cvPutText(dst, nu, CvPoint(maxX - 20, rect.height / 2), &font, CV_RGB(0, 255, 255));
		cvDrawLine(dst, CvPoint(maxX, 0), CvPoint(maxX, rect.height), CvScalar(0, 255, 255), 2);
		return false;

	}
	else{
		sprintf_s(nums, "%s/%f_open_%d.jpg", foldname, avg, n);
		cvSaveImage(nums, s_dis);
		cvPutText(dst, nu, CvPoint(maxX - 20, rect.height / 2), &font, CV_RGB(0, 0, 255));
		cvDrawLine(dst, CvPoint(maxX, 0), CvPoint(maxX, rect.height), CvScalar(0, 0, 255), 2);

		return true;
	}
	//cvWaitKey(0);
}

bool getSwichState(IplImage *s_dis, CvRect rect){
	int nbighud = 0;
	int maxBigHud = 0;
	int nbignine = 0;
	int maxBigNine = 0;
	for (int y = 0; y < rect.height; y++){
		int num = 0;
		int maxBigHud = 0;
		for (int x = 0; x < rect.width; x++){
			uchar* ptr = (uchar*)(s_dis->imageData + s_dis->widthStep * y);
			int val = ptr[x];
			if (val == 255){
				num++;
				if (num > maxBigHud){
					maxBigHud = num;
				}
			}
			else{
				num = 0;
			}
		}
		printf("%d,", maxBigHud);
	}
	printf("\n");
	//printf("\nmaxBigHud: %d  maxBigNine:%d\n", maxBigHud, maxBigNine);
	char nums[200];
	//如果连续出现大于高度一半的柱子大于13根，并且出现连续出现高度大于1/3的柱子出现20根 就认为是关的否则就是开的
	if (maxBigHud >= rect.width / 8 && maxBigNine >= rect.width/12){  //&& maxcontbignine > 10  //maxcontbighud > 5
		sprintf(nums, "%s/%d_%d_close_%d.jpg", foldname, maxBigHud, maxBigNine, n);
		//printf("num:%s\n", nums);
		cvSaveImage(nums, s_dis);
		//printf("关");
		return true;
	}
	else{
		//printf("开");
		sprintf(nums, "%s/%d_%d_open_%d.jpg", foldname, maxBigHud, maxBigNine, n);
		//printf("num:%s\n", nums);
		cvSaveImage(nums, s_dis);
		return false;
	}
}
int main()
{
#if 1
	char filename[100];
	sprintf(filename, "use/%s.jpg", foldname);
	_mkdir(foldname);
	IplImage *src = ResizeImg(filename);
	//cvWaitKey(0);
	
 	CvPoint *vertexs =  getVertexs(src);
	printf("%d", vertexs[2].y);
#endif
#if 1  //透视变换
	IplImage *dst = PersTrans(vertexs, src);
	//cvShowImage("透视变换", dst);

#if 1  //得到y方向上的小矩形上下界
	vector<int> yY;
	vector<int> rY;
	GetTbBound(dst, rY, yY);
	//cvWaitKey(0);

#endif  //得到y的 
#if 1  // 遍历所有的方格（开关），判断开关状态
	int rown = yY.size();  //4
	int coln = 9;  //9 //默认为9列  可能需要改
	for (int i = rown - 1; i >= 0; i--){
		for (int j = 0; j < coln; j++){
			CvRect rect(j*dst->width / 9, rY[i], dst->width / 9, yY[i] - rY[i]);

			cvRectangleR(dst, rect, CvScalar(0, 0, 0), 6);
			cvSetImageROI(dst, rect);  //设置感兴趣区域
			//cvSmooth(dst, dst,0,2);
			//s32通道转8位
		//	printf("rect.height: %d", rect.height);

			IplImage *s_dis = Threshold(dst, rect);
			cvDilate(s_dis, s_dis);

			cvShowImage("s_dis图片", s_dis);
			//cvWaitKey(1000);
#if 1  //判断开和关状态
			n = ((rown - i - 1) * 9 + j + 1);
		//	printf(" n:%d\n", n);
			
			//getSwichState(s_dis, rect);
			getSwichState1(s_dis, dst, rect,n);
#endif
			cvWaitKey(5);
			cvResetImageROI(dst);
			cvReleaseImage(&s_dis);
		}
	}
	cvShowImage("dst", dst);
	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	return 0;

#endif

#endif
}



