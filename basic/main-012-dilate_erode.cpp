/*
��̬ѧ�������ǻ������ͺ͸�ʴ��
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat src, d_dst, r_dst, dst, bt_dst;
char OUTPUT[] = "output image";
int element_size = 3;
int max_size = 15;
void CallBack_Demo(int, void*);

int main(int argc, char** argv) {
	
	src = imread("D:/learning/image/9.png", WINDOW_AUTOSIZE);
	if (src.empty()) {
		printf("can not found image...\n");
		return -1;
	}
	imshow("imput image", src);
	//namedWindow(OUTPUT);
	////�����϶��������size��С
	//createTrackbar("Element size:", OUTPUT, &element_size, max_size, CallBack_Demo);
	//CallBack_Demo(0, 0);

	//������ �ȸ�ʴ������ ȥ��С�Ķ��� MORPH_OPEN
	//�ղ��� �����ͺ�ʴ ���С�ĺڶ� MORPH_CLOSE
	//��̬ѧ�ݶ� ���ͼ�ȥ��ʴ MORPH_GRADIENT
	//��ñ ԭͼ-������ MORPH_TOPHAT
	//��ñ �ղ���-ԭͼ MORPH_BLACKHAT
	Mat kernel = getStructuringElement(MORPH_RECT, Size(15, 15), Point(-1, -1));
	morphologyEx(src, dst, MORPH_BLACKHAT, kernel, Point(-1, -1));
	imshow("output image", dst);

	waitKey(0);
	return 0;
}

//
void CallBack_Demo(int, void*) {
	int s = element_size * 2 + 1;
	//����ṹ��Ԫ��
	Mat structure_element = getStructuringElement(MORPH_RECT, Size(s, s), Point(-1, -1));
	//dilate(src, dst, structure_element, Point(-1, -1), 1); //����
	erode(src, dst, structure_element, Point(-1, -1), 1);  //��ʴ
	imshow(OUTPUT, dst);
	return;
}