#ifdef _CH_
#pragma package <opencv>
#endif
#include <iostream>
using namespace std;

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#endif

#define _DEBUG

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;

CvHistogram *hist = 0;
int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
CvRect selection;
CvRect track_window;
CvBox2D track_box;  // tracking 返回的区域 box，带角度
CvConnectedComp track_comp;
int hdims = 48;     // 划分HIST的个数，越高越精确 个bins
float hranges_arr[] = { 0, 180 };
float* hranges = hranges_arr;
int vmin = 10, vmax = 256, smin = 30;

void on_mouse(int event, int x, int y, int flags)
{
	if (!image)
		return;
	if (image->origin)// 从下到上 y坐标成镜像
		y = image->height - y;
	
	if (select_object)  //拖动的时候会一直执行这个地方if
	{
		//找到左上角
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
	
		//暂时标记右下角
		selection.width  = selection.x + CV_IABS(x - origin.x);
		selection.height = selection.y + CV_IABS(y - origin.y);
		
		//如果左上角小于0， 就取0
		selection.x = MAX(selection.x, 0); //左
		selection.y = MAX(selection.y, 0); //上
		//如果右下角的坐标超出了边框，就只取到右下角
		selection.width = MIN(selection.width, image->width); //右
		selection.height = MIN(selection.height, image->height);//下

		//最后，右下角减去左上角得到宽度和高度
		selection.width -= selection.x; 
		selection.height -= selection.y;
	}

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = cvPoint(x, y); 
		selection = cvRect(x, y, 0, 0);
		select_object = 1;
		break;
	case CV_EVENT_LBUTTONUP:
		select_object = 0;
		//标记选择的区域大于0个像素，这样就可以进行对选择区域进行分析
		if (selection.width > 0 && selection.height > 0)
			track_object = -1;
#ifndef _DEBUG
		printf("/n # 鼠标的选择区域：\n");
		printf("/n   X = %d, Y = %d, Width = %d, Height = %d",
			selection.x, selection.y, selection.width, selection.height);
		printf("\n");
#endif
		break;
	}
}


CvScalar hsv2rgb(float hue)
{
	int rgb[3], p, sector;
	static const int sector_data[][3] =
	{ { 0, 2, 1 }, { 1, 2, 0 }, { 1, 0, 2 }, { 2, 0, 1 }, { 2, 1, 0 }, { 0, 1, 2 } };
	hue *= 0.033333333333333333333333333333333f;   //  1/30
	sector = cvFloor(hue);
	p = cvRound(255 * (hue - sector));
	p ^= sector & 1 ? 255 : 0;

	rgb[sector_data[sector][0]] = 255;
	rgb[sector_data[sector][1]] = 0;
	rgb[sector_data[sector][2]] = p;

#ifndef _DEBUG
	printf("/n # Convert HSV to RGB：");
	printf("/n   HUE = %f", hue);
	printf("/n   R = %d, G = %d, B = %d", rgb[0], rgb[1], rgb[2]);
#endif

	return cvScalar(rgb[2], rgb[1], rgb[0], 0);
}

