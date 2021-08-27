#include <highgui.h>
#include <cv.h>
#include <math.h>
typedef struct Area{
	CvPoint center;  //中心点坐标
	int width; //宽度
	int height; //高度
	CvSeq* c;
	Area(CvPoint p, int w, int h){
		center = p; width = w; height = h;
	}
}Area;



/*
描述：将图片摆正
算法：从左边3度到右边3度旋转，每旋转0.3度，计算图像最上面的一个红线的点的个数
//取最大点的个数对应的旋转角度为最终角度
*/
void rotatImg(IplImage *src){
	//旋转中心为图像中心  
	CvPoint2D32f center;
	center.x = float(src->width / 2.0 + 0.5);
	center.y = float(src->height / 2.0 + 0.5);
	//计算二维旋转的仿射变换矩阵  
	float m[6];
	CvMat M = cvMat(2, 3, CV_32F, m);
	IplImage *src2 = cvCreateImage(cvGetSize(src), 8, 3);
	IplImage * src_float = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	IplImage *hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	IplImage *h = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);

	int arcMaxN = 0, arcy = 0;
	float arcm = 0.0;
	for (int i = -30; i < 30; i += 3){
		float arc = (float)i / 10.0;
		cv2DRotationMatrix(center, arc, 1, &M);
		//变换图像，并用黑色填充其余值  
		cvWarpAffine(src, src2, &M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, CvScalar(255, 0, 255));
		cvConvertScale(src2, src_float);
		cvCvtColor(src_float, hsv, CV_BGR2HSV);
		cvSplit(hsv, h, 0, 0, 0);
		int maxN = 0, miny = 0;
		for (int y = 0; y < h->height; y++){
			int rnum = 0;
			float* ptr = (float*)(h->imageData + y*h->widthStep);
			for (int x = 0; x < h->width; x++){
				float value = ptr[x];
				if (value >= 340 || (value < 14)){  //红色	
					rnum++;
				}
			}
			//	printf("%d,%d\n", rnum, y);
			if (rnum > h->width / 4){  //排除零散的红色噪声点，到一定长度了再计算
				if (rnum >= maxN){
					maxN = rnum;  //记录最大值，递增的到最大值
				}
				else{
					//printf("brak,%d, %d\n", maxN,y);
					miny = y;
					y = h->height - 1;
				}
			}
		}
		if (maxN > arcMaxN){
			arcMaxN = maxN;  //记录最大的红色点数，一个角度对应一个
			arcy = miny;   //用来调试，在图上画出 
			arcm = arc;   //记录最大红色点数，对应的角度
		}
		//cvDrawLine(src2, CvPoint(0, miny), CvPoint(h->width, miny), CvScalar(255, 0, 255));
		/*cvShowImage("cvErode1", src2);
		cvWaitKey(10);*/
	}
	cv2DRotationMatrix(center, arcm, 1, &M);
	//变换图像，并用黑色填充其余值  
	cvWarpAffine(src, src, &M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, CvScalar(255, 0, 255));
	//printf("arcMaxN:%d,arcy:%d\n", arcMaxN,arcy);
	//cvDrawLine(src, CvPoint(0, arcy), CvPoint(src->width, arcy), CvScalar(255, 0, 255));  //for debug  arcy
	/*cvShowImage("cvErode1", src);
	cvWaitKey(10);*/
}
/*
描述：将8位图像转换到hsv 32位空间，返回h通道，用来判断红色和黄色
*/
IplImage* gethImg(IplImage* src){
	IplImage* src_float = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	cvConvertScale(src, src_float);
	IplImage* hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
	IplImage* h = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	cvCvtColor(src_float, hsv, CV_BGR2HSV);
	cvSplit(hsv, h, 0, 0, 0);
	cvReleaseImage(&src_float);
	cvReleaseImage(&hsv);
	return h;
}
/*
描述：对旋转后的图像，找到开关区域
*/
CvRect getROIArea(IplImage* src){
	IplImage *h = gethImg(src);
	int min_startx = INT_MAX;
	int max_endx = 0;
	int rstarty = 0;
	int rendy = 0;
	for (int y = 0; y < h->height; y++){
		float* ptr = (float*)(h->imageData + y*h->widthStep);
		int num = 0, rnum = 0;
		int startx = 0;
		int endx = 0;
		int snum = 0; //开头最大连续黄颜色个数
		int eNum = 0;//末尾最大连续非黄颜色个数
		for (int x = 0; x < h->width; x++){
			float value = ptr[x];
			if (value >= 50 && value < 60){//黄色    //value >= 340 || value < 14
				num++;   //统计有多少黄颜色
				if (startx == 0)  //如果还没有确定开始的位置
					snum++;//统计连续的黄点数目
				if (snum == 15){ //如果，连续的黄点数够15个了，就认为找到了一个起始点
					startx = x - 15; //那么往回数15个就是起点了
					snum = 0;  //然后就不计数了
				}
				//终点一样计算，当startx已经确定的情况下，如果遇到6个像素点，是空的，则认为到了最后
				eNum = 0;
			}
			else{
				snum = 0;
				if (startx != 0){   //当startx已经确定的情况下
					if (endx == 0)
						eNum++;
					if (eNum == 6){
						endx = x - 6;
						eNum = 0;
					}
				}
			}
			if (rstarty == 0 && (value >= 340 || (value < 14))){  //红色
				rnum++;
				if (rnum > h->width / 2){
					rstarty = y;
				}
			}
			else{
				rnum = 0;
			}
		}
		if (num > h->width / 2){
		//	printf("%d,", endx);
			min_startx = startx < min_startx ? startx : min_startx;  //取最大值
			max_endx = endx > max_endx ? endx : max_endx;  //取最小值
		//	cvDrawLine(src, CvPoint(min_startx, y), CvPoint(max_endx, y), CvScalar(255, 0, 255));
		}
	}
	for (int y = h->height - 1; y >= 0; y--){
		float* ptr = (float*)(h->imageData + y*h->widthStep);
		int rnum = 0;
		for (int x = 0; x < h->width; x++){
			float value = ptr[x];
			if (rendy == 0 && (value >= 340 || (value < 14))){  //红色
				rnum++;
				if (rnum > h->width / 5){
					rendy = y;
					y = 0;
					break;
				}
			}
			else{
				rnum = 0;
			}
		}
	}
	//cvDrawRect(src, CvPoint(min_startx, rstarty + 10), CvPoint(max_endx + 10, rendy - 10), CvScalar(255, 0, 0), 3);
	CvRect rect(min_startx, rstarty + 15, max_endx+ 15 - min_startx, rendy - 25 - rstarty);
	return rect;
}



