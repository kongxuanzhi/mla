#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;

int main()
{
		
#if 0 //仿射变换 
	CvPoint2D32f srcTri[3], dstTri[3];
	CvMat*       rot_mat = cvCreateMat(2, 3, CV_32FC1);
	CvMat*       warp_mat = cvCreateMat(2, 3, CV_32FC1);
	IplImage*    in = NULL;
	IplImage*    dst = NULL;
	in = cvLoadImage("use/r_1.jpg", 1);
	IplImage *src = cvCreateImage(cvSize(1008, 756), 8, 3);
	cvResize(in, src);

	cvNamedWindow("src", 1);
	cvShowImage("src", src);
	dst = cvCloneImage(src);
	cvZero(dst);
	srcTri[0].x = 761;
	srcTri[0].y = 49;
	srcTri[1].x = 120;
	srcTri[1].y = 13;
	srcTri[2].x = 151;
	srcTri[2].y = 733;

	dstTri[0].x = dst->width - 1;
	dstTri[0].y = 0;
	dstTri[1].x = 0;
	dstTri[1].y = 0;
	dstTri[2].x = 0;
	dstTri[2].y = dst->height - 1;


	cvDrawCircle(src, CvPoint(dstTri[0].x, dstTri[0].y), 5, CvScalar(0, 255, 255), 2);
	cvDrawCircle(src, CvPoint(dstTri[1].x, dstTri[1].y), 5, CvScalar(0, 255, 255), 2);
	cvDrawCircle(src, CvPoint(dstTri[2].x, dstTri[2].y), 5, CvScalar(0, 255, 255), 2);

	cvDrawCircle(src, CvPoint(srcTri[0].x, srcTri[0].y), 5, CvScalar(255, 0, 255), 2);
	cvDrawCircle(src, CvPoint(srcTri[1].x, srcTri[1].y), 5, CvScalar(255, 0, 255), 2);
	cvDrawCircle(src, CvPoint(srcTri[2].x, srcTri[2].y), 5, CvScalar(255,0 , 255), 2);

	cvGetAffineTransform(srcTri, dstTri, warp_mat);
	cvWarpAffine(src, dst, warp_mat);
	//cvCopy(dst, src);

	cvShowImage("dst", dst);
	cvShowImage("src", src);

	cvWaitKey(0);

	srcTri[0].x = dst->width - 1;
	srcTri[0].y = 0;
	srcTri[1].x = 907;
	srcTri[1].y = 678;
	srcTri[2].x = 0;
	srcTri[2].y = dst->height - 1;

	dstTri[0].x = dst->width - 1;
	dstTri[0].y = 0;
	dstTri[1].x = dst->width - 1;
	dstTri[1].y = dst->height - 1;
	dstTri[2].x = 0;
	dstTri[2].y = dst->height - 1;

	cvGetAffineTransform(srcTri, dstTri, warp_mat);
	cvWarpAffine(dst, src, warp_mat);

	cvShowImage("src", dst);
	cvShowImage("dst", src);

	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseMat(&warp_mat);
	cvReleaseMat(&rot_mat);
#endif

	return 0;
}
