#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#define cvGetHistValue_1D( hist, idx0 )     ((float*)(cvPtr1D( (hist)->bins, (idx0), 0 ))) 
#define cvGetHistValue_2D( hist, idx0, idx1 )  ((float*)(cvPtr2D( (hist)->bins, (idx0), (idx1), 0 ))) 
#define cvGetHistValue_3D( hist, idx0, idx1, idx2 )   ((float*)(cvPtr3D( (hist)->bins, (idx0), (idx1), (idx2), 0 ))) 
#define cvGetHistValue_nD( hist, idx )      ((float*)(cvPtrND( (hist)->bins, (idx), 0 )))

int main()
{
	int hist_size[] = { 16, 16, 16 };
	float range[] = { 0, 255 };
	float *ranges[] = { range };
	int i, j, bin_w;
	float max_value, min_value;
	int min_idx, max_idx;
	char *name[3] = { "b", "g", "r" };
	double mean = 0, variance = 0;
	IplImage* img = cvLoadImage("big.jpg", 1);
	IplImage *pImage = NULL;
	IplImage *pImg[3];
	pImg[0] = cvCreateImage(cvGetSize(img), 8, 1);
	pImg[1] = cvCreateImage(cvGetSize(img), 8, 1);
	pImg[2] = cvCreateImage(cvGetSize(img), 8, 1);
	cvSplit(img, pImg[0], pImg[1], pImg[2], NULL);
	pImage = cvCloneImage(img);

	CvRect rect = cvRect(0, 0, 500, 600);
	cvSetImageROI(pImage, rect);

	//创建一个图像用来存放直方图
	IplImage *histImage[3];
	CvHistogram *hist[3];
	for (j = 0; j<3; j++){
		histImage[j] = cvCreateImage(cvSize(320, 200), 8, 3);
		//cvZero(histImage[j]);
		hist[j] = cvCreateHist(1, &hist_size[j], CV_HIST_ARRAY, ranges, 1); //申请空间到bins
		//计算直方图并作用到hist变量中
		cvCalcHist(&pImg[j], hist[j], 0, NULL); //填充bins
		//得到直方图的最值及标号
		cvGetMinMaxHistValue(hist[j], &min_value, &max_value, &min_idx, &max_idx);  
		printf("maxv:%f,minv:%f\n", max_value, min_value);
		printf("minidL:%d,maxid:%d\n", min_idx, max_idx);


		//缩放其最大值和最小值让其融入图像
		cvScale(hist[j]->bins, hist[j]->bins, ((double)histImage[j]->height) / max_value, 0);  //缩放高度到图片的高度
		//设置所有的直方图的数值为255  //背景色
		cvSet(histImage[j], cvScalarAll(100), 0);
		//建一个比例因子  沿宽度释放
		bin_w = cvRound((double)histImage[j]->width / hist_size[j]);  //等比
		mean = 0;
		for (i = 0; i<hist_size[j]; i++)
		{
			CvScalar scalar = cvScalarAll(0);
			scalar.val[j] = (i * 255 / hist_size[j]);
			cvRectangle(histImage[j], cvPoint(i*bin_w, histImage[j]->height),
				cvPoint((i + 1)*bin_w, histImage[j]->height - cvRound(cvGetReal1D(hist[j]->bins, i))), scalar, -1, 8, 0);

			float *bins = cvGetHistValue_1D(hist[j], i);
			//增加均值
			mean += bins[0];
			//std::cout<<bins[0]<<" "<<bins[1]<<std::endl;
			//printf("%d %d\n",bins[0],bins[1]);
		}
		cvNamedWindow(name[j], 0);
		cvShowImage(name[j], histImage[j]);

		mean /= hist_size[j];
		//根据均值计算变化量
		for (i = 0; i<hist_size[j]; i++)
		{
			float* bins = cvGetHistValue_1D(hist[j], i);
			variance += pow((bins[0] - mean), 2);
		}

		variance /= hist_size[j];
		printf("histgram Mean:%f\n", mean);

	}

	//创建窗口
	cvNamedWindow("Original", 0);
	cvShowImage("Original", pImage);
	cvWaitKey(0);
	cvReleaseImage(&img);
	for (j = 0; j<3; j++){
		cvDestroyWindow(name[j]);
		cvReleaseImage(&pImg[j]);
		cvReleaseImage(&histImage[j]);
		cvReleaseHist(&hist[j]);
	}
	cvDestroyWindow("Original");
	return 0;
}






















