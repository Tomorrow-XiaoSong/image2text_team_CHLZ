/*************************************用户调用指南******************************************/
/***   1、用户定义Image2Text类的一个对象的同时要给对象提供构造参数：输入图片的地址            ***/
/***   	例：Image2Text test("C:\\picture.jpg"）                                          ***/
/***   2、根据您想要的输出模式：.txt 或者 .html 选择对应的函数                              ***/
/***    例：test.to_txt( v1, v2, v3 = ,v4 = )可以输出.txt文件                             ***/
/***        第一个参数: 输出地址                                                          ***/
/***        第二个参数: 想输出的类型：NONDESTRUCTIVEIMAGE无损图片                          ***/
/***                                  CHARIMAGE字符画                                    ***/
/***        当选择字符画输出类型的时候，可以选择提供输出的字符画的宽度v3和高度v4              ***/
/***                                                                                    ***/
/***        test.to_html()                                                              ***/
/***                                                                                    ***/
/***                                                                                    ***/
/******************************************************************************************/

#ifndef IMAGE2TEXT_H
#define IMAGE2REXT_H

#include<iostream>
#include<string>
#include<fstream>
#include<opencv2\opencv.hpp>

//宏定义，作缺省值
#define NONINPUT -1314						//用户无输入的缺省值，用于判断
#define NONDESTRUCTIVE_WIDTH 500
#define NONDESTRUCTIVE_HEIGHT 500
#define CHARIMAGE_WIDTH 70
#define CHARIMAGE_HEIGHT 70
#define CHARHTML_WIDTH 100
#define CHARHTML_HEIGHT 100
#define COLORHTML_WIDTH 260
#define COLORHTML_HEIGHT 260
enum pictureQuality { NONDESTRUCTIVE_TXT, CHAR_TXT,COLOR_HTML,CHAR_HTML };
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
	
	colorMatrix RGB_to_colorMatrix(Mat RGB, int width = NONINPUT, int height = NONINPUT);
								//返回值为彩色图片矩阵的结构体，该结构体内有：彩色像素结构体矩阵指针，彩色像素结构体矩阵的长、高

	char* huiduMatrix_to_charImage(const huiduMatrix &huiduMatrix);
								//灰度矩阵 转 字符画，返回值可以做为.txt文件内容

	char* huiduMatrix_to_charHtml(const huiduMatrix &huiduMatrix);
								//灰度矩阵 转 字符画，返回值可以做为.html文件内容
	
	char* huiduMatrix_to_colorCharHtml(const huiduMatrix &huiduMatrix);
								//彩图的灰度矩阵 转 字符画，返回值可以做为.html文件内容
	
	char* huiduMatrix_to_nondestructiveImage(const huiduMatrix &huidu_matrix);
								//灰度矩阵 转 无损画，返回值仅能作为.txt 的文件内容
	
public:
	Image2Text(string input_image_address);
	~Image2Text();
	void to_txt(string output_address,int type_of_outputImage,int width = NONINPUT,int height = NONINPUT);
								//根据用户选择输出.txt，可以是无损，也可以是字符画

	void to_html(string output_address,int width = NONINPUT, int height = NONINPUT);
								//输出.html

	//字符反转函数——未完成
};


#endif
