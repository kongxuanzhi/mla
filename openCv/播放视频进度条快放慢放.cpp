#include <iostream>
using namespace std;
#include <highgui.h>
#include <cv.h>
int g_slider_position = 0;
CvCapture* g_capture = NULL;  //得到帧句柄

void onTrackbarSlide(int pos){

	cvSetCaptureProperty(g_capture,
		CV_CAP_PROP_POS_FRAMES,
		pos);//设置一桢为单位，设置为第pos个帧的位置
	g_slider_position = pos;
}

int main(){
	
	cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture("开不了口.mp4");
	//获取视频有多少个帧
	int frames = (int)cvGetCaptureProperty(g_capture,CV_CAP_PROP_FRAME_COUNT);
	cout << frames << endl;
	if (frames != 0){
		//创建进度条
		cvCreateTrackbar(
			"Position", //进度条名称
			"Example3", //包含在那个窗口中
			&g_slider_position,  //当前位置
			frames,   //帧的个数
			onTrackbarSlide  //回调函数，设置视频读取到的进度
			);
	}
	IplImage* frame;
	
	while (1){
		frame = cvQueryFrame(g_capture);  
		if (!frame) break;
		cvShowImage("Example3",frame);
		char c = cvWaitKey(24);  //等待24ms，然后执行下面的语句
		if (c == 27)break;  //如果按esc键，退出

		g_slider_position++;  //帧加加
		cout << g_slider_position << endl;
		//重新设置进度条，跟随视频移动
		cvCreateTrackbar(
			"Position",
			"Example3",
			&g_slider_position,
			frames,
			onTrackbarSlide
			);
	}
	
	cvReleaseCapture(&g_capture);
	cvDestroyWindow("Example3");
	getchar();
}