/*
输入三通道图像(in) 返回二值化的单通道边缘图像
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
/*
在二值化的图片上查找轮廓
输入二值化图像，返回轮廓序列，只检测外边界，忽略内边界
*/
CvSeq* FindCountour(IplImage *img){
	CvSeq* first_contour = NULL;
	int Nc = cvFindContours(
		img,   //在该单通道图片上查找轮廓
		cvCreateMemStorage(),
		&first_contour,
		sizeof(CvContour),
		CV_RETR_CCOMP   //CV_RETR_CCOMP //不要内部的轮廓
		);
	printf("总共的轮廓数目 : %d\n", Nc);
	return first_contour;
}

//int lineH = 40;  //小于这个值，算是一行   //可以固定  //比例
static int cmp_func(const void* _a, const void* _b, void* userdata)
{
	Area* a = (Area*)_a;
	Area* b = (Area*)_b;
	int y_diff = a->center.y - b->center.y;
	int x_diff = a->center.x - b->center.x;
	int lineH = (*(int*)userdata);
	printf("lineH:%d\t",lineH);
	if (abs(y_diff) < lineH){ //近似看作一条行
		return x_diff;  //根据x排序
	}
	else if (y_diff < 0){  //不需要调整
		return -1;
	}
	else if (y_diff > lineH){   //一个点在另一个点的下面，一定需要调整
		return 1;
	}
}
typedef struct State{
	int x;
	int y;
	bool state;  //开 false， 闭合 true
}State;

