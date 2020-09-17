#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
Mat src, dst, map_x, map_y;
const char* OUTPUT_TITLE = "remap demo";
int index = 0;
void update_map(void);
int main(int argc, char** argv) {
	src = imread("D:/images/ppm/blueeye.ppm");
	if (!src.data) {
		printf("could not load image...\n");
		return -1;
	}
	std::cout << "channal: " << src.channels() << std::endl;
	char input_win[] = "input image";
	namedWindow(input_win, WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_TITLE, WINDOW_AUTOSIZE);
	imshow(input_win, src);

	map_x.create(src.size(), CV_32FC1); // map1: x坐标，也可以是(x, y)
	map_y.create(src.size(), CV_32FC1); // map2: y坐标，如果map1传入的是(x, y)，map2可不传

	int c = 0;
	while (true) {
		c = waitKey(500);
		if ((char)c == 27) { // 当按下ESC时，退出循环
			break;
		}
		index = c % 4;
		update_map();  // 根据按下的键 更新map
		remap(src, dst, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 255, 255));
		imshow(OUTPUT_TITLE, dst);
	}

	return 0;
}

// 对坐标进行操作
void update_map(void) {
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			switch (index) {
			case 0:  // 长宽缩小2倍 图像位于中心
				if (col > (src.cols * 0.25) && col <= (src.cols*0.75) && row > (src.rows*0.25) && row <= (src.rows*0.75)) {
					map_x.at<float>(row, col) = 2 * (col - src.cols*0.25 - 1);
					map_y.at<float>(row, col) = 2 * (row - src.rows*0.25 - 1);
				}
				else {
					map_x.at<float>(row, col) = 0;
					map_y.at<float>(row, col) = 0;
				}
				break;
			case 1:
				map_x.at<float>(row, col) = (src.cols - col - 1);  // 水平翻转
				map_y.at<float>(row, col) = row;
				break;
			case 2:
				map_x.at<float>(row, col) = col;
				map_y.at<float>(row, col) = (src.rows - row - 1);  // 垂直翻转
				break;
			case 3:
				map_x.at<float>(row, col) = (src.cols - col - 1);  // 水平 垂直翻转
				map_y.at<float>(row, col) = (src.rows - row - 1);
				break;
			}

		}
	}
}
