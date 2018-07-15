/**************************************************功能调用指南**************************************************************/
//	类Image2Text对用户开发的功能函数共有两个：
//	1、根据图片及用户要求生成.txt文件
//	（1）函数原型：void to_txt(string output_address, int type_of_output, int width = NONINPUT, int height = NONINPUT);
//	（2）参数说明：
//	output_address：控制输出图片地址
//	type_of_output：控制输出模式
//		（1）CHAR_TXT:黑白字符画
//		（2）REVERSAL_CHAR_TXT:黑白反转输出的字符画
//		（3）NONDESTRUCTIVE_TXT：高清晰度黑白字符画
//		（4）REVERSAL_NONDESTRUVTIVE_TXT:高清晰度黑白反转字符画
//	width：输出图片的width，带缺省值
//	height：输出图片的height，带缺省值
//	注：用户可自定义width与height，也可不提供。不提供时，会根据输入图片比例输出较佳的大小。
//	2、根据图片及用户要求生成.html文件
//	（1）函数原型：void to_html(string output_address, int type_of_output, int width = NONINPUT, int height = NONINPUT);
//	（2）参数说明：
//	output_address：控制输出图片地址
//	type_of_output：控制输出模式
//		（1）CHAR_HTML:黑白字符画
//		（2）REVISE_CHAR_HTML:黑白反转输出的字符画
//		（3）NONDESTRUCTIVE_HTML:高清晰度黑白字符画
//		（4）REVERSAL_NINDESTRUCTIVE_HTML:生成.html格式的高清晰度黑白反转字符画
//		（5）COLOR_HTML:彩色字符画
//		（6）NONDESTRUCTIVE_HTML:高清晰度彩色字符画
//	width：输出图片的width，带缺省值
//	height：输出图片的height，带缺省值
//	注：用户可自定义width与height，也可不提供。不提供时，会根据输入图片比例输出较佳的大小
/***************************************************************************************************************************/

#ifndef IMAGE2TEXT_H
#define IMAGE2REXT_H

#include<iostream>
#include<string>
#include<fstream>
#include<opencv2\opencv.hpp>

//宏定义，作缺省值
#define NONINPUT -1314						//用户无输入的缺省值，用于判断
#define CHARIMAGE_WIDTH 70
#define CHARIMAGE_HEIGHT 70
#define NONDESTRUCTIVE_WIDTH 500
#define NONDESTRUCTIVE_HEIGHT 500
#define CHARHTML_WIDTH 100
#define CHARHTML_HEIGHT 100
#define NONDESTRUCTIVE_HTML_WIDTH 1200
#define NONDESTRUCTIVE_HTML_HEIGHT 1200
#define COLORHTML_WIDTH 70
#define COLORHTML_HEIGHT 70
#define NONDESTRUCTIVE_COLORHTML_WIDTH 215
#define NONDESTRUCTIVE_COLORHTML_HEIGHT 215

enum outputMode { NONDESTRUCTIVE_TXT, CHAR_TXT, CHAR_HTML, COLOR_HTML, NONDESTRUCTIVE_HTML, REVERSAL_CHAR_TXT, REVERSAL_NONDESTRUCTIVE_TXT, REVERSAL_CHAR_HTML,REVERSAL_NONDESTRUCTIVE_HTML ,NONDESTRUCTIVE_COLOR_HTML};
								//用户输出模式 枚举
enum format { HTML, TXT };					//用户输出文件格式控制，枚举

using namespace std;
using namespace cv;

class Image2Text
{
private:
	struct huiduMatrix {					//灰度矩阵结构体，包含灰度矩阵指针，矩阵宽度，矩阵高度
		int * matrix;
		int width;
		int height;
	} huidu_matrix;						//实例化对象：huidu_matrix
	typedef struct colorPx {				//彩色像素结构体，每个对象有RGB三种参数
		int Red;
		int Green;
		int Blue;
	};
	struct colorMatrix {					//彩色矩阵结构体
		colorPx* matrix;				//彩色像素结构体矩阵指针
		int width;					//记录该结构体矩阵的宽度
		int height;					//记录该结构体矩阵的高度
	}color_matrix;						//实例化对象
	Mat input_image;					//记录输入的图片
	char* char_image;					//字符画数组
private:
	void control_output_format(string& outputAddress, int outputFormat);
								//判断并控制输出为.txt 或者.html
	
	huiduMatrix RGB_to_huiduMatrix(Mat RGB, int type, int width, int height);
								//width:输出矩阵的宽度，height:输出矩阵的高度,缺省值为NONDESTRUCTIVE_WIDTH NONDESTRUCTIVE_HEIGHT
								//可以不给长宽参数，默认为NONDESTRUCTIVE_WIDTH和NONDESTRUCTIVE_HEIGHT
								//可以只给一个参数：width，height会设置为NONDESTRUCTIVE_HEIGHT
								//但注意不能只给height
	
	colorMatrix RGB_to_colorMatrix(Mat RGB,int type, int width = NONINPUT, int height = NONINPUT);
								//返回值为彩色图片矩阵的结构体，该结构体内有：彩色像素结构体矩阵指针，彩色像素结构体矩阵的长、高

	char* huiduMatrix_to_charTXT(const huiduMatrix &huiduMatrix, int type = CHAR_TXT);
								//灰度矩阵 转 字符画，返回值可以做为.txt文件内容
								//type可选择正常输出或者反转输出
	
	char* huiduMatrix_to_charHtml(const huiduMatrix &huiduMatrix,int type = CHAR_HTML);
								//灰度矩阵 转 字符画，返回值可以做为.html文件内容
	
	char* huiduMatrix_to_colorCharHtml(const huiduMatrix &huiduMatrix);
								//彩图的灰度矩阵 转 字符画，返回值可以做为.html文件内容
	
	char* huiduMatrix_to_nondestructiveTXT(const huiduMatrix &huidu_matrix,int type = NONDESTRUCTIVE_TXT);
								//灰度矩阵 转 无损画，返回值作为.txt 的文件内容
	
	char* huiduMatrix_to_nondestructiveHtml(const huiduMatrix &huiduMatrix,int type = NONDESTRUCTIVE_HTML);
								//灰度矩阵 转 无损画，返回值.html 的文件内容
	
public:
	Image2Text(string input_image_address);
	~Image2Text();
	
	void to_txt(string output_address,int type_of_outputImage,int width = NONINPUT,int height = NONINPUT);
								//根据用户选择输出.txt，可以是无损，也可以是字符画

	void to_html(string output_address, int type_of_outout, int width = NONINPUT, int height = NONINPUT);
								//输出.html
};


#endif
