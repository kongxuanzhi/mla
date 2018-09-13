#include <highgui.h>
#include <cv.h>
#include <math.h>

CvScalar g_piple = CvScalar(153, 51, 47, 0);// //  // 146, 70, 80//53,100,82
IplImage *img = cvLoadImage("闸刀识别图片7李伟拍 - 副本 (2).jpg");

int R =126;  // 69
int G = 18;  //26
int B = 145;  //19

CvScalar gaps = CvScalar(R,G,B);

bool getInstance(const CvScalar color, CvScalar g_piple, CvScalar gaps){
	return abs(color.val[0] - g_piple.val[0]) < gaps.val[0]
		&& abs(color.val[1] - g_piple.val[1]) < gaps.val[1] 
		&& abs(color.val[2] - g_piple.val[2]) < gaps.val[2];
}
IplImage* showImg(CvScalar g_piple, CvScalar gaps){
	IplImage *temp = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img, temp);
	printf("%d %d %d\n", R, G, B);
	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){
			CvScalar color = cvGet2D(temp, i, j);
			if (getInstance(color, g_piple, gaps)){
				cvSet2D(temp, i, j, cvScalarAll(255));
			}
			else{
				cvSet2D(temp, i, j, cvScalarAll(0));
			}
		}
	}
	return temp;
}


void onTrackbarSlide1(int pos){
	R = pos;
   cvShowImage("img",showImg(g_piple,gaps));
}

void onTrackbarSlide2(int pos){
	G = pos;
	cvShowImage("img", showImg(g_piple, gaps));
}

void onTrackbarSlide3(int pos){
	B = pos;
	cvShowImage("img", showImg(g_piple, gaps));
}


int main(){
	cvNamedWindow("img", 1);
	cvCreateTrackbar(
		"R",
		"img",
		&R,
		255,
		onTrackbarSlide1
		);

	cvCreateTrackbar(
		"G",
		"img",
		&G,
		255,
		onTrackbarSlide2
		);
	cvCreateTrackbar(
		"B",
		"img",
		&B,
		255,
		onTrackbarSlide3
		);
	//while (true){
		
		//cvWaitKey(0);
	//}
	
	
	cvWaitKey(0);
}