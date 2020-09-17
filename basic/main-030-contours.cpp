/*
检测轮廓 绘制轮廓
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat src, dst;
const char* output_win = "findcontours-demo";
int threshold_value = 100;
int threshold_max = 255;
RNG rng;
void Demo_Contours(int, void*);
int main(int argc, char** argv) {
	src = imread("D:/images/ppm/malight.ppm");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input-image", WINDOW_AUTOSIZE);
	namedWindow(output_win, WINDOW_AUTOSIZE);
	imshow("input-image", src);
	cvtColor(src, src, COLOR_BGR2GRAY);

	const char* trackbar_title = "Threshold Value:";
	createTrackbar(trackbar_title, output_win, &threshold_value, threshold_max, Demo_Contours);
	Demo_Contours(0, 0);

	waitKey(0);
	return 0;
}

void Demo_Contours(int, void*) {
	Mat canny_output;
	vector<vector<Point>> contours; //轮廓集合 其中每个元素是一个点集
	vector<Vec4i> hierachy; //拓扑图集合 每个元素为长度4的int类型向量
	Canny(src, canny_output, threshold_value, threshold_value * 2, 3, false);
	//输入 返回的轮廓 返回拓扑结构（可选） 轮廓返回模式 轮廓查找方法 轮廓像素的偏移 
	findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	dst = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//输出的图像 全部的轮廓 第几个轮廓 绘制颜色 绘制线宽 绘制线的类型 拓扑结构图 绘制的最大层数 偏移
		drawContours(dst, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow(output_win, dst);
}
