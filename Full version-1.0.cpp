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
	rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0), -1);
	rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255), -1);
	rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0), -1);
}

void Encode(char a) {//解码函数，ASCII转2进制
	for (int j = 7; j >= 0; j--) {
		encode[j] = a % 2;
		a = a / 2;
	}
}

void Draw(vector<int> out, int pictime, int length) {
	int key = 0;
	int change_part = 0;
	char str[20];//用于改文件名的字符串
	for (int count = 0; count < pictime; count++)//控制要画几张图
	{
		Mat src(720, 720, CV_8UC1, Scalar(255));
		drawBlock(src, Point(0, 0));
		drawBlock(src, Point(615, 0));
		drawBlock(src, Point(0, 615));//画那三个小框框

		for (int col = 0; col < 6; col++) {//画最上面那块
			for (int row = 0; row < 24; row++) {
				Point pos(row * 20 + 120, col * 20);
				Point recSize(20, 20);
				rectangle(src, pos, pos + recSize, Scalar((1 - out[key]) * 255), -1);//颜色1或者0，全填充
				key++;
				change_part++;
				if (key > length * 8 || change_part > 144) break;
			}
			if (key > length * 8 || change_part > 144) break;
		}

		for (int col = 0; col < 30; col++) {
			for (int row = 0; row < 30; row++) {
				Point pos(row * 20 + 120, col * 20 + 120);//因为只能从120开始画，前面是定位的方格
				Point recSize(20, 20);//pos和recSize都是point类
				rectangle(src, pos, pos + recSize, Scalar((1 - out[key]) * 255), -1);//颜色1或者0，全填充
				key++;
				if (key > length * 8 || change_part > 1044) break;
			}
			if (key > length * 8 || change_part > 1044) break;
		}

		for (int col = 0; col < 24; col++) {
			for (int row = 0; row < 6; row++) {
				Point pos(row * 20, col * 20 + 120);
				Point recSize(20, 20);
				rectangle(src, pos, pos + recSize, Scalar((1 - out[key]) * 255), -1);//颜色1或者0，全填充
				key++;
				if (key > length * 8) break;
			}
			if (key > length * 8) break;
		}
		change_part = 0;
		img.push_back(src);
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

//解码部分的函数
//初始化数组
void initArr(int* arr) {
	for (int i = 0; i < 6; i++) {
		arr[i] = 0;
	}
}
void mov(int* len) {
	for (int i = 0; i < 4; i++) {
		len[i] = len[i + 2];
	}
}

//判断五段是否成比1:1:3:1:1例
bool judge(int* len) {
	float arr[5];
	float average = 0;
	for (int i = 0; i < 5; i++) {
		arr[i] = len[i + 1] - len[i];
	}
	arr[2] = arr[2] / 3;//第三段除以3，判断五段是否相等
	for (int i = 0; i < 5; i++) {
		average += arr[i] / 5;
	}
	for (int i = 1; i < 5; i++) {
		//判断数据是否在均值上下波动，幅度为1/2
		if (fabs(average - arr[i]) > (average / 2))
			return false;
	}
	return true;
}



int* scan(Mat img, Point pos, int end, int axis) {
	int len[7] = { 0,0,0,0,0,0,0 };
	int i = 0;			//数组下标
	bool flag = false;
	if (axis == 1) {
		//遍历列
		for (int col = pos.y; col < end; col++) {
			Scalar color = img.at<uchar>(Point(pos.x, col));
			//首次遇见黑像素时开始计数，记录起始列坐标
			if (i == 0 && color.val[0] < 10) {
				len[i] = col;
				i++;
				flag = true;
			}
			//未记满五段，遇到不同像素则记录下一段
			else if (i > 0) {

				if (flag != (color.val[0] < 10)) {
					len[i] = col;
					i++;
					flag = (!flag);
				}
				if (col == end - 1) {
					//遇到边界时记录最后一段的长度
					len[i] = col + 1;
					i++;
				}
				if (i == 6) {

					//存满五段距离后判断是否成比例
					if (judge(len)) {
						//若成比例则退出,数组标识符设置为1
						len[6] = 1;
						break;
					}
					else {
						//判断失败则清除前两段距离，继续
						mov(len);
						i = 4;
					}
				}
			}
		}
	}

	if (axis == 0) {
		//遍历行
		for (int row = pos.x; row < end; row++) {
			Scalar color = img.at<uchar>(Point(row, pos.y));
			//首次遇见黑像素时开始计数
			if (i == 0 && color.val[0] < 10) {
				len[i] = row;
				i++;
				flag = true;
			}
			//未记满五段，遇到不同像素则记录下一段
			else if (i > 0) {
				if (flag != (color.val[0] < 10)) {
					len[i] = row;
					i++;
					flag = (!flag);
				}
				if (row == end - 1) {
					//遇到边界时记录最后一段的长度
					len[i] = row + 1;
					i++;
				}
				if (i == 6) {
					//存满五段距离后判断是否成比例
					if (judge(len)) {
						//若成比例则退出,数组标识符设置为1
						len[6] = 1;
						break;
					}
					else {
						//判断失败则清除前两段距离，继续
						mov(len);
						i = 4;
					}
				}
			}
		}
	}

	return len;
}

//找到位置探测符的位置
Rect findPosiation(Mat img, Point topLeft, Point bottomRight) {
	int* len;
	int left = 0, right = 0, top = 0, bottom = 0;
	for (int row = topLeft.x; row < bottomRight.x; row++) {
		//按列扫描找到上下边框
		len = scan(img, Point(row, topLeft.y), bottomRight.y, 1);
		if (len[6] == 1) {
			//首次找到对应列后，扫描之后的10列再判断
			int sum = 0;
			for (int i = 0; i < 10; i++) {
				sum += scan(img, Point(row + i, topLeft.y), bottomRight.y, 1)[6];
			}
			if (sum > 8) {
				top = len[0];
				bottom = len[5] - 1;
				break;
			}
		}
	}

	for (int col = topLeft.y; col < bottomRight.y; col++) {
		//按行扫描找到左右边框
		len = scan(img, Point(topLeft.x, col), bottomRight.x, 0);
		if (len[6] == 1) {
			//首次找到对应行后，扫描之后的10行再判断，保证正确
			int sum = 0;
			for (int i = 0; i < 10; i++) {
				sum += scan(img, Point(topLeft.x, col + i), bottomRight.x, 0)[6];
			}
			if (sum > 8) {
				left = len[0];
				right = len[5] - 1;
				break;
			}
		}
	}

	return Rect(Point(left, top), Point(right, bottom));
}

//找到有效区域
Point2f* getROI(Mat img) {
	//分别找到左上，左下，右上的探测符
	Rect topLeft = findPosiation(img, Point(0, 0), Point(img.cols, img.rows));
	Rect bottomLeft = findPosiation(img, Point(topLeft.tl().x, topLeft.br().y + 1), Point(topLeft.br().x + 10, img.rows));
	Rect topRight = findPosiation(img, Point(topLeft.br().x + 1, topLeft.tl().y), Point(img.cols, topLeft.br().y + 10));
	Point2f ROI[4];
	ROI[0] = topLeft.tl();
	ROI[1] = Point(topRight.br().x, topRight.y);
	ROI[2] = Point(bottomLeft.x, bottomLeft.br().y);
	ROI[3] = Point(topRight.br().x + bottomLeft.x - topLeft.x, topRight.y + bottomLeft.br().y - topLeft.y);
	return(ROI);
}

string decode(Mat img) {
	//解码
	string code;
	for(int col=0;col<6;col++)
		for (int row = 0; row < 24; row++) {
			Point pos(row * 20, col * 20), center(10, 10), tr(120, 0);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 10) {
				code.push_back('1');
			}
			else code.push_back('0');
		}
	for (int row = 0; row < 30; row++) {
		for (int col = 0; col < 30; col++) {
			Point pos(col * 20, row * 20), center(10, 10), tr(120, 120);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 10) {
				code.push_back('1');
			}
			else code.push_back('0');
		}
	}
	for (int col = 0; col < 24; col++)
		for (int row = 0; row < 6; row++) {
			Point pos(row * 20, col * 20), center(10, 10), tr(0, 120);
			Scalar color = img.at<uchar>(tr + pos + center);
			if (color.val[0] < 10) {
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

Mat handleImg(Mat img) {
	//简单图像处理
	cvtColor(img, img, CV_RGB2GRAY);	//灰度化
	//blur(img, img, Size(5, 5));			//滤波
	//threshold(img, img, 80, 255, THRESH_BINARY);	//二值化
	resize(img, img, Size(720, 720), 0, 0, INTER_NEAREST);//调整大小
	return img;
}

string code_recognition(Mat srcImage) {
	Mat dstImage;
	string code;
	//处理原始图片
	srcImage = handleImg(srcImage);
	//imshow("srcImg", srcImage);
	//waitKey();
	//得到有效区域
	try {
		Point2f* ROI = getROI(srcImage);
		Point2f srcArea[4];
		Point2f dstArea[4];
		for (int i = 0; i < 4; i++) {
			srcArea[i] = ROI[i];
		}
		dstArea[0] = Point2f(0, 0);
		dstArea[1] = Point2f(srcImage.cols - 1, 0);
		dstArea[2] = Point2f(0, srcImage.rows - 1);
		dstArea[3] = Point2f(srcImage.cols - 1, srcImage.rows - 1);

		//计算3个二维点对之间的仿射变换矩阵（2行x3列）
		Mat warpPerspective_mat = getPerspectiveTransform(srcArea, dstArea);
		//应用仿射变换，可以恢复出原图
		warpPerspective(srcImage, dstImage, warpPerspective_mat, srcImage.size());
		//imshow("img", dstImage);
		//waitKey();

		//resize(dstImage, dstImage, Size(720, 720), 0, 0, INTER_NEAREST);
		//imshow("dstImg", dstImage);
		//waitKey();
		//解码
		code = decode(srcImage);
	}
	catch (...) {
		;
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
		else srcImages.push_back(frame.clone());
		//imshow("video", frame);
		//waitKey();
	}
}

int main()
{
	int choice = 0;
	cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2) {
		cout << "您选择的功能序号无效，请重新选择" << endl;
		system("cls");
		cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
		cin >> choice;
	}
	cout << endl;
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
		pictime = (length * 8) / 1188;//判断要做几张图
		if (((length * 8) % 1188) != 0) pictime++;
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
		vector<Mat> srcImages;
		char videoname[20], outname[20];
		cout << "请输入要解码的视频名，例如“test.mp4”" << endl;
		cin >> videoname;
		cout << "请输入解码后存放信息的文件名，例如“out.txt”" << endl;
		cin >> outname;
		string videopath(videoname);
		string outfile(outname);
		ofstream out(outfile);

		readVideo(videopath, srcImages);
		string codes;
		string text;

		for (Mat img : srcImages) {
			string code;
			code = code_recognition(img);
			if (code.empty())
				continue;
			else codes.append(code);
		}
		text = codeToText(codes);
		//cout << codes;
		out << text;
		system("pause");
	}
	return 0;
}
