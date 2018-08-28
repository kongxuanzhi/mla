#include <highgui.h>
#include <cv.h>

IplImage* doPyrDown(IplImage *in, int filter = CV_GAUSSIAN_5x5){
	//assert((in->width)/2 == 0 && (in->height)/2 == 0); //这一句必须要注释掉
	IplImage* out = cvCreateImage(
		cvSize(in->width/2, in->height/2), //定义输出图片的大小为一半
		in->depth,  //每个像素的存储格式
		in->nChannels  //通道数
		);
	cvPyrDown(in, out,filter);   //剪裁图片
	return out;
}


int main(){
	IplImage *in = cvLoadImage("th.jpg");
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", in);
	IplImage *out = doPyrDown(in, CV_GAUSSIAN_5x5);
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example2", out);
	cvWaitKey(0);
}

