/*点多边形测试*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;
int main(int argc, char** argv) {
	const int r = 100;
	Mat src = Mat::zeros(r * 4, r * 4, CV_8UC1);

	//6个点
	vector<Point> vert(6);
	vert[0] = Point(3 * r / 2, static_cast<int>(1.34*r));   
	vert[1] = Point(1 * r, 2 * r);
	vert[2] = Point(3 * r / 2, static_cast<int>(2.866*r));   
	vert[3] = Point(5 * r / 2, static_cast<int>(2.866*r));
	vert[4] = Point(3 * r, 2 * r);   
	vert[5] = Point(5 * r / 2, static_cast<int>(1.34*r));

	//绘制多边形
	for (int i = 0; i < 6; i++) {
		line(src, vert[i], vert[(i + 1) % 6], Scalar(255), 3, 8, 0);
	}
	
	vector<vector<Point>> contours; //轮廓
	vector<Vec4i> hierachy; //拓扑图
	Mat csrc;
	src.copyTo(csrc);
	findContours(csrc, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat raw_dist = Mat::zeros(csrc.size(), CV_32FC1);  //32位浮点型 1通道
	for (int row = 0; row < raw_dist.rows; row++) {
		for (int col = 0; col < raw_dist.cols; col++) {
			//返回double类型数据  轮廓 测试点 是否返回实际距离值
			double dist = pointPolygonTest(contours[0], Point2f(static_cast<float>(col), static_cast<float>(row)), true);
			//将双精度浮点类型强转为单精度浮点类型 将距离表示为像素上
			raw_dist.at<float>(row, col) = static_cast<float>(dist);  
		}
	}

	double minValue, maxValue;
	//获取图像上最大最小值
	minMaxLoc(raw_dist, &minValue, &maxValue, 0, 0, Mat());
	Mat drawImg = Mat::zeros(src.size(), CV_8UC3);
	//绘制
	for (int row = 0; row < drawImg.rows; row++) {
		for (int col = 0; col < drawImg.cols; col++) {
			//每个像素值代表距离
			float dist = raw_dist.at<float>(row, col);
			if (dist > 0) {
				// >0表示在轮廓里面 给B通道加上颜色 距离轮廓越近亮度越高
				drawImg.at<Vec3b>(row, col)[0] = (uchar)(abs(1.0 - (dist / maxValue)) * 255); //归一化
			}
			else if (dist < 0) {
				// <0表示在轮廓外面 给R通道加上颜色 距离轮廓越近亮度越高
				drawImg.at<Vec3b>(row, col)[2] = (uchar)(abs(1.0 - (dist / minValue)) * 255);
			} else {
				// =0表示在轮廓上 颜色为白色
				drawImg.at<Vec3b>(row, col)[0] = (uchar)(abs(255 - dist));
				drawImg.at<Vec3b>(row, col)[1] = (uchar)(abs(255 - dist));
				drawImg.at<Vec3b>(row, col)[2] = (uchar)(abs(255 - dist));
			}
		}
	}

	const char* output_win = "point polygon test demo";
	char input_win[] = "input image";
	namedWindow(input_win, WINDOW_AUTOSIZE);
	namedWindow(output_win, WINDOW_AUTOSIZE);

	imshow(input_win, src);
	imshow(output_win, drawImg);

	waitKey(0);
	return 0;
}