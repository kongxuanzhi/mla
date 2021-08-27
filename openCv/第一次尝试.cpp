#include <highgui.h>
#include <cv.h>
#include<vector>
#include <algorithm>
#include <iostream>
using  namespace std;

#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff)  
int lineH = 25;  //小于这个值，算是一行   //可以固定
int thresholdx = 20;  // x1-x2小于这个值的算是一类   //可以固定
int thresholdy = 110;  //y1-y2小于这个值的算是一类   // 重新弄
int max_len = 300; //大于300高的去掉     //
int state_len = 75;  //长度小于75的算打开
int max_area = 3000; //面积大于这个的过滤掉
int min_area = 200;  //小于这个的过滤掉

typedef struct Area{
	CvPoint center;
	int width;
	int height;
	int x;
	int y;  //索引坐标
	Area(CvPoint p, int w, int h){
		center = p; width = w; height = h;
	}
}Area;


static int cmp_func(const void* _a, const void* _b, void* userdata)
{
	Area* a = (Area*)_a;
	Area* b = (Area*)_b;
	int y_diff = a->center.y - b->center.y;
	int x_diff = a->center.x - b->center.x;
	//int lineH = *(int*)userdata;
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


bool getInstance(const CvScalar color, CvScalar g_piple, CvScalar gaps){
	return abs(color.val[0] - g_piple.val[0]) < gaps.val[0]
		&& abs(color.val[1] - g_piple.val[1]) < gaps.val[1]
		&& abs(color.val[2] - g_piple.val[2]) < gaps.val[2];
}
IplImage* ThresholdImg(IplImage *src, CvScalar g_piple, CvScalar gaps){
	IplImage *temp = cvCreateImage(cvGetSize(src), 8, 3);
	cvCopy(src, temp);
	for (int i = 0; i < temp->height; i++){
		for (int j = 0; j < temp->width; j++){
			CvScalar color = cvGet2D(temp, i, j);
			if (getInstance(color, g_piple, gaps)){
				cvSet2D(temp, i, j, cvScalarAll(0));
			}
			else{
				cvSet2D(temp, i, j, cvScalarAll(255));
			}
		}
	}
	return temp;
}

int is_equal(const void* _a, const void* _b, void* userdata)
{
	CvPoint a = (*(const Area*)_a).center;
	CvPoint b = (*(const Area*)_b).center;
	return abs(a.x - b.x) <= thresholdx && abs(a.y - b.y) <thresholdy;  
}

int main(){
	//CvScalar g_piple = CvScalar(62, 107, 87, 0);// //  // 146, 70, 80//53,100,82
	//CvScalar gaps = CvScalar(206, 24, 13);
	//IplImage *in = cvLoadImage("闸刀识别图片3.jpg");

	CvScalar g_piple = CvScalar(171, 180, 177, 0);// //  // 146, 70, 80//53,100,82
	CvScalar gaps = CvScalar(195, 94, 176);
	IplImage *dst = cvLoadImage("IMG_0493.JPG");
	IplImage*in = 0;
	int width = 1440 ;
	int height = 1080;
	in = cvCreateImage(CvSize(width, height), 8, 3);
	IplImage* temps = cvCreateImage(CvSize(width, height), 8,3);
	cvResize(dst, in, CV_INTER_LINEAR);

	//cvCvtColor(temps, in, CV_BGR2GRAY);
	cvNamedWindow("原图", CV_WINDOW_AUTOSIZE);
	cvShowImage("原图", in);

	//IplImage *in = cvLoadImage("闸刀识别图片7李伟拍 - 副本 (2).jpg");
	//
	//CvScalar g_piple = CvScalar(153, 51, 47, 0);// //  // 146, 70, 80//53,100,82
	//CvScalar gaps = CvScalar(126, 18, 145);
	//IplImage *in = cvLoadImage("闸刀识别图片5李伟拍.jpg");
	//
	//IplImage *in = cvLoadImage("C:\\Users\\孔龙飞\\Pictures\\opencv.jpg");

	cvSmooth(in, in, CV_GAUSSIAN, 5, 5);
	cvSmooth(in, in, CV_GAUSSIAN, 5, 5);
	cvSmooth(in, in, CV_GAUSSIAN, 5, 5);
	cvDilate(in, in);
	cvDilate(in, in);

	cvWaitKey(0);

	IplImage *des = cvCreateImage(cvGetSize(in), 8, 1);
	IplImage *des2 = cvCreateImage(cvGetSize(in), 8, 3);
	des2 = ThresholdImg(in, g_piple, gaps); //根据相似的颜色，二值化图片   有待完善
#pragma region 测试新的二值化的方法

	//IplConvKernel *kernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_ELLIPSE);

	////IplImage* gray_plane1 = cvCloneImage(in);
	//IplImage* temp = cvCreateImage(cvGetSize(des), 8, 1);
	//cvLaplace(des, des);
	//cvNamedWindow("cvErode", 1);
	//cvShowImage("cvErode", des);
	//cvWaitKey(0);
	//cvMorphologyEx(des, des, temp, kernel, CV_MOP_TOPHAT, 1);
	//cvNamedWindow("cvErode", 1);
	//cvShowImage("cvErode", des);
	//cvWaitKey(0);

	/*cvSub(in, des, in);
	cvSub(in, des, des);
	cvNamedWindow("cvErode2", 1);
	cvShowImage("cvErode2", des);*/

	//cvAdaptiveThreshold(des, des, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,81,15);  //不太可取
	//cvThreshold(des, des, 95, 255, CV_THRESH_BINARY);

	//cvXorS(des, cvScalarAll(0), des);

	///cvWaitKey(0);

#pragma endregion


	cvNamedWindow("灰度后的图", CV_WINDOW_AUTOSIZE);
	cvShowImage("灰度后的图", des2);

	cvSplit(des2, 0, des, 0, 0);
	cvNamedWindow("二值化后的图", CV_WINDOW_AUTOSIZE);
	cvShowImage("二值化后的图", des);
	//cvAdaptiveThreshold(s, des, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,71,15);  //不太可取
	cvWaitKey(0);

	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	int Nc = cvFindContours(
		des,   //在该单通道图片上查找轮廓
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_CCOMP   //CV_RETR_CCOMP  //不要内部的轮廓
		);
	printf("总共的轮廓数目 : %d\n", Nc);

	int nk = 0;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1);//初始化画笔
	IplImage *initContourImg = cvCreateImage(cvGetSize(in), 8, 3);
	CvSeq*  point_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(Area), cvCreateMemStorage(0));//保存中心点
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		double  area1 = cvContourArea(c);  //轮廓包围的面积
		//printf("area:%f\n", area1);
		if (area1>min_area && area1 <max_area){   //第一次筛选，面积在这个区间的保留       ==================
			cvDrawContours(initContourImg, c, CVX_RED, CVX_BLUE, 0, 1, 8);  //画出轮廓
			CvRect rect = cvBoundingRect(c);
			cvRectangleR(initContourImg, rect, cvScalarAll(255));  //提供的接口  //获得轮廓外接矩形
			CvPoint center = CvPoint((2 * rect.x + rect.width) / 2,
									 (2 * rect.y + rect.height) / 2);  //获得轮廓外接矩形中心点
			Area area = Area(center,rect.width,rect.height);
			cvSeqPush(point_seq, &area);  //将中心点保存在序列中

			char str[25];
			itoa(nk, str, 10);
			nk++;
			cvPutText(initContourImg, str, cvPoint(center.x, center.y), &font, CV_RGB(0, 255, 0));//在图片中输出字符 
			cvShowImage("初始勾勒轮廓", initContourImg);
			//cvWaitKey(10);
		}
	}
	cvWaitKey(0);

	IplImage *canvas = cvCreateImage(cvGetSize(in), 8, 3);
	cvNamedWindow("points", 1);

	CvSeq* labels = 0; //声明输出标签
	int i, class_count = cvSeqPartition(point_seq, 0, &labels, is_equal, 0); //聚类
	printf("\nclass_count: %d\n",class_count);
	cvWaitKey(0);

	CvSeq*  point__sort_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(Area), cvCreateMemStorage(0));//保存中心点
	int kk = 0;
	while (kk < class_count){
		int h = 0;
		int w = 0;
		int t_x = 0;
		int t_y = 0;
		int b_num = 0;
		for (int k = 0; k < labels->total; k++){
			int curLab = *(int*)cvGetSeqElem(labels, k); // g
			if (curLab == kk){  //第kk类
				Area curArea = (*(Area*)cvGetSeqElem(point_seq, k)); 
				h += curArea.height;  //总高度
				w =MAX(curArea.width,w);
				t_x += curArea.center.x;
				t_y += curArea.center.y;
				b_num++;
			}
		}
		//过滤
		if (h > max_len){  //太长了的话，不行
			kk++;
			continue;
		}
		if ((b_num == 1 && (h < state_len))){   //只有一个分类，并且高度小于70的都扔掉  
			kk++;
			continue;
		}
		//printf("h:%d,w:%d ",h,w);
		CvPoint newCenter = CvPoint(t_x / b_num, t_y/b_num);  //合并后的中心点
		Area bigArea = Area(newCenter, w,h); //合并为一个开关   
		cvSeqPushFront(point__sort_seq, &bigArea);  //推进去一个开关
		kk++;
	}

	printf("point_sort_seq_size:%d\n", point__sort_seq->total);  //多少个开关
	cvSeqSort(point__sort_seq, cmp_func);  //单个点根据中心点坐标排下序 从左到右->从上到下
	cvSeqPartition(point__sort_seq, 0, &labels, is_equal, 0);   //聚类 打乱了顺序，重新分下类， 重新分下类
	char str1[25];
	char str3[25];
	char str4[25];
	char *state;
	for (i = 0; i < point__sort_seq->total; i++)
	{
		state = "close";
		int curlab = *(int*)cvGetSeqElem(labels, i);
		CvPoint pt = (*(Area*)cvGetSeqElem(point__sort_seq, i)).center;
		int h = (*(Area*)cvGetSeqElem(point__sort_seq, i)).height;
		int w = (*(Area*)cvGetSeqElem(point__sort_seq, i)).width;
		if (h < state_len)  { //得到开关的状态
			state = "open";
		}
		//printf("第%d个%s\n", i,state);
		//printf("{%d,%d},", pt.x, pt.y);
		cvCircle(canvas, pt, 3, CvScalar(255), -1);
		itoa(curlab, str1, 10);
		itoa(h, str3, 10);
		itoa(w, str4, 10);
		
		cvPutText(canvas, str3, CvPoint(pt.x - 55, pt.y - 15), &font, CV_RGB(0, 255, 0));//高度
		cvPutText(canvas, str4, CvPoint(pt.x - 5, pt.y - 15), &font, CV_RGB(0, 255, 0));// 宽度
		cvPutText(canvas, str1, pt, &font, CV_RGB(0, 0, 255));//类别
		cvPutText(canvas, state, pt, &font, CV_RGB(0, 255, 255));//开关状态
		cvRectangleR(canvas, CvRect(pt.x - w / 2, pt.y - h / 2, w, h), cvScalarAll(255));
	    cvShowImage("points", canvas);
		cvWaitKey(300);
	}
	cvWaitKey(0);
}
