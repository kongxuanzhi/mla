#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;

Mat g_srcImage, g_dstImage;
int g_nElementShape = MORPH_RECT;

int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;

static void on_OpenClose(int,void*);
static void on_ErodeDilate(int, void*);
static void on_TopBlackHat(int, void*);

int main(){
	g_srcImage = imread("闸刀识别图片7李伟拍.jpg");
	namedWindow("[原始图]");
	imshow("[原始图]", g_srcImage);

	namedWindow("[开运算/闭运算]",1);
	namedWindow("[腐蚀/膨胀]",1);
	namedWindow("[顶帽/黑帽]",1);

	g_nOpenCloseNum = 9;
	g_nErodeDilateNum = 9;
	g_nTopBlackHatNum = 2;

	//滚动条
	createTrackbar("迭代值", "[开运算/闭运算]", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, on_OpenClose);
	createTrackbar("迭代值", "[腐蚀/膨胀]", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, on_ErodeDilate);
	createTrackbar("迭代值", "[顶帽/黑帽]", &g_nTopBlackHatNum, g_nMaxIterationNum * 2 + 1, on_TopBlackHat);

	while (1){
		int c;
		
		on_OpenClose(g_nOpenCloseNum, 0);
		on_ErodeDilate(g_nErodeDilateNum, 0);
		on_TopBlackHat(g_nTopBlackHatNum, 0);

		c = waitKey(0);
		char key = (char)c;
		if (key == 'q' || key == 27){
			break;
		}
		if (key == 49){
			g_nElementShape = MORPH_ELLIPSE;
		}
		else if (key == 50){
			g_nElementShape = MORPH_RECT;
		}
		else if (key == 51){
			g_nElementShape = MORPH_CROSS;
		}
		else if (key == ' '){
			g_nElementShape = (g_nElementShape + 1) % 3;
		}
	}
	waitKey(0);
	waitKey(0);
	return 0;
}

static void on_OpenClose(int, void*){
	//偏移量的定义
	int offset = g_nOpenCloseNum - g_nMaxIterationNum;
	int Absolute_offset = offset > 0 ? offset : -offset;
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset,Absolute_offset));
	if (offset < 0){
		morphologyEx(g_srcImage,g_dstImage, MORPH_OPEN, element);
	}
	else{
		morphologyEx(g_srcImage, g_dstImage, MORPH_CLOSE, element);
	}
	imshow("[开运算/闭运算]",g_dstImage);
}

static void on_ErodeDilate(int, void*){
	//偏移量的定义
	int offset = g_nErodeDilateNum - g_nMaxIterationNum;
	int Absolute_offset = offset > 0 ? offset : -offset;
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));
	if (offset < 0){
		erode(g_srcImage, g_dstImage, element);
	}
	else{
		dilate(g_srcImage, g_dstImage, element);
	}
	imshow("[腐蚀/膨胀]", g_dstImage);
}
static void on_TopBlackHat(int, void*){
	//偏移量的定义
	int offset = g_nTopBlackHatNum - g_nMaxIterationNum;
	int Absolute_offset = offset > 0 ? offset : -offset;
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));
	if (offset < 0){
		morphologyEx(g_srcImage, g_dstImage, MORPH_TOPHAT, element);
	}
	else{
		morphologyEx(g_srcImage, g_dstImage, MORPH_BLACKHAT, element);
	}
	imshow("[顶帽/黑帽]", g_dstImage);
}