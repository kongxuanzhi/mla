#include <highgui.h>
#include <cv.h>

#include <iostream>
using namespace std;

//读取一个视频转化为灰度播放，但MJPG编码不存在
int main(){
	CvCapture * capture = 0;
	capture = cvCreateFileCapture("开不了口.mp4");
	if (!capture) return -1;
	IplImage *bgr_frame = cvQueryFrame(capture); //init the video read
	
	double fps = cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
	CvSize size = cvSize(
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
	);
	CvVideoWriter *writer = cvCreateVideoWriter(
		"开得了口.mp4",  //输出设备文件名
		CV_FOURCC('M','J','P','G'),  //编码器
		fps,  //帧率
		size  //大小
	);
	IplImage* logpolar_frame = cvCreateImage(
		size, IPL_DEPTH_8U,
		3
	);
	while ((bgr_frame=cvQueryFrame(capture))!= NULL){
		cvLogPolar(bgr_frame, logpolar_frame,
			cvPoint2D32f(bgr_frame->width/2,
				bgr_frame->height/2
				),
				40,
				CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS
			);
		cvWriteFrame(writer,logpolar_frame); //将一桢数据写入到文件 
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&logpolar_frame);
	cvReleaseCapture(&capture);
	return 0;
}
