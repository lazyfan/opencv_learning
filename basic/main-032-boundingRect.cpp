/*轮廓周围绘制矩形框和圆形框*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;
Mat src, gray_src, drawImg;
int threshold_v = 170;
int threshold_max = 255;
const char* output_win = "rectangle-demo";
RNG rng(12345);
void Contours_Callback(int, void*);
int main(int argc, char** argv) {
	src = imread("D:/images/ppm/lena.ppm");
	if (!src.data) {
		printf("could not load image...\n");
		return -1;
	}
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	blur(gray_src, gray_src, Size(3, 3), Point(-1, -1));
	
	const char* source_win = "input image";
	namedWindow(source_win, WINDOW_AUTOSIZE);
	namedWindow(output_win, WINDOW_AUTOSIZE);
	imshow(source_win, src);

	createTrackbar("Threshold Value:", output_win, &threshold_v, threshold_max, Contours_Callback);
	Contours_Callback(0, 0);

	waitKey(0);
	return 0;
}

void Contours_Callback(int, void*) {
	Mat binary_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	threshold(gray_src, binary_output, threshold_v, threshold_max, THRESH_BINARY);
	//imshow("binary image", binary_output);
	findContours(binary_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));

	vector<vector<Point>> contours_ploy(contours.size()); //闭合轮廓
	vector<Rect> ploy_rects(contours.size()); //闭合轮廓周围的最小矩形
	vector<Point2f> ccs(contours.size()); //闭合轮廓周围最小圆形的圆心
	vector<float> radius(contours.size()); //闭合轮廓周围最小圆形的半径

	vector<RotatedRect> minRects(contours.size()); //闭合轮廓周围的旋转矩形
	vector<RotatedRect> myellipse(contours.size()); //闭合轮廓周围的椭圆形

	for (size_t i = 0; i < contours.size(); i++) {
		//基于RDP算法实现,减少多边形轮廓点数 获得闭合的轮廓
		approxPolyDP(Mat(contours[i]), contours_ploy[i], 3, true);
		//得到轮廓周围最小矩形左上交点坐标和右下角点坐标，绘制一个矩形
		ploy_rects[i] = boundingRect(contours_ploy[i]);  
		//得到最小区域圆形的圆心和半径
		minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
		if (contours_ploy[i].size() > 5) {
			//得到最小区域椭圆
			myellipse[i] = fitEllipse(contours_ploy[i]);
			//得到一个旋转的矩形，返回旋转矩形
			minRects[i] = minAreaRect(contours_ploy[i]);
		}
	}

	// draw it
	//drawImg = Mat::zeros(src.size(), src.type());
	Mat drawImg;
	src.copyTo(drawImg);
	Point2f pts[4];
	for (size_t t = 0; t < contours.size(); t++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//rectangle(drawImg, ploy_rects[t], color, 2, 8);
		//circle(drawImg, ccs[t], radius[t], color, 2, 8);
		//绘制旋转矩形和椭圆
		if (contours_ploy[t].size() > 5) {
			ellipse(drawImg, myellipse[t], color, 1, 8);
			minRects[t].points(pts);
			for (int r = 0; r < 4; r++) {
				line(drawImg, pts[r], pts[(r + 1) % 4], color, 1, 8);
			}
		}
	}

	imshow(output_win, drawImg);
	return;
}