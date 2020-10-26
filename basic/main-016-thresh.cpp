#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>


using namespace cv;
using namespace std;

Mat src, gray_src, dst;
int threshold_value = 127;
int threshold_max = 255;
int threshold_type = 2;
int threshold_type_max = 4;
const char* output_title = "binary image";
void Threshold_Demo(int, void*);


int main(int argc, char** argv) {
	src = imread("D:/Learning/image/1.jpg");
	if (src.empty()) {
		printf("not image\n");
		return -1;
	}
	namedWindow("input image", WINDOW_AUTOSIZE);
	namedWindow(output_title, WINDOW_AUTOSIZE);  //Ҫ��ʾtrackbar �������ⲽ
	imshow("input image", src);
	createTrackbar("Treshold Value", output_title, &threshold_value, threshold_max, Threshold_Demo);
	//5��ȡ��ֵ�ķ������⻬��
	createTrackbar("Treshold Type", output_title, &threshold_type, threshold_type_max, Threshold_Demo);
	Threshold_Demo(0, 0);

	waitKey(0);
	return 0;
}

void Threshold_Demo(int, void*) {
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	threshold(gray_src, dst, threshold_value, threshold_max, threshold_type); 
	//threshold(gray_src, dst, 0, 255, THRESHOLD_OTSU | threshold_type);  //THRESHOLD_OTSU��THRESHOLD_TRAINGLE�Զ�ȡ��ֵ
	imshow(output_title, dst);
}