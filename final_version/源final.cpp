//#include<iostream>
//#include<opencv2/opencv.hpp>
//#include <opencv2\imgproc\types_c.h>
//#include<math.h>
//#include<vector>
//#include<strstream>
//#include<fstream>
//#include <windows.h>
//#include <cstdlib>
//#include <algorithm>
//using namespace std;
//using namespace cv;
//
////编码部分的函数
//
//int encode[8] = { 0 };
//vector<Mat>img;
//string v_name;
//
//char Cxor(char a, char b) {
//	if (a == b) {
//		return '0';
//	}
//	else if (a != b) {
//		return '1';
//	}
//}
//
//string CalculateCRC(string data, string& CRC) {
//	data.append(string(CRC.size(), '0'));
//	for (int i = 0; i < data.size() - CRC.size(); i++) {
//		if (data[i] == '1') {
//			for (int j = 0; j < CRC.size(); j++) {
//				data[i + j] = Cxor(data[i + j], CRC[j]);
//			}
//		}
//	}
//	return data.substr(data.size() - CRC.size(), CRC.size());
//}
//
//void drawBlock(Mat img, Point tr) {//用来画角落的定位矩形，画法是黑白黑3个矩形框叠加
//	rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0, 0, 0), -1);
//	rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255, 255, 255), -1);
//	rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0, 0, 0), -1);
//}
//
//void Encode(unsigned char a) {//编码函数，ASCII转2进制
//	for (int j = 7; j >= 0; j--) {
//		encode[j] = a % 2;
//		a = a / 2;
//	}
//}
//
//void Draw(string code, int lastimg_flag) {
//	int key = 0;
//	int one = 255, two = 255, three = 255;//用来存三通道的值
//	int a, b;
//	Mat src(780, 780, CV_8UC3, Scalar(255, 255, 255));
//	drawBlock(src, Point(0 + 30, 0 + 30));
//	drawBlock(src, Point(615 + 30, 0 + 30));
//	drawBlock(src, Point(0 + 30, 615 + 30));
//	drawBlock(src, Point(615 + 30, 615 + 30));//画那4个小框框
//
//	for (int col = 0; col < 36 * 2; col++) {//画最上面那块
//		for (int row = 0; row < 24 * 2; row++) {
//			Point pos(row * 10 + 120 + 30, col * 10 + 30);
//			Point recSize(10, 10);
//			a = code[key] - 48;
//			b = code[key + 1] - 48;
//			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
//			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
//			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
//			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
//			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
//			key = key + 2;
//			if (lastimg_flag == 1 && key >= code.size())
//				goto enddraw;
//		}
//	}
//
//	for (int col = 0; col < 24 * 2; col++) {
//		for (int row = 24 * 2; row < 30 * 2; row++) {
//			Point pos(row * 10 + 120 + 30, col * 10 + 120 + 30);//因为只能从120开始画，前面是定位的方格
//			Point recSize(10, 10);//pos和recSize都是point类
//			a = code[key] - 48;
//			b = code[key + 1] - 48;
//			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
//			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
//			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
//			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
//			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
//			key = key + 2;
//			if (lastimg_flag == 1 && key >= code.size())
//				goto enddraw;
//		}
//	}
//
//	for (int col = 0; col < 24 * 2; col++) {
//		for (int row = 0; row < 6 * 2; row++) {
//			Point pos(row * 10 + 30, col * 10 + 120 + 30);
//			Point recSize(10, 10);
//			a = code[key] - 48;
//			b = code[key + 1] - 48;
//			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
//			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
//			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
//			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
//			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
//			key = key + 2;
//			if (lastimg_flag == 1 && key >= code.size())
//				goto enddraw;
//		}
//	}
//enddraw:
//	img.push_back(src);
//}
//
//void Video() {
//	char name2[20];
//	cout << "请输入编码后的视频文件名（mp4格式），例如“1.mp4”:" << endl;
//	cin >> name2;
//	int frame_rate = 10;
//	cout << "视频帧率默认为10" << endl;
//	VideoWriter video(name2, CAP_ANY, frame_rate, Size(780, 780));
//	for (size_t i = 0; i < img.size(); i++)
//	{
//		Mat image = img[i].clone();
//		// 流操作符，把图片传入视频
//		video << image;
//	}
//}
//
////解码部分的函数
//
//string decode(Mat& img, ofstream& ofv) {
//	string code;
//	string wrongcode;
//	string CRC = "10000010011000001000111011011011";
//	bool endFlag = false;
//
//	//解码
//	int divide = 120;
//	for (int col = 0; col < 36 * 2; col++)
//		for (int row = 0; row < 24 * 2; row++) {
//			Point pos(row * 10, col * 10), center(5, 5), tr(120, 0);
//			Scalar color = img.at<Vec3b>(tr + pos + center);
//			if (color[0] > 180 && color[1] > 180 && color[2] > 180) {
//				endFlag = true;
//				goto end;
//			}
//			/*cout << color << endl;*/
//			if (color[0] < 100 && color[1] < 100 && color[2] < 100) {
//				code += "00";
//			}
//			else if (color[0] < color[2] / 1.5 && color[1] < color[2] / 1.5) {
//				code += "01";
//			}
//			else if (color[0] < color[1] / 1.5 && color[2] < color[1] / 1.5) {
//				code += "10";
//			}
//			else if (color[1] < color[0] / 1.5 && color[2] < color[0] / 1.5) {
//				code += "11";
//			}
//			else if ((color[0] < 180) && color[1] < 180 && color[2] < 180) {
//				code += "00";
//			}
//		}
//	for (int row = 0; row < 24 * 2; row++) {
//		for (int col = 24 * 2; col < 30 * 2; col++) {
//			Point pos(col * 10, row * 10), center(5, 5), tr(120, 120);
//			Scalar color = img.at<Vec3b>(tr + pos + center);
//			if (color[0] > 180 && color[1] > 180 && color[2] > 180) {
//				endFlag = true;
//				goto end;
//			}
//			if (color[0] < 100 && color[1] < 100 && color[2] < 100) {
//				code += "00";
//			}
//			else if (color[0] < color[2] / 1.5 && color[1] < color[2] / 1.5) {
//				code += "01";
//			}
//			else if (color[0] < color[1] / 1.5 && color[2] < color[1] / 1.5) {
//				code += "10";
//			}
//			else if (color[1] < color[0] / 1.5 && color[2] < color[0] / 1.5) {
//				code += "11";
//			}
//			else if ((color[0] < 180) && color[1] < 180 && color[2] < 180) {
//				code += "00";
//			}
//		}
//	}
//	for (int col = 0; col < 24 * 2; col++)
//		for (int row = 0; row < 6 * 2; row++) {
//			Point pos(row * 10, col * 10), center(5, 5), tr(0, 120);
//			Scalar color = img.at<Vec3b>(tr + pos + center);
//			if (color[0] > 180 && color[1] > 180 && color[2] > 180) {
//				endFlag = true;
//				goto end;
//			}
//			if (color[0] < 100 && color[1] < 100 && color[2] < 100) {
//				code += "00";
//			}
//			else if (color[0] < color[2] / 1.5 && color[1] < color[2] / 1.5) {
//				code += "01";
//			}
//			else if (color[0] < color[1] / 1.5 && color[2] < color[1] / 1.5) {
//				code += "10";
//			}
//			else if (color[1] < color[0] / 1.5 && color[2] < color[0] / 1.5) {
//				code += "11";
//			}
//			else if ((color[0] < 180) && color[1] < 180 && color[2] < 180) {
//				code += "00";
//			}
//		}
//end:
//	if (code.empty())
//		return code;
//	else if (CalculateCRC(code, CRC) != string(CRC.size(), '0')) {
//		for (int i = 0; i < (code.size() - CRC.size()) / 8; i++)
//		{
//			ofv << (unsigned char)0;
//		}
//		code = code.substr(0, code.size() - CRC.size());
//		if (endFlag == true)
//			code.append(string(32, '0'));
//		return code;
//	}
//	else {
//		code = code.substr(0, code.size() - CRC.size());
//		for (int i = 0; i < code.size() / 8; i++) {
//			ofv << (unsigned char)255;
//		}
//		if (endFlag == true)
//			code.append(string(32, '0'));
//		return code;
//	}
//}
//
//string codeToText(string codes) {
//	string text = "";
//	auto it = codes.begin();
//	int num = 0, i = 0;
//	int zero_times = 0;
//	while (true) {
//		num = 0;
//		for (i = 0; i < 8; i++, it++) {
//			//cout << *it;
//			num += (*it - '0') * pow(2, 7 - i);
//		}
//		if (num == 0) {
//			zero_times++;
//		}
//		else zero_times = 0;
//		if (zero_times == 4)break;
//		//cout << (char)num;
//		text.push_back(unsigned char(num));
//	}
//	for (int i = 0; i < 3; i++)
//		text.pop_back();
//	return text;
//}
//
//Mat handleImg(Mat& srcImg) {
//	Mat midImg = srcImg.clone();
//	//简单图像处理
//	cvtColor(midImg, midImg, CV_RGB2GRAY);	//灰度化
//	//cvtColor(srcImg, srcImg, CV_RGB2GRAY);
//	for (int i = 0; i < 5; i++)
//		GaussianBlur(midImg, midImg, Size(3, 3), 0);			//滤波
//	threshold(midImg, midImg, 100, 255, THRESH_BINARY);	//二值化
//	return midImg;
//}
//
//Point2f* sortCorner(vector<Point2f>& mc) {
//	Point2f* p = new Point2f[4];
//	float sum[4];
//	for (int i = 0; i < mc.size(); i++) {
//		sum[i] = mc[i].x + mc[i].y;
//		//cout << "x:" << mc[i].x << " y:" << mc[i].y<<endl;
//	}
//	int bottomRight = max_element(sum, sum + 4) - sum;
//	int topLeft = min_element(sum, sum + 4) - sum;
//	int topRight = 0;
//	int bottomLeft = 0;
//	for (int i = 0; i < mc.size(); i++) {
//		if (i == bottomRight || i == topLeft)
//			continue;
//		if (mc[i].x > mc[i].y)
//			topRight = i;
//		if (mc[i].x < mc[i].y)
//			bottomLeft = i;
//	}
//	p[0] = mc[topLeft];
//	p[1] = mc[topRight];
//	p[2] = mc[bottomLeft];
//	p[3] = mc[bottomRight];
//	return p;
//}
//
//vector<Point2f> getCenter(vector<vector<Point>>& corner) {
//	vector<Moments> mu(corner.size());
//	for (int i = 0; i < corner.size(); i++)
//	{
//		mu[i] = moments(corner[i], false);
//	}
//
//	//  计算中心矩
//	vector<Point2f> mc(corner.size());
//	for (int i = 0; i < corner.size(); i++)
//	{
//		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
//	}
//	return mc;
//}
//
//vector<vector<Point>> findCorner(Mat& midImg) {
//	vector<vector<Point>> contours, corner;
//	vector<Vec4i> hierarchy;
//	findContours(midImg, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
//
//	int parentIdx = -1, ic = 0;
//	for (int i = 0; i < contours.size(); i++)		//找到四个角的轮廓放进corner
//	{
//		if (contourArea(contours[i]) > 50000) continue;
//		//hierarchy[i][2] != -1 表示不是最外面的轮廓
//		if (hierarchy[i][2] != -1 && ic == 0)
//		{
//			parentIdx = i;
//			ic++;
//		}
//		else if (hierarchy[i][2] != -1)
//		{
//			ic++;
//		}
//		//最外面的清 0
//		else if (hierarchy[i][2] == -1)
//		{
//			ic = 0;
//			parentIdx = -1;
//		}
//		//找到定位点信息
//		if (ic >= 2)
//		{
//			corner.push_back(contours[parentIdx]);
//			ic = 0;
//			parentIdx = -1;
//
//			/*Mat cimg = Mat(midImg.size(), midImg.type());
//			drawContours(cimg, corner, -1, Scalar(255), -1);
//			imshow("cor", cimg);
//			waitKey();*/
//		}
//	}
//	return corner;
//}
//
//void getROI(Mat& midImg, Mat& srcImg, Mat& dstImg) {
//
//	try {
//		vector<vector<Point>> corner = findCorner(midImg);
//
//		vector<Point2f> mc(corner.size());//获得四个角的中心点
//
//		/*Mat cimg=Mat(srcImg.size(),midImg.type());
//		drawContours(cimg, corner, -1, Scalar(255), -1);
//		imshow("cor", cimg);
//		waitKey();*/
//
//		if (corner.size() == NULL || int(corner.size()) != 4)
//			return;
//
//		mc = getCenter(corner);
//
//		//imshow("img", srcImg);
//		//waitKey();
//
//		Point2f* P1;
//		Point2f P2[4];
//		P1 = sortCorner(mc);		//四个角按照左上，右上，左下，右下排序
//		P2[0] = Point2f(52, 52);	//对应透视变换后的点位
//		P2[1] = Point2f(668, 52);
//		P2[2] = Point2f(52, 668);
//		P2[3] = Point2f(668, 668);
//		//透视变换
//		Mat elementTransf = getPerspectiveTransform(P1, P2);
//		warpPerspective(srcImg, dstImg, elementTransf, srcImg.size(), 1, 0, Scalar(255, 255, 255));
//		//imshow("img", dstImg);
//		//waitKey();
//	}
//	catch (...) {
//
//	}
//}
//
//string code_recognition(Mat& srcImage, ofstream& ofv) {
//	Mat midImage;
//	Mat dstImage;
//	string code = "";
//
//	try {
//		//处理原始图片
//		midImage = handleImg(srcImage);
//		//imshow("srcImg", midImage);
//		//waitKey();
//		getROI(midImage, srcImage, dstImage);
//		code = decode(dstImage, ofv);
//	}
//	catch (...) {
//
//	}
//	return code;
//}
//
//void readVideo(string videopath, vector<Mat>& srcImages) {
//	//读取视频,参数为0则打开摄像头
//	VideoCapture capture(videopath);
//	Mat frame;
//	while (true) {
//		//一帧一帧读
//		capture >> frame;
//		//imshow("img", frame);
//		//waitKey();
//		if (frame.empty()) {
//			break;
//		}
//		else {
//			resize(frame, frame, Size(720, 720), 0, 0, INTER_NEAREST);//调整大小
//			srcImages.push_back(frame.clone());
//			frame.release();
//		}
//	}
//}
//
//
//
//void Encode_crc(string code, int lastimg_flag) {
//	string CRC = "10000010011000001000111011011011";
//	code.append(CalculateCRC(code, CRC));
//	Draw(code, lastimg_flag);
//}
//
//int main()
//{
//	int choice = 1;
//	cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
//	cin >> choice;
//	while (choice != 1 && choice != 2) {
//		cout << "您选择的功能序号无效，请重新选择" << endl;
//		cout << "请输入要选择的功能序号：1.信息编码   2.视频解码" << endl;
//		cin >> choice;
//	}
//	cout << endl;
//	if (choice == 1) {
//		char name1[20];
//		cout << "请输入需要转换的信息所存放文件名，例如“e1.bin”:" << endl;
//		cin >> name1;
//
//		//信息读入部分
//		vector<int>out;
//		string inString;
//		ifstream fs(name1, ios::binary);
//		stringstream ss;
//		ss << fs.rdbuf();
//		inString = ss.str();
//		int length_char, length_bit;
//		length_char = inString.size();//求字符串长度
//		length_bit = length_char * 8;
//
//		//信息到二进制-编码部分
//		int img_count;
//		int LEN = 8;
//		img_count = length_bit / 9184;//判断要做几张图
//		if (length_bit % 9184 != 0) img_count++;
//		//cout << img_count << endl;
//		//cout << length_char << endl;
//		string data;
//		int temporary;
//		inString += "\0";
//		if (length_char > 92000)length_char = 92000;
//		if (img_count > 80)img_count = 81;
//		for (int i = 0; i < length_char; i++) {
//			Encode((unsigned char)inString[i]);//编码函数，设置全局变量 encode[LEN]来传递
//			for (int j = 0; j < LEN; j++)
//			{
//				temporary = encode[j];
//				if (temporary == 1) data.append(1, '1');
//				if (temporary == 0) data.append(1, '0');
//			}
//		}
//		data += "\0";
//		//cout << "Finish encode" << endl;
//		//cout << data.length() << endl;
//		for (int i = 0; i < img_count - 1; i++) {
//			Encode_crc(data.substr(i * 1148 * 4 * 2, 1148 * 4 * 2), 0);//第二个参数为0表示不是最后一张图
//		}
//		Encode_crc(data.substr((img_count - 1) * 1148 * 4 * 2), 1);
//		//cout << "Finish CRC" << endl;
//		//图片转视频
//		Video();
//	}
//	else {
//		vector<Mat> srcImages;
//		char videoname[20], outname[20];
//		cout << "请输入要解码的视频名，例如“1.mp4”" << endl;
//		cin >> videoname;
//		cout << "请输入解码后存放信息的文件名，例如“1.bin”" << endl;
//		cin >> outname;
//		cout << "请输入存放检验信息的文件名，例如“v1.bin”" << endl;
//		cin >> v_name;
//		string videopath(videoname);
//		string outfile(outname);
//		ofstream out(outfile, ios::binary);
//		ofstream ofv(v_name, ios::binary);
//
//		readVideo(videopath, srcImages);
//		string codes;
//		vector<string> sv;
//		string text;
//
//		int i = 0;
//		for (i = 0; i < srcImages.size(); i++) {
//			string code;
//			//cout << i << endl;
//			code = code_recognition(srcImages[i], ofv);
//			if (code.empty())
//				continue;
//			else {
//				sv.push_back(code);
//				break;
//			}
//		}
//		i = i + 3;
//		for (; i < srcImages.size(); i += 3) {
//			string code;
//			code = code_recognition(srcImages[i], ofv);
//			if (code.empty())
//				continue;
//			else sv.push_back(code);
//		}
//
//		for (int i = 0; i < sv.size(); i++) {
//			codes.append(sv[i]);
//		}
//		codes.append(string(32, '0'));
//		//cout << codes<<endl;
//		text = codeToText(codes);
//		//cout << text;
//		out << text;
//		system("pause");
//		return 0;
//	}
//}