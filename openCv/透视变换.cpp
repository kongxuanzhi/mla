#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;

int main()
{
		
#if 0 //透视变换 
	
	CvPoint2D32f srcTri[4], dstTri[4];
	CvMat*       warp_mat = cvCreateMat(3, 3, CV_32FC1);
	IplImage*    in = NULL;
	IplImage*    dst = NULL;

	in = cvLoadImage("use/r_1.jpg", 1);
	IplImage *src = cvCreateImage(cvSize(1008, 756), 8, 3);
	cvResize(in, src);

	dst = cvCloneImage(src);
	dst->origin = src->origin;
	cvZero(dst);

	srcTri[0].x = 761;
	srcTri[0].y = 49;
	srcTri[1].x = 120;
	srcTri[1].y = 13;
	srcTri[2].x = 151;
	srcTri[2].y = 733;
	srcTri[3].x = 725;
	srcTri[3].y = 695;

	dstTri[0].x = dst->width - 1;
	dstTri[0].y = 0;
	dstTri[1].x = 0;
	dstTri[1].y = 0;
	dstTri[2].x = 0;
	dstTri[2].y = dst->height - 1;
	dstTri[3].x = dst->width - 1;
	dstTri[3].y = dst->height - 1;

	cvGetPerspectiveTransform(srcTri, dstTri, warp_mat);
	cvWarpPerspective(src, dst, warp_mat);

	cvNamedWindow("src", 1);
	cvShowImage("src", src);
	cvNamedWindow("Affine_Transform", 1);
	cvShowImage("Affine_Transform", dst);

	cvWaitKey(0);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseMat(&warp_mat);
#endif

	return 0;
}
