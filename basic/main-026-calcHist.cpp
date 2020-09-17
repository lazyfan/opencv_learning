/*计算不同通道上的直方图*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat src = imread("D:/images/ppm/lena.ppm");
	if (!src.data) {
		printf("could not load image...\n");
		return -1;
	}
	char INPUT_T[] = "input image";
	char OUTPUT_T[] = "histogram demo";
	namedWindow(INPUT_T, WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_T, WINDOW_AUTOSIZE);
	imshow(INPUT_T, src);

	// 分通道显示
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	//imshow("single channel demo", bgr_planes[0]);

	// 计算直方图
	int histSize = 256; //bin数量
	float range[] = { 0, 256 }; //像素值范围
	const float *histRanges = { range }; //因为只计算1张图所以只有一个范围
	Mat b_hist, g_hist, r_hist;
	//返回的直方图大小为1*bins
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRanges, true, false);

	// 对直方图归一化 方便在同一张图中比较
	int hist_h = 400;
	int hist_w = 512;
	int bin_w = hist_w / histSize;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

	// render histogram chart 绘制每个通道上的直方图
	for (int i = 1; i < histSize; i++) {
		//根据左上点和右下点绘制矩形
		rectangle(histImage, Point((i - 1)*bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);

		rectangle(histImage, Point((i - 1)*bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);

		rectangle(histImage, Point((i - 1)*bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}
	imshow(OUTPUT_T, histImage);

	waitKey(0);
	return 0;
}