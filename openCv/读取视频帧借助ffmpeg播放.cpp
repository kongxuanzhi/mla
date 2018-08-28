#include <iostream>
using namespace std;
#include <highgui.h>
	
//要运行这段代码，需要将将 
//opencv/bin/opencv_ffmpeg310.dll 拷到c:\\windows\system32\ 下面就好了
int main(){
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	CvCapture *capture = cvCreateFileCapture("开不了口.mp4");
	IplImage *frame=NULL;
	while (1){
		frame = cvQueryFrame(capture);
		if (!frame) break;
		cvShowImage("Example2",frame);
		char c = cvWaitKey(24);
		if (c == 27)break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Example2");
	getchar();
}
