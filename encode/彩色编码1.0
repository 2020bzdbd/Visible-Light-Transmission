#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<math.h>
#include<vector>
#include<strstream>
#include<fstream>
#include <windows.h>
using namespace std;
using namespace cv;

//编码部分的函数

int encode[8] = { 0 };
vector<Mat>img;

void drawBlock(Mat img, Point tr) {//用来画角落的定位矩形，画法是黑白黑3个矩形框叠加
	rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0,0,0), -1);
	rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255,255,255), -1);
	rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0,0,0), -1);
}

void Encode(char a) {//编码函数，ASCII转2进制
	for (int j = 7; j >= 0; j--) {
		encode[j] = a % 2;
		a = a / 2;
	}
}

void Draw(vector<int> out, int pictime, int length) {
	int key = 0;
	int one = 255, two = 255, three = 255;//用来存三通道的值
	int a, b;
	char str[20];//用于改文件名的字符串
	for (int count = 0; count < pictime; count++)//控制要画几张图
	{
		Mat src(720, 720, CV_8UC3, Scalar(255,255,255));
		drawBlock(src, Point(0, 0));
		drawBlock(src, Point(615, 0));
		drawBlock(src, Point(0, 615));
		drawBlock(src, Point(615, 615));//画那4个小框框

		for (int col = 0; col < 36; col++) {//画最上面那块
			for (int row = 0; row < 24; row++) {
				Point pos(row * 20 + 120, col * 20);
				Point recSize(20, 20);
				a = out[key];
				b = out[key + 1];
				if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
				if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
				if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
				if (a == 1 && b == 1) { one = 255; two = 255; three = 255; }
				rectangle(src, pos, pos + recSize, Scalar(one,two,three), -1);//颜色1或者0，全填充
				key = key + 2;
				if (key > length * 8) break;
			}
			if (key > length * 8) break;
		}

		for (int col = 0; col < 24; col++) {
			for (int row = 24; row < 30; row++) {
				Point pos(row * 20 + 120, col * 20 + 120);//因为只能从120开始画，前面是定位的方格
				Point recSize(20, 20);//pos和recSize都是point类
				a = out[key];
				b = out[key + 1];
				if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
				if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
				if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
				if (a == 1 && b == 1) { one = 255; two = 255; three = 255; }
				rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
				key = key + 2;
				if (key > length * 8) break;
			}
			if (key > length * 8) break;
		}

		for (int col = 0; col < 24; col++) {
			for (int row = 0; row < 6; row++) {
				Point pos(row * 20, col * 20 + 120);
				Point recSize(20, 20);
				a = out[key];
				b = out[key + 1];
				if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
				if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
				if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
				if (a == 1 && b == 1) { one = 255; two = 255; three = 255; }
				rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
				key = key + 2;
				if (key > length * 8) break;
			}
			if (key > length * 8) break;
		}
		img.push_back(src);
		//sprintf(str, "color-%d.jpg", count);
		//imwrite(str, src);
	}
}
void Video() {
	char name2[20];
	cout << "请输入编码后的视频文件名（mp4格式），例如“test.mp4”:" << endl;
	cin >> name2;
	int frame_rate = 10;
	cout << "请输入视频帧率，约束编码后的视频长度:" << endl;
	cin >> frame_rate;
	VideoWriter video(name2, CAP_ANY, frame_rate, Size(720, 720));
	for (size_t i = 0; i < img.size(); i++)
	{
		Mat image = img[i].clone();
		// 流操作符，把图片传入视频
		video << image;
	}
}
int main()
{
	int choice = 1;
	/*cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2) {
		cout << "您选择的功能序号无效，请重新选择" << endl;
		system("cls");
		cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
		cin >> choice;
	}
	cout << endl;
	*/
	if (choice == 1) {
		char name1[20];
		cout << "请输入需要转换的信息所存放文件名，例如“in.txt”:" << endl;
		cin >> name1;
		//信息读入部分
		vector<int>out;
		string inString;
		fstream fs(name1);
		stringstream ss;
		ss << fs.rdbuf();
		inString = ss.str();
		//cout << inString << endl;//输出检验
		int length;//长度
		length = inString.size();//求字符串长度

		//信息到二进制-编码部分
		int pictime;
		int LEN = 8;
		pictime = (length * 4) / 1152;//判断要做几张图
		if (((length * 4) % 1152) != 0) pictime++;
		void Encode(char a);//编码函数
		for (int i = 0; i < length; i++) {
			Encode(inString[i]);//编码函数，设置全局变量 encode[LEN]来传递
			for (int j = 0; j < LEN; j++)
				out.push_back(encode[j]);//装入out容器
		}
		for (int i = 0; i < LEN; i++) {
			out.push_back(0);
		}//设置终止符号

		//二进制到图片部分，绘制识别区域
		Draw(out, pictime, length);
		//图片转视频
		Video();
	}
	else {

	}
	return 0;
}