#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff)  
int main(){//斜：499，504，505 510 511 513
	IplImage *in = cvLoadImage("../use/IMG_20160812_094016.jpg");   //    503 510
	IplImage *out = cvCreateImage(cvSize(1008, 756), 8, 3);
	cvResize(in, out);

	rotatImg(out);
	CvRect rect = getROIArea(out);
	cvSetImageROI(out, rect);

	cvShowImage("origin",out);
	cvWaitKey(0);

	//做几次高斯模糊，四次差不多
	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);
	cvShowImage("cvSmooth", out);
	//cvWaitKey(0);
	cvDilate(out, out, 0, 3);//膨胀处理3次
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
#if 1//得到边缘  除去边缘
	IplImage *srcMini = cvCreateImage(cvGetSize(out), 8, 3);
	cvCopy(out, srcMini);
	IplImage *hsv = cvCreateImage(cvGetSize(srcMini), 8, 3);
	cvCvtColor(srcMini, hsv, CV_BGR2HSV);
	IplImage* s = cvCreateImage(cvGetSize(hsv), 8, 1);  //0-255
	cvSplit(hsv, 0, s, 0, 0);
	for (int y = 0; y < s->height; y++){
		uchar* ptrs = (uchar*)(s->imageData + y*s->widthStep);
		for (int x = 0; x < s->width; x++){
			ptrs[x] = ptrs[x] < 90 ? 0 : 255;
		}
	}
#endif
	//进行边缘检测
	//out = doCanny(out, 40, 40, 3);
	out = doCanny(out, 40, 40, 3);

	//cvShowImage("初始勾勒轮廓2", out);
	//cvWaitKey(0);
	cvDilate(out, out,0,4); 
	cvDilate(s, s, 0, 9);
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	cvSub(out, s, out); 
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	for (int y = 0; y <out->height; y++){
		uchar* ptr = (uchar*)(out->imageData + y*out->widthStep);
		int redNum = 0; 
		int maxRedNum = 0;
		for (int x = 0; x < out->width; x++){
			if (ptr[x] == 255){
				redNum++; 
				if (redNum > maxRedNum){
					maxRedNum = redNum;
				}
			}
			else{
				redNum = 0; 
			}
		}
		if (maxRedNum >  out->width / 6){
			for (int x = 0; x< out->width; x++){
				cvSet2D(out,y, x, CvScalar(0, 0, 0));
			}
		}
	}
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	IplConvKernel *element = cvCreateStructuringElementEx(5, 5, 0, 0, CV_SHAPE_RECT, NULL);
	cvErode(out, out, element);
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	cvDilate(out, out, 0,6);
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	cvDilate(out, out);
	cvShowImage("cvDilate3", out);
	//cvWaitKey(0);
	/*cvShowImage("初始勾勒轮廓2", out);
	//cvWaitKey(0);*/

	//去掉右上角可能出现的白色螺丝钉
	for (int y = 0; y <22; y++){
		uchar* ptr = (uchar*)(out->imageData + y*out->widthStep);
		for (int x = out->width-1; x > out->width - 22; x--){
			ptr[x] = 0;
		}
	}
