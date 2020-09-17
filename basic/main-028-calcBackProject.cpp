/*直方图反向投影(Back Projection)*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat src; Mat hsv; Mat hue; 
vector<Mat> h;
int bins = 12; //bin数量
void Hist_And_Backprojection(int, void*);
int main(int argc, char** argv) {
	src = imread("D:/images/ppm/lena.ppm");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	const char*  window_image = "input image";
	namedWindow(window_image, WINDOW_NORMAL);
	namedWindow("BackProj", WINDOW_NORMAL);
	namedWindow("Histogram", WINDOW_NORMAL);

	//将BGR色彩空间转换为HSV空间 Hue, Saturation, Value 即色调 饱和度 明度 
	cvtColor(src, hsv, COLOR_BGR2HSV);

	hue.create(hsv.size(), hsv.depth());
	int nchannels[] = { 0, 0 }; //拆分的对应关系 即原图第0通道分到目标图第0通道
	//把输入的矩阵（或矩阵数组）的某些通道拆分复制给对应的输出矩阵（或矩阵数组）的某些通道中，1表示图像或对应关系个数
	mixChannels(&hsv, 1, &hue, 1, nchannels, 1); //现在hue中只有hsv中的第0通道的值
	
	//以上方法等价于：
	//split(hsv, h);

	createTrackbar("Histogram Bins:", window_image, &bins, 180, Hist_And_Backprojection);
	Hist_And_Backprojection(0, 0);

	imshow(window_image, src);
	waitKey(0);
	return 0;
}

void Hist_And_Backprojection(int, void*) {
	float range[] = { 0, 180 }; //像素值范围
	const float *histRanges = { range }; //因为只计算1张图所以只有一个范围
	Mat h_hist;
	//计算hue上第0通道上的直方图
	calcHist(&h[0], 1, 0, Mat(), h_hist, 1, &bins, &histRanges, true, false);
	normalize(h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat()); //归一化到0-255之间
	//cout << h_hist << endl;

	Mat backPrjImage;
	//反向投影
	calcBackProject(&h[0], 1, 0, h_hist, backPrjImage, &histRanges, 1, true);
	imshow("BackProj", backPrjImage);

	//绘制直方图
	int hist_h = 400;
	int hist_w = 400;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	int bin_w = (hist_w / bins);
	for (int i = 1; i < bins; i++) {
		rectangle(histImage, 
			Point((i - 1)*bin_w, (hist_h - cvRound(h_hist.at<float>(i - 1) * (400 / 255)))),//左上
			//Point(i*bin_w, (hist_h - cvRound(h_hist.at<float>(i) * (400 / 255)))),
			Point(i*bin_w, hist_h),//右下
			Scalar(0, 0, 255), -1);
	}
	imshow("Histogram", histImage);

	return;
}

