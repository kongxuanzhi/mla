#include <highgui.h>
#include <cv.h>
#include <math.h>
IplImage* doPyrDown(IplImage *in, int filter = CV_GAUSSIAN_5x5){
	IplImage* out = cvCreateImage(
		cvSize(800, 600), //定义输出图片的大小为一半
		in->depth,  //每个像素的存储格式
		in->nChannels  //通道数
		);
	cvPyrDown(in, out, filter);   //剪裁图片 同时高斯模糊
	return out;
}

IplImage *doCanny(IplImage *in,
	double lowThresh,
	double highThresh,
	double aperture){
	if (in->nChannels == 1){
		return 0; //Canny only handles gray scale images
	}

	IplImage *out = cvCreateImage(
		cvSize(cvGetSize(in).width, cvGetSize(in).height),
		IPL_DEPTH_8U,
		1
		);
	//边缘检测
	cvCanny(in, out, lowThresh, highThresh, aperture);
	return out;
}

void Iterator(IplImage *img){
	for (int y = 0; y < img->height; y++){
		uchar* ptr = (uchar*)(img->imageData + y*img->widthStep);
		for (int x = 0; x < img->width; x++){
			printf("(%d,%d,%d),", ptr[3 * x], ptr[3 * x + 1], ptr[3 * x + 2]);
		}
	}
}
//在二值化的图片上查找轮廓
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
#define CVX_RED CV_RGB(0xff, 0x00, 0x00)  
#define CVX_GREEN CV_RGB(0x00, 0xff, 0x00)  
#define CVX_BLUE CV_RGB(0x00, 0x00, 0xff)  
int main(){//斜：499，504，505 510 511 513
	IplImage *in = cvLoadImage("IMG_0500.JPG");   // 513  509
	//IplImage *in = cvLoadImage("C:\\Users\\孔龙飞\\Pictures\\opencv.jpg");
	//cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Example1", in);

	//将图像缩小一半
	IplImage *out = cvCreateImage(cvSize(1008, 756), 8, 3);
		//doPyrDown(in, CV_GAUSSIAN_5x5);
	//out = doPyrDown(out, CV_GAUSSIAN_5x5);//再缩小一半
	cvResize(in, out);

	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);
	cvSmooth(out, out, CV_GAUSSIAN);

	/*cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example2", out);*/
#if 1  //得到边缘除去边缘
	IplImage *srcMini = cvCreateImage(cvGetSize(out), 8, 3);
	cvCopy(out, srcMini);

	IplImage *hsv = cvCreateImage(cvGetSize(srcMini), 8, 3);
	cvCvtColor(srcMini, hsv, CV_BGR2HSV);
	IplImage* s = cvCreateImage(cvGetSize(hsv), 8, 1);  //0-255
	IplImage* v = cvCreateImage(cvGetSize(hsv), 8, 1);  //0-255
	cvSplit(hsv, 0, s, v, 0);
	for (int y = 0; y < s->height; y++){
		uchar* ptrs = (uchar*)(s->imageData + y*s->widthStep);
		uchar* ptrv = (uchar*)(v->imageData + y*v->widthStep);
		for (int x = 0; x < s->width; x++){
			/*int value = abs(ptrs[x] - 0)*abs(ptrs[x] - 0) + abs(ptrv[x] - 255)*abs(ptrv[x] - 255);
			printf("%",value);*/
			//ptrs[x] = ptrv[x]<120 ? 255 : 0;
			if (ptrs[x]<90){ //ptrv[x]<120
				//ptrs[x] = ptrv[x] < 120 ? 255 : 0;
				ptrs[x] =  0;
				//cvDrawCircle(srcMini, CvPoint(x, y), 1, CvScalar(0, 0, 0));
			}
			else{
				ptrs[x] = 255;
			}
		}
	}
	//Iterator(s);
	cvShowImage("Example21", s);
	cvWaitKey(0);
	cvShowImage("Example22", srcMini);
	cvWaitKey(0);