#if 1
	CvSeq* first_contour = FindCountour(out);
	int nk = 0;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1);//初始化画笔

	IplImage *initContourImg1 = cvCreateImage(cvGetSize(out), 8, 3);
	float width = 0.0;
	float height = 0.0;
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		cvDrawContours(initContourImg1, c, CVX_RED, CVX_BLUE, 0, 1, 8);//画出轮廓 :-  
		CvRect rect = cvBoundingRect(c);
	//	cvRectangleR(initContourImg1, rect, cvScalarAll(255));  //画出外接矩形  
		width += (float)rect.width;
		height += (float)rect.height;
		nk++;
	//	printf("%d,", rect.height);
	//	cvShowImage("初始勾勒轮廓2", initContourImg1);
		//cvWaitKey(10);
	}
	//计算均值
	width = (float)width / nk;
	height = (float)height / nk;

	//printf("%f,", height);

	CvSeq*  point_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(Area), cvCreateMemStorage(0));//保存中心点
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		CvRect rect = cvBoundingRect(c);

		//if (rect.width >= width && rect.width<width && rect.height >= height && rect.height < height){
		if (rect.width >= width*0.5 && rect.width<width*1.5 && rect.height >= height*0.5 && rect.height < height * 1.3){
			

			CvPoint center = CvPoint((2 * rect.x + rect.width) / 2,
				(2 * rect.y + rect.height) / 2);  //获得轮廓外接矩形中心点
			Area area = Area(center, rect.width, rect.height);
			area.c = c;
			cvSeqPush(point_seq, &area);  //将中心点保存在序列中
#pragma region 画椭圆
			//char str[25];
			//itoa(nk, str, 10);
			//nk++;
			//cvPutText(initContourImg1, str, cvPoint(center.x, center.y), &font, CV_RGB(0, 255, 0));//在图片中输出字符
			//CvBox2D ellipse = cvFitEllipse2(c);//最小二乘法的椭圆拟合    
			//cvEllipseBox(initContourImg1, ellipse, CV_RGB(255, 0, 0)); //在图上画椭圆   
			//cvEllipseBox(initContourImg1, ellipse, CV_RGB(255, 0, 0)); //在图上画椭圆   
#pragma endregion
		}
	}
	//排序
	int h = (int)height;
	cvSeqSort(point_seq, cmp_func, &h);  //单个点根据中心点坐标排下序 从左到右->从上到下
	//判断开和关
	int prey = 0;
	int x = 1;
	int y = 0;
	bool bs;
	State *states = (State*)malloc(sizeof(State)* point_seq->total);
	for (int i = 0; i < point_seq->total; i++){
		Area area = (*(Area*)cvGetSeqElem(point_seq, i));
		bs = true;
		char str[25];
		itoa(i, str, 10);
		
		cvPutText(initContourImg1, str, cvPoint(area.center.x, area.center.y), &font, CV_RGB(0, 255, 0));//在图片中输出字符
#if 1
		CvRect rect = cvBoundingRect(area.c);
		int dis_lb = 1000;
		int dis_rt = 1000;
		CvPoint lb(rect.x,rect.y+rect.height);
		CvPoint rt(rect.x + rect.width, rect.y);
		CvPoint close_lb, clost_rt;
		//cvDrawCircle(initContourImg1, lb,5, CvScalar(0, 255, 255), 2);
		//cvDrawCircle(initContourImg1, rt, 5, CvScalar(0, 255, 255), 2);
		for (int ii = 0; ii < area.c->total; ii++){
			CvPoint *p = (CvPoint*)cvGetSeqElem(area.c, ii);
			int dis = (p->x - lb.x)*(p->x - lb.x) + (p->y - lb.y)*(p->y - lb.y);
			if (dis < dis_lb){
				dis_lb = dis;
				close_lb = CvPoint(p->x, p->y);
			}
			dis = (p->x - rt.x)*(p->x - rt.x) + (p->y - rt.y)*(p->y - rt.y);
			if (dis < dis_rt){
				dis_rt = dis;
				clost_rt = CvPoint(p->x, p->y);
			}
		}
		int maxx = -1; int minx = INT_MAX;
		int dist = dis_lb + dis_rt + rect.width;

		printf("%d,", dis_lb+ dis_rt + rect.width);
		//int dist_lb = (lb.x - clost_rt.x)*(lb.x - clost_rt.x) + (lb.y - clost_rt.y)*(lb.y - clost_rt.y); //矩形左下角到轮廓右上角距离
		//int dist_rt = (rt.x - close_lb.x)*(rt.x - close_lb.x) + (rt.y - close_lb.y)*(rt.y - close_lb.y); //矩形右上角到轮廓左下角距离
		//int dist = dist_rt + dist_lb;
		//cvDrawLine(initContourImg1, CvPoint(0, miny), CvPoint(initContourImg1->width, miny), CvScalar(255, 255, 0));
		//cvDrawLine(initContourImg1, CvPoint(0, maxy), CvPoint(initContourImg1->width, maxy), CvScalar(255, 255, 0));
	
		cvDrawCircle(initContourImg1, close_lb, 5, CvScalar(0, 255, 0), 2);
		cvDrawCircle(initContourImg1, clost_rt, 5, CvScalar(0, 255, 0), 2);

#else    //根据最小外接矩形的面积减去轮廓的面积的差来判断卡关的状态
		CvBox2D rect1 = cvMinAreaRect2(area.c);
		CvPoint2D32f rect_pts0[4];
		cvBoxPoints(rect1, rect_pts0); //四个顶点坐标
		int npts = 4;
		CvPoint rect_pts[4], *pt = rect_pts;
		for (int rp = 0; rp<4; rp++)
			rect_pts[rp] = cvPointFrom32f(rect_pts0[rp]);

		double x1 = (double)rect_pts[0].x, y1 = (double)rect_pts[0].y;
		double x2 = (double)rect_pts[1].x, y2 = (double)rect_pts[1].y;
		double x3 = (double)rect_pts[2].x, y3 = (double)rect_pts[2].y;
		//double arc = atan((x2-x1)/(y2-y1)); //角度，不过，不能保证所有的都很好
		double minArea = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) * sqrt((x2 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3));
		double areac = cvContourArea(area.c);
		double cha = minArea - areac;
		printf("%f, ", cha);
		
		CvPoint lb = rect_pts[0];
		CvPoint rt = rect_pts[2];
		cvDrawCircle(initContourImg1, lb, 1, CvScalar(0, 255, 0),5);
		//cvDrawCircle(initContourImg1, rt, 1, CvScalar(0, 255, 0), 5);
		
	
		cvShowImage("初始勾勒轮廓2", initContourImg1);

		if (cha < 1300){  //+ rect.width*3
			cvPolyLine(initContourImg1, &pt, &npts, 1, 1, CV_RGB(255, 0, 0), 1);
		}
		else{
			bs = false;
			cvPolyLine(initContourImg1, &pt, &npts, 1, 1, CV_RGB(0, 255, 0), 1);
		}
		if (i!=0 && abs(area.center.y - prey) > area.width * 2){
			x++;
			y = 0;
		}
		prey = area.center.y;
		y++;
#endif
		if (dist < 500){  //+ rect.width*3
			cvRectangleR(initContourImg1, rect, CV_RGB(255, 0, 0));  //画出外接矩形  
		}
		else{
			bs = false;
			cvRectangleR(initContourImg1, rect, CV_RGB(0, 255, 0));  //画出外接矩形  
		}
		if (i != 0 && abs(area.center.y - prey) > area.width * 2){
			x++;
			y = 0;
		}
		prey = area.center.y;
		y++;
		states[i].state = bs;
		states[i].x = x;
		states[i].y = y;
		cvShowImage("初始勾勒轮廓2", initContourImg1);
		cvWaitKey(0);
		////cvWaitKey(0);
	}
	/*printf("\n");
	for (int i = 0; i < point_seq->total; i++){
		printf("(x:%d,y:%d, state:%d)\n", states[i].x, states[i].y, states[i].state);
	}*/
#endif
	cvWaitKey(0);
	cvReleaseImage(&out);
}
