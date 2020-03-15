#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace std;
using namespace cv;


int main() 
{
	Mat srcImage, outImage;
	srcImage = imread("G:\\Desktop\\QD2.png");
	string code;

	float number[4];
	Point2f overcenter[4];
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;  //4元组合
	Mat draw, draw1, roi;
	int c = 0, ic = 0;
	int parentIdx = -1;
	vector<vector<Point>> newcontours;
	Point tl(0, 0);



	cvtColor(srcImage, srcImage, CV_RGB2GRAY);
	threshold(srcImage, srcImage, 20, 255, THRESH_BINARY);
	findContours(srcImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	draw = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0));
	draw1 = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0));
	drawContours(draw, contours, 1, Scalar(255, 0, 0), -1);

	//轮廓筛选
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > 9500 && area < 10500) {
			newcontours.push_back(contours[i]);
		}
	}

	for (int i = 0; i < newcontours.size(); i++)
	{
		Rect rect = boundingRect(Mat(newcontours[i]));
		double w = rect.width;
		double h = rect.height;
		double area = w * h;
		if (area > 10000 && area < 11000)
		{
			tl = rect.br();
			rectangle(draw1, rect, Scalar(255,255,255), -1);
		}
	}

	for (int row = 0; row < 30; row++) {
		for (int col = 0; col < 30; col++) {
			Point pos(row * 20, col * 20), center(10, 10);
			Scalar color = srcImage.at<uchar>(tl + pos + center);
			if (color.val[0] > 250) {
				code.append("1");
			}
			else code.append("0");
		}
	}

	cout << code << endl;
	imshow("QD1.png", draw1);
	waitKey();
	//system("pause");
	return 0;
}