#endif
	//进行边缘检测
	out = doCanny(out, 45, 90, 3);
	//Iterator(out);
	cvNamedWindow("Example30", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example30", out);
	cvWaitKey(0);



	for (int i = 0; i <2; i++){
		cvWaitKey(0);
		cvDilate(out, out); 
		cvDilate(s, s);
		cvNamedWindow("Example31", CV_WINDOW_AUTOSIZE);
		cvShowImage("Example31", out);
	}
	cvShowImage("Example21", s);
	cvWaitKey(0);
	IplConvKernel *element = cvCreateStructuringElementEx(5,5, 0, 0, CV_SHAPE_RECT, NULL);
	cvDilate(s, s);
	cvDilate(s, s );

	cvShowImage("Example21", s);
	cvWaitKey(0);
	cvSub(out, s, out); 
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);
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
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);
	cvErode(out, out, element);
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);
	cvDilate(out, out);
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);
	cvDilate(out, out);
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);
	cvDilate(out, out,element);
	cvNamedWindow("Example32", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example32", out);
	cvWaitKey(0);


	cvNamedWindow("Example33", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example33", out);
#if 1
	CvSeq* first_contour = FindCountour(out);

	int nk = 0;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1);//初始化画笔
	IplImage *initContourImg1 = cvCreateImage(cvGetSize(in), 8, 3);
	float width = 0.0;
	float height = 0.0;
	float area = 0.0;
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		cvDrawContours(initContourImg1, c, CVX_RED, CVX_BLUE, 0, 1, 8);  //画出轮廓 
		CvRect rect = cvBoundingRect(c);
		cvRectangleR(initContourImg1, rect, cvScalarAll(255));  //画出外接矩形
		CvPoint center = CvPoint((2 * rect.x + rect.width) / 2,
			(2 * rect.y + rect.height) / 2);  //获得轮廓外接矩形中心点
		width += (float)rect.width;
		height += (float)rect.height;
		/*area += cv*/
		char str[25];
		itoa(nk, str, 10);
		nk++;
		cvPutText(initContourImg1, str, cvPoint(center.x, center.y), &font, CV_RGB(0, 255, 0));//在图片中输出字符 

	}
	cvShowImage("初始勾勒轮廓", initContourImg1);

	width = (float)width / nk;
	height = (float)height / nk;

	printf("width:%f\n", height);
	IplImage *initContourImg = cvCreateImage(cvGetSize(out), 8, 3);
	for (CvSeq* c = first_contour; c != NULL; c = c->h_next){
		CvRect rect = cvBoundingRect(c);
		if (rect.width >= width*5.0 / 6.0 && rect.width<width*1.8 && rect.height >= height*3.0 / 4.0){
			cvRectangleR(initContourImg, rect, cvScalarAll(255));  //画出外接矩形
			CvPoint center = CvPoint((2 * rect.x + rect.width) / 2,
				(2 * rect.y + rect.height) / 2);  //获得轮廓外接矩形中心点
			char str[25];
			itoa(nk, str, 10);
			nk++;
			cvPutText(initContourImg, str, cvPoint(center.x, center.y), &font, CV_RGB(0, 255, 0));//在图片中输出字符 

			CvBox2D ellipse = cvFitEllipse2(c);//最小二乘法的椭圆拟合    
			cvEllipseBox(initContourImg, ellipse, CV_RGB(255, 0, 0)); //在图上画椭圆   
			cvEllipseBox(initContourImg1, ellipse, CV_RGB(255, 0, 0)); //在图上画椭圆   

			CvBox2D rect = cvMinAreaRect2(c);
			//printf("%f, ", rect.angle);
			printf("%f, ", ellipse.angle/180.0 );

			CvPoint2D32f rect_pts0[4];
			cvBoxPoints(rect, rect_pts0); //四个顶点坐标
			int npts = 4;
			CvPoint rect_pts[4], *pt = rect_pts;
			for (int rp = 0; rp<4; rp++)
				rect_pts[rp] = cvPointFrom32f(rect_pts0[rp]);
			//for (int i = 0; i < 4; i++){
			//	printf("x:%d, y:%d\n", rect_pts[i].x, rect_pts[i].y);
			//	int dis = (rect_pts[0].x - rect_pts[i].x)*(rect_pts[0].x - rect_pts[i].x) + (rect_pts[0].y - rect_pts[i].y)*(rect_pts[0].y - rect_pts[i].y);
			////	printf("maxdis:%d\n", dis);
			//	if (dis>maxdis){
			//		maxdis = dis;
			//		longest = i;
			//	}
			//	cvDrawCircle(initContourImg, CvPoint(rect_pts[i].x, rect_pts[i].y), 5, CvScalar(0, 255, 0));
			//	cvWaitKey(200);
			//}
			double x1 = (double)rect_pts[1].x, y1 = (double)rect_pts[1].y;
			double x2 = (double)rect_pts[0].x, y2 = (double)rect_pts[0].y;
			double arc = atan((x2-x1)/(y2-y1));
		//	printf("%f ", arc + ellipse.angle / 180.0 );
			if (arc < 0.1){
				cvPolyLine(initContourImg, &pt, &npts, 1, 1, CV_RGB(255, 0, 0), 2);
			}
			else{
				cvPolyLine(initContourImg, &pt, &npts, 1, 1, CV_RGB(0, 255, 0), 2);
			}
			cvShowImage("初始勾勒轮廓2", initContourImg);
		  // cvWaitKey(0);
		}
	}

	cvShowImage("初始勾勒轮廓", initContourImg1);
	cvWaitKey(0);

#endif




	cvWaitKey(0);

	cvReleaseImage(&out);
}
