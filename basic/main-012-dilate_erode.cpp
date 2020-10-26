/*
形态学操作都是基于膨胀和腐蚀的
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
	////设置拖动滑块控制size大小
	//createTrackbar("Element size:", OUTPUT, &element_size, max_size, CallBack_Demo);
	//CallBack_Demo(0, 0);

	//开操作 先腐蚀后膨胀 去掉小的对象 MORPH_OPEN
	//闭操作 先膨胀后腐蚀 填充小的黑洞 MORPH_CLOSE
	//形态学梯度 膨胀减去腐蚀 MORPH_GRADIENT
	//顶帽 原图-开操作 MORPH_TOPHAT
	//黑帽 闭操作-原图 MORPH_BLACKHAT
	Mat kernel = getStructuringElement(MORPH_RECT, Size(15, 15), Point(-1, -1));
	morphologyEx(src, dst, MORPH_BLACKHAT, kernel, Point(-1, -1));
	imshow("output image", dst);

	waitKey(0);
	return 0;
}

//
void CallBack_Demo(int, void*) {
	int s = element_size * 2 + 1;
	//定义结构型元素
	Mat structure_element = getStructuringElement(MORPH_RECT, Size(s, s), Point(-1, -1));
	//dilate(src, dst, structure_element, Point(-1, -1), 1); //膨胀
	erode(src, dst, structure_element, Point(-1, -1), 1);  //腐蚀
	imshow(OUTPUT, dst);
	return;
}