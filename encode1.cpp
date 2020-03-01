#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"//识别下面的颜色标识

using namespace std;
using namespace cv;

int encode[8] = { 0 };

void drawBlock(Mat img, Point tr) {//用来画角落的定位矩形，画法是黑白黑3个矩形框叠加
	rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0), -1);
	rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255), -1);
	rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0), -1);
}

void Encode(char a){//解码函数，ASCII转2进制
	for (int j = 7; j >= 0; j--) {
		encode[j] = a % 2;
		a = a / 2;
	}
}

void Draw(vector<int> out, int pictime, int length){
	int key = 0;
	char str[20];//用于改文件名的字符串
	for (int count = 0; count < pictime; count++)//控制要画几张图
	{
		Mat src(720, 720, CV_8UC1, Scalar(255));
		drawBlock(src, Point(0, 0));
		drawBlock(src, Point(615, 0));
		drawBlock(src, Point(0, 615));//画那三个小框框

		for (int row = 0; row < 30; row++) {
			for (int col = 0; col < 30; col++) {
				Point pos(row * 20 + 120, col * 20 + 120);//因为只能从120开始画，前面是定位的方格
				Point recSize(20, 20);//pos和recSize都是point类
				rectangle(src, pos, pos + recSize, Scalar((1 - out[key]) * 255), -1);//颜色1或者0，全填充
				key++;
				if (key > length * 8) break;
			}
			if (key > length * 8) break;
		}
		sprintf(str, "./QD%d.jpg", count);//改变文件名
		imwrite(str, src);//保存图片
		imshow("src", src);
		waitKey();
	}
}

int main() {
	//信息读入部分
	vector<int>out;
	string inString;
	fstream fs("file.txt");
	stringstream ss;
	ss << fs.rdbuf();
	inString = ss.str();
	cout << inString << endl;
	int length;//长度
	length = inString.size();//求字符串长度

	//信息到二进制-编码部分
	int pictime;
	int LEN = 8;
	pictime = (length * 8) / 900;//判断要做几张图
	if ((length % 225) != 0) pictime++;
	void Encode(char a);//编码函数
	for (int i = 0; i < length; i++) {
		Encode(inString[i]);//编码函数，设置全局变量 encode[LEN]来传递
		for (int j = 0; j < LEN; j++)
			out.push_back(encode[j]);//装入out容器
	}
	for (int i = 0; i < LEN; i++) {
		out.push_back(0);
	}//设置终止符号
	for (int i = 0; i < out.size(); i++)
		cout << out.at(i);//输出检验
	cout << endl;
	
	//二进制到图片部分，绘制识别区域
	Draw(out, pictime, length);
	return 0;
}
