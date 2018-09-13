#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int main(){
	Mat src  = imread("Lena.jpg");
	Mat src1 = src.clone();

	imshow("[原始图，边缘检测]",src);

	Mat dst, edge, gray;
	dst.create(src1.size(), src1.type());

	cvtColor(src1, gray, COLOR_BGR2GRAY);  

	blur(gray, edge, Size(3,3));  //滤波模糊

	Canny(edge, edge, 3, 9, 3);  //边缘检测

	dst = Scalar::all(0);

	src1.copyTo(dst, edge);//将src1的edge边缘部分拷贝到dst，dst中带颜色了
	imshow("[效果图] canny 边缘检测1", edge);

	imshow("[效果图] canny 边缘检测2", dst);

	waitKey(0);
	return 0;
}