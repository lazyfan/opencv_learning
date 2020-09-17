/*
������� ��������
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
	vector<vector<Point>> contours; //�������� ����ÿ��Ԫ����һ���㼯
	vector<Vec4i> hierachy; //����ͼ���� ÿ��Ԫ��Ϊ����4��int��������
	Canny(src, canny_output, threshold_value, threshold_value * 2, 3, false);
	//���� ���ص����� �������˽ṹ����ѡ�� ��������ģʽ �������ҷ��� �������ص�ƫ�� 
	findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	dst = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//�����ͼ�� ȫ�������� �ڼ������� ������ɫ �����߿� �����ߵ����� ���˽ṹͼ ���Ƶ������� ƫ��
		drawContours(dst, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow(output_win, dst);
}