int main(int argc, char** argv)
{
	CvCapture* capture = 0;
	IplImage* frame = 0;

	//if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
	//	capture = cvCreateCameraCapture(-1);
	//	//capture = cvCaptureFromCAM(argc == 2 ? argv[1][0] - '0' : 0);
	//else if (argc == 2)
	  capture = cvCaptureFromAVI("开不了口.mp4");

	if (!capture)
	{
		fprintf(stderr, "Could not initialize capturing.../n");
		getchar();
		return -1;
	}

	printf("Hot keys: /n"
		"/tESC - quit the program/n"
		"/tc - stop the tracking/n"
		"/tb - switch to/from backprojection view/n"
		"/th - show/hide object histogram/n"
		"To initialize tracking, select the object with mouse/n");

	cvNamedWindow( "Histogram", 1 );  //打开窗口
	cvNamedWindow("CamShiftDemo", 1);
	//cvNamedWindow("a", 1);
	cvSetMouseCallback("CamShiftDemo", (CvMouseCallback)on_mouse, NULL); // on_mouse 自定义事件
	
	cvCreateTrackbar("Vmin", "CamShiftDemo", &vmin, 256, 0);
	cvCreateTrackbar("Vmax5", "CamShiftDemo", &vmax, 256, 0);
	cvCreateTrackbar("Smin", "CamShiftDemo", &smin, 256, 0);

	for (;;)
	{
		int i, bin_w, c;
		frame = cvQueryFrame(capture);
		if (!frame)
			break;

		if (!image)
		{
			/* allocate all the buffers */
			image = cvCreateImage(cvGetSize(frame), 8, 3);
			image->origin = frame->origin;  //1 上下颠倒
			hsv = cvCreateImage(cvGetSize(frame), 8, 3);

			hue = cvCreateImage(cvGetSize(frame), 8, 1);
			mask = cvCreateImage(cvGetSize(frame), 8, 1);
			backproject = cvCreateImage(cvGetSize(frame), 8, 1);

			//分成hdims 个bin，每个bin的取值范围都在0-255. 然后 横的区域hranges 0-180
			hist = cvCreateHist(1, &hdims, CV_HIST_ARRAY, &hranges, 1);  // 创建直方图
			histimg = cvCreateImage(cvSize(320, 200), 8, 3);
			cvZero(histimg);
		}

		cvCopy(frame, image, 0);
		cvCvtColor(image, hsv, CV_BGR2HSV);  //彩色空间转换 BGR to HSV 

		if (track_object)  //-1和1的时候执行   //开始追踪
		{
			int _vmin = vmin, _vmax = vmax;

			cvInRangeS(hsv, cvScalar(0,  smin, MIN(_vmin, _vmax), 0),
			                cvScalar(180, 256, MAX(_vmin, _vmax), 0), mask);  // 得到二值的MASK  0和1 0代表不在范围内，1代表在范围内

			//hsv中包含色彩， 饱和度和亮度 ，hue是色彩，也就是只提取色彩
			cvSplit(hsv, hue, 0, 0, 0);  //只提取 HUE 分量  颜色的色调
			
			//IplImage* b = cvCreateImage(cvGetSize(frame), 8, 1);
			//cvSplit(hsv, b, 0, 0, 0);  //只提取 HUE 分量  颜色的色调
			//cvShowImage("a", hue);
			

			if (track_object < 0)  //选择了区域
			{
				float max_val = 0.f;
				cvSetImageROI(hue, selection);  // 得到选择区域 for ROI
				cvSetImageROI(mask, selection); // 得到选择区域 for mask

				cvCalcHist(&hue, hist, 0, mask); //计算直方图

				cvGetMinMaxHistValue(hist, 0, &max_val, 0, 0);  //只找最大值
				cout << "max_val:" << max_val << endl;
				//for (i = 0; i < hdims; i++)    //那么多个竖条
				//{
				//	cout << cvGetReal1D(hist->bins, i)<<" ";
				//}
				//cout << endl;
				cvConvertScale(hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0); // 缩放 bin 到区间 [0,255] 
				cvResetImageROI(hue);  // remove ROI
				cvResetImageROI(mask);

				track_window = selection;  //记录选择区域，以便持续跟踪

				track_object = 1; //取消选择
				cvZero(histimg);  //清空屏幕，重新绘制

				bin_w = histimg->width / hdims;  // hdims: 条的个数，则 bin_w 为条的宽度  
   
				// 画直方图
				for (i = 0; i < hdims; i++)    //那么多个竖条
				{
					//cout << cvGetReal1D(hist->bins, i) << endl;;
					int val = cvRound(cvGetReal1D(hist->bins, i)*histimg->height / 255);// 再次缩放到图片容纳的范围
					
					CvScalar color = hsv2rgb(i*180.f / hdims);  //角度  //生成不同的颜色柱子
					cvRectangle(histimg,
						        cvPoint(i*bin_w, histimg->height),  //柱子左下角
						        cvPoint((i + 1)*bin_w,     histimg->height - val),  //柱子右上角
						        color, 
								-1, 
								8, 
								0);
				}
			}

			//利用反向投影跟踪  
			//反向投影：是一种记录像素点(cvCalcBackProject())或像素块(cvCalcBackProjectPatch())
			//如何适应直方图模型中分布的方式。
			//如：有一个颜色直方图，可以利用反向投影在图像中找到该区域。
			cvCalcBackProject(&hue, backproject, hist);  // 使用 back project 方法     跟hist得到的结果进行比较
			cvAnd(backproject, mask, backproject, 0);  //指定在mask区域内跟踪

		//	cout << track_window.x << "\t" << track_window.y << endl;

			// calling CAMSHIFT 算法模块   ，返回一个椭圆  //整个算法的核心
			cvCamShift(backproject, track_window,
				cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),
				&track_comp, &track_box);
			//记录新的区域
			track_window = track_comp.rect;   //物体移动到新的区域
			cvRectangleR(image, track_window,CvScalar(255),-1,8,0);

			// 没太明白
			if (backproject_mode)
				cvCvtColor(backproject, image, CV_GRAY2BGR); // 使用backproject灰度图像

			if (image->origin)  //角度上下镜像
				track_box.angle = -track_box.angle;

			//在image上画一个绿色的 椭圆
			cvEllipseBox(image, track_box, CV_RGB(0, 255, 0), 13, CV_AA, 0);
		}

		//Display  在鼠标未抬起之前
		if (select_object && selection.width > 0 && selection.height > 0)
		{
			cvSetImageROI(image, selection);
			cvXorS(image, cvScalarAll(255), image, 0); //异或选中区域的像素，突出显示  cvXorS 在数组和标量之间进行按位异或操作
			cvResetImageROI(image);
		}

		cvShowImage("CamShiftDemo", image);
		cvShowImage("Histogram", histimg);

		c = cvWaitKey(24);
		if (c == 27)
			break;  // exit from for-loop
		switch (c)
		{
		case 'b':
			backproject_mode ^= 1;
			break;
		case 'c':
			track_object = 0;
			cvZero(histimg);
			break;
		case 'h':
			show_hist ^= 1;
			if (!show_hist)
				cvDestroyWindow("Histogram");
			else
				cvNamedWindow("Histogram", 1);
			break;
		default:
			;
		}
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("CamShiftDemo");

	return 0;
}

#ifdef _EiC
main(1, "camshiftdemo.c");
#endif


