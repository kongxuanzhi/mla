int main()
{
    RNG rng(12345);
    Mat a = imread("1RT05508-0.jpg");
    imshow("原图", a);
    cvtColor(a, a, CV_RGB2GRAY);     //转为灰度图
    Canny(a, a, 100, 300, 3);         //边缘检测
    Mat bbb;
    cvtColor(a, bbb, CV_GRAY2BGR);
    vector<Vec4i> lines;
    HoughLinesP(a, lines, 1, CV_PI / 180, 50, 50, 10);   //检测
    for (size_t i = 0; i < lines.size(); i++)       //划线
    {
        Vec4i l = lines[i];
        line(bbb, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, CV_AA);      //用的是随机颜色
    }
    imshow("效果图", bbb);
    cvWaitKey(10000);

}