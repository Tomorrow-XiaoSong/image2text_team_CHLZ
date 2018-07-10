#include<iostream>
#include<opencv2\opencv.hpp>
#include<string>
#include<fstream>

using namespace std;
using namespace cv;

void RGB_to_char_image(string input_image_address, string output_image_address)
{												//!!!输入 读、存图片的地址（此处可以在界面优化，例如戳C盘->戳desktop->戳图片）
	//读取图片					
	Mat in_image, huidu_image;								//创建Mat图像类的对象,in_image记录读入的图片，huidu_image记录转化后的灰度图片
	in_image = imread(input_image_address, IMREAD_UNCHANGED);				//打开方式为：IMREAD_UNCHANGED,8位，彩色或非彩色
	if (in_image.empty()) {									//打开错误时发送错误信息
		cerr << "Error! The input image can't be read...\n";
	}

	//彩图 转 灰度图片
	cvtColor(in_image, huidu_image, COLOR_BGR2GRAY);		 			//直接调用cvtColor函数，通过第三个参数设置转换方式为彩图转灰度图片

	//灰度图片 转 灰度矩阵
	int nl = huidu_image.rows;								//行数  
	int nc = huidu_image.cols * huidu_image.channels();		 			//列数 = 像素点列数*通道数（此处为灰度图片只有一个gray通道。彩色图片有RGB三个通道）
	int* huidu_matrix = new int[nl*nc + 1];					 		//定义灰度矩阵，用一维数组表示二维数组，便于后边操作
	for (int j = 0; j<nl; j++) {
		uchar* data = huidu_image.ptr<uchar>(j);			 		//data记录图片每行像素的起始地址,ptr<uchar>(j)获得第j行的头指针，<uchar>是图片中的数据类型
		for (int i = 0; i<nc; i++) {							//将该行地址下的所有像素记录到灰度矩阵的对应行中
			huidu_matrix[j*nc + i] = data[i];
		}
	}

	//huidu_matrix 转 字符画
	char huidu_char[8] = "@#xo+- ";							 	//灰度字符													
	char* char_image = new char[nl*nc + 1];					 		//字符画矩阵
	for (int j = 0; j < nl; j++) {							 	//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < nc; i++) {						 	//0-35:@   36-71:#   72-108:x   109-144:o   145-180:+   181-216:-   217-255: 
			if (huidu_matrix[j*nc + i] <= 35)char_image[j*nc + i] = huidu_char[0];
			else if (huidu_matrix[j*nc + i] <= 71)char_image[j*nc + i] = huidu_char[1];
			else if (huidu_matrix[j*nc + i] <= 108)char_image[j*nc + i] = huidu_char[2];
			else if (huidu_matrix[j*nc + i] <= 144)char_image[j*nc + i] = huidu_char[3];
			else if (huidu_matrix[j*nc + i] <= 180)char_image[j*nc + i] = huidu_char[4];
			else if (huidu_matrix[j*nc + i] <= 216)char_image[j*nc + i] = huidu_char[5];
			else char_image[j*nc + i] = huidu_char[6];
		}
	}

	//将生成的字符画图片储存
	ofstream f(output_image_address, ios::out);
	for (int j = 0; j < nl; j++) {
		for (int i = 0; i < nc; i++) {
			f << char_image[j*nc + i];
			f << char_image[j*nc + i];
		}
		f << '\n';
	}
	f.close();

	//释放内存
	delete[] huidu_matrix;
	delete[] char_image;
}
