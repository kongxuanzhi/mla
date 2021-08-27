int main()
{
    RNG rng(12345);
    Mat a = imread("1RT05508-0.jpg");
    imshow("原图", a);
    cvtColor(a, a, CV_RGB2GRAY);     //转为灰度图
    Canny(a, a, 100, 300, 3);         //进行边缘检测
    Mat bbb;
    cvtColor(a, bbb, CV_GRAY2BGR);
    vector<Vec2f> lines;
    HoughLines(a, lines, 1, CV_PI / 180, 100);    //检测
    for (size_t i = 0; i < lines.size(); i++)    //开始划线
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(bbb, pt1, pt2, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, CV_AA);       //实现随机颜色
    }
    imshow("效果图", bbb);
    cvWaitKey(10000);

}