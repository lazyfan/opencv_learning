/*ֱ��ͼ����ͶӰ(Back Projection)*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat src; Mat hsv; Mat hue; 
vector<Mat> h;
int bins = 12; //bin����
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

	//��BGRɫ�ʿռ�ת��ΪHSV�ռ� Hue, Saturation, Value ��ɫ�� ���Ͷ� ���� 
	cvtColor(src, hsv, COLOR_BGR2HSV);

	hue.create(hsv.size(), hsv.depth());
	int nchannels[] = { 0, 0 }; //��ֵĶ�Ӧ��ϵ ��ԭͼ��0ͨ���ֵ�Ŀ��ͼ��0ͨ��
	//������ľ��󣨻�������飩��ĳЩͨ����ָ��Ƹ���Ӧ��������󣨻�������飩��ĳЩͨ���У�1��ʾͼ����Ӧ��ϵ����
	mixChannels(&hsv, 1, &hue, 1, nchannels, 1); //����hue��ֻ��hsv�еĵ�0ͨ����ֵ
	
	//���Ϸ����ȼ��ڣ�
	//split(hsv, h);

	createTrackbar("Histogram Bins:", window_image, &bins, 180, Hist_And_Backprojection);
	Hist_And_Backprojection(0, 0);

	imshow(window_image, src);
	waitKey(0);
	return 0;
}

void Hist_And_Backprojection(int, void*) {
	float range[] = { 0, 180 }; //����ֵ��Χ
	const float *histRanges = { range }; //��Ϊֻ����1��ͼ����ֻ��һ����Χ
	Mat h_hist;
	//����hue�ϵ�0ͨ���ϵ�ֱ��ͼ
	calcHist(&h[0], 1, 0, Mat(), h_hist, 1, &bins, &histRanges, true, false);
	normalize(h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat()); //��һ����0-255֮��
	//cout << h_hist << endl;

	Mat backPrjImage;
	//����ͶӰ
	calcBackProject(&h[0], 1, 0, h_hist, backPrjImage, &histRanges, 1, true);
	imshow("BackProj", backPrjImage);

	//����ֱ��ͼ
	int hist_h = 400;
	int hist_w = 400;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	int bin_w = (hist_w / bins);
	for (int i = 1; i < bins; i++) {
		rectangle(histImage, 
			Point((i - 1)*bin_w, (hist_h - cvRound(h_hist.at<float>(i - 1) * (400 / 255)))),//����
			//Point(i*bin_w, (hist_h - cvRound(h_hist.at<float>(i) * (400 / 255)))),
			Point(i*bin_w, hist_h),//����
			Scalar(0, 0, 255), -1);
	}
	imshow("Histogram", histImage);

	return;
}

