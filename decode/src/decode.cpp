#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<math.h>
#include<vector>
#include<strstream>
#include<fstream>
#include <windows.h>
#include <cstdlib>
#include <algorithm>
using namespace std;
using namespace cv;


string decode(Mat img) {
	string code;
	//解码
	for (int col = 0; col < 36; col++)
		for (int row = 0; row < 24; row++) {
			Point pos(row * 20, col * 20), center(10, 10), tr(120, 0);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 100) {
				code.push_back('1');
			}
			else code.push_back('0');
		}
	for (int row = 0; row < 24; row++) {
		for (int col = 24; col < 30; col++) {
			Point pos(col * 20, row * 20), center(10, 10), tr(120, 120);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 100) {
				code.push_back('1');
			}
			else code.push_back('0');
		}
	}
	for (int col = 0; col < 24; col++)
		for (int row = 0; row < 6; row++) {
			Point pos(row * 20, col * 20), center(10, 10), tr(0, 120);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 100) {
				code.push_back('1');
			}
			else code.push_back('0');
		}
	return code;
}

string codeToText(string codes) {
	string text = "";
	auto it = codes.begin();
	int num = 0;
	while (true) {
		num = 0;
		for (int i = 0; i < 8; i++, it++) {
			num += (*it - '0') * pow(2, 7 - i);
		}
		if (num == 0)
			break;
		text.push_back(char(num));
	}
	return text;
}

Mat handleImg(Mat& srcImg) {
	Mat midImg = srcImg.clone();
	//简单图像处理
	cvtColor(midImg, midImg, CV_RGB2GRAY);	//灰度化
	cvtColor(srcImg, srcImg, CV_RGB2GRAY);
	for (int i = 0; i < 5; i++)
		GaussianBlur(midImg, midImg, Size(3, 3), 0);			//滤波
	threshold(midImg, midImg, 80, 255, THRESH_BINARY);	//二值化
	return midImg;
}

Point2f* sortCorner(vector<Point2f>& mc) {
	Point2f* p = new Point2f[4];
	float sum[4];
	for (int i = 0; i < mc.size(); i++) {
		sum[i] = mc[i].x + mc[i].y;
	}
	int bottomRight = max_element(sum, sum + 4) - sum;
	int topLeft = min_element(sum, sum + 4) - sum;
	int topRight = 0;
	int bottomLeft = 0;
	for (int i = 0; i < mc.size(); i++) {
		if (i == bottomRight || i == topLeft)
			continue;
		if (mc[i].x > mc[i].y)
			topRight = i;
		if (mc[i].x < mc[i].y)
			bottomLeft = i;
	}
	p[0] = mc[topLeft];
	p[1] = mc[topRight];
	p[2] = mc[bottomLeft];
	p[3] = mc[bottomRight];
	return p;
}

vector<Point2f> getCenter(vector<vector<Point>>& corner) {
	vector<Moments> mu(corner.size());
	for (int i = 0; i < corner.size(); i++)
	{
		mu[i] = moments(corner[i], false);
	}

	//  计算中心矩
	vector<Point2f> mc(corner.size());
	for (int i = 0; i < corner.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	return mc;
}

vector<vector<Point>> findCorner(Mat& midImg) {
	vector<vector<Point>> contours, corner;
	vector<Vec4i> hierarchy;
	findContours(midImg, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	int parentIdx = -1, ic = 0;
	for (int i = 0; i < contours.size(); i++)		//找到四个角的轮廓放进corner
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
			corner.push_back(contours[parentIdx]);
			ic = 0;
			parentIdx = -1;
		}
	}
	return corner;
}

void getROI(Mat& midImg, Mat& srcImg, Mat& dstImg) {
	try {
		vector<vector<Point>> corner = findCorner(midImg);

		vector<Point2f> mc(corner.size());//获得四个角的中心点
		if (int(corner.size()) != 4)
			return;
		mc = getCenter(corner);

		Point2f* P1;
		Point2f P2[4];
		P1 = sortCorner(mc);		//四个角按照左上，右上，左下，右下排序
		P2[0] = Point2f(52, 52);	//对应透视变换后的点位
		P2[1] = Point2f(668, 52);
		P2[2] = Point2f(52, 668);
		P2[3] = Point2f(668, 668);
		//透视变换
		Mat elementTransf = getPerspectiveTransform(P1, P2);
		warpPerspective(srcImg, dstImg, elementTransf, srcImg.size(), 1, 0, Scalar(255, 255, 255));
		//imshow("img", dstImg);
		//waitKey();
	}
	catch (...) {

	}
}

string code_recognition(Mat& srcImage) {
	Mat midImage;
	Mat dstImage;
	string code = "";

	try {
		//处理原始图片
		midImage = handleImg(srcImage);
		//imshow("srcImg", midImage);
		//waitKey();
		getROI(midImage, srcImage, dstImage);
		code = decode(dstImage);
	}
	catch (...) {

	}
	return code;
}

void readVideo(string videopath, vector<Mat>& srcImages) {
	//读取视频,参数为0则打开摄像头
	VideoCapture capture(videopath);
	Mat frame;
	while (true) {
		//一帧一帧读
		capture >> frame;
		if (frame.empty()) {
			break;
		}
		else {
			resize(frame, frame, Size(720, 720), 0, 0, INTER_NEAREST);//调整大小
			srcImages.push_back(frame.clone());
			frame.release();
		}
	}
}

int main()
{
	vector<Mat> srcImages;
	string videopath("G:\\Desktop\\pa\\t.mp4");
	string outfile("G:\\Desktop\\pa\\out.txt");
	ofstream out(outfile);

	readVideo(videopath, srcImages);
	string codes;
	vector<string> sv;
	string text;

	int i = 0;
	for (i = 0; i < srcImages.size(); i++) {
		string code;
		code = code_recognition(srcImages[i]);
		if (code.empty())
			continue;
		else {
			sv.push_back(code);
			break;
		}
	}
	i = i + 3;
	for (; i < srcImages.size(); i += 3) {
		string code;
		code = code_recognition(srcImages[i]);
		if (code.empty())
			continue;
		else sv.push_back(code);
	}

	for (int i = 0; i < sv.size(); i ++) {
		codes.append(sv[i]);
	}
	text = codeToText(codes);
	//cout << codes;
	out << text;
	system("pause");
	return 0;
}