#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace std;
using namespace cv;


int main() 
{
	Mat srcImage, outImage;
	srcImage = imread("G:\\Desktop\\QD1.png");

	float number[4];
	Point2f overcenter[4];
	vector<vector<Point>> contours, contours2;
	vector<Vec4i> hierarchy;  //4元组合
	Mat draw, draw1, roi;
	int c = 0, ic = 0;
	int parentIdx = -1;
	float temp, temp1;
	vector<vector<Point>> newcontours;



	cvtColor(srcImage, srcImage, CV_RGB2GRAY);
	threshold(srcImage, srcImage, 30, 255, THRESH_BINARY);
	findContours(srcImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	draw = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0));
	draw1 = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0));
	drawContours(draw, contours, 1, Scalar(255, 0, 0), -1);

	//轮廓筛选
	for (int i = 0; i < contours.size(); i++)
	{
		//hierarchy[i][2] != -1 表示不是最外面的轮廓
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		//最外面的清 0
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}
		//找到定位点信息
		if (ic >= 2)
		{
			contours2.push_back(contours[parentIdx]);
			ic = 0;
			parentIdx = -1;
		}
	}

	vector<Point> center_all;  //center_all获取特性中心
	double area= contourArea(contours2[0]);
	for (int i = 0; i < contours2.size(); i++)
	{
		double temp = contourArea(contours2[i]);
		if (abs(temp-area>5))
			continue;
		Rect rect = boundingRect(Mat(contours2[i]));
		double w = rect.width;
		double h = rect.height;
		double rate = min(w, h) / max(w, h);
		if (rate > 0.9)   
		{
			Point2f points[4] = { rect.tl(),Point2f(rect.x,rect.y + rect.height),rect.br(),Point2f(rect.x + rect.width,rect.y) };
			for (int i = 0; i < 4; i++)
				line(draw, points[i], points[(i + 1) % 4], Scalar(0, 255, 0), 2);
			drawContours(draw1, contours2, -1, Scalar(255, 255, 255), -1);
			center_all.push_back(rect.tl() + Point(rect.width/2,rect.height/2));
		}
	}

	/*连接三个黑色正方形区域，将其变成一个轮廓，即可用最小矩形框选*/
	for (int i = 0; i < center_all.size(); i++)
	{
		line(draw1, center_all[i], center_all[(i + 1) % center_all.size()], Scalar(255, 0, 0), 3);
	}
	cvtColor(draw1, draw1, CV_RGB2GRAY);
	findContours(draw1, newcontours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	imshow("draw", srcImage);
	imshow("QD1.png", draw1);
	waitKey();
	//system("pause");
	return 0;
}