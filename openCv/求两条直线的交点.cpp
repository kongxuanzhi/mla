/*
已知两条直线， 四个端点，得到他们的焦点，排除平行的情况
//利用三点连成一条直线，然后联立两个方程，解得x和y
*/
CvPoint getTouchP(CvPoint* lp, CvPoint* tp){
	double x0 = lp[0].x, y0 = lp[0].y;
	double x1 = lp[1].x, y1 = lp[1].y;
	double x2 = tp[0].x, y2 = tp[0].y;
	double x3 = tp[1].x, y3 = tp[1].y;
	printf("%f \t %f\n", x0, y0);
	printf("%f \t %f\n", x1, y1);
	printf("%f \t %f\n", x2, y2);
	printf("%f \t %f\n", x3, y3);

	double k1 = (y0 - y1) / (x0 - x1);
	double k2 = (y2 - y3) / (x2 - x3);
	printf("%f \t %f\n", k1, k2);

	double x = k1 == k2 ? 0: (k1*x0 - k2*x2 + y2 - y0) / (k1 - k2);
	double y = y0 + (x - x0)*k1;
	printf("%f %f",  x,y);
	return CvPoint(x, y);
}
