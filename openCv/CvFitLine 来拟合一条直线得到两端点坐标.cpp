/*
	输入点序列，使用cvFitLine拟合出一条直线，求出两端点
*/
CvPoint* GetLine(vector<CvPoint> points,IplImage *src){
	CvPoint* points2 = (CvPoint*)malloc(points.size() * sizeof(points[0]));
	for (int i = 0; i < points.size(); i++){
		CvPoint p = points[i];
		points2[i] = p;
	}
	CvMat pointMat = cvMat(1, points.size(), CV_32SC2, points2); //点集, 存储count个随机点points  
	float line[4];
	cvFitLine(&pointMat, CV_DIST_L1, 1, 0.001, 0.001, line);

	printf("line4:%f,%f,%f,%f\n", line[0], line[1], line[2], line[3]);
	CvPoint pt1, pt2; //直线的两个端点  

	CvPoint* pts = (CvPoint*)malloc(2 * sizeof(points[0]));
	float d, t;
	d = sqrt((double)line[0] * line[0] + (double)line[1] * line[1]);  //line[0 & 1]存储的是单位向量，所以d=1  
	//printf("\n %f\n", d);  
	line[0] /= d;
	line[1] /= d;
	//画出线段的两个端点(避免线太短，以线上一个随机点向两侧延伸line[0]*t )  
	t = (float)(src->width / 2 + src->height / 2);
	pts[0].x = cvRound(line[2] - line[0] * t);
	pts[0].y = cvRound(line[3] - line[1] * t);
	pts[1].x = cvRound(line[2] + line[0] * t);
	pts[1].y = cvRound(line[3] + line[1] * t);
	cvLine(src, pts[0], pts[1], CV_RGB(0, 255, 0), 3, CV_AA, 0);
	return pts;
}