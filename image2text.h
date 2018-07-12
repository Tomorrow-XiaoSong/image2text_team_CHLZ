/*************************************用户调用指南*****************************************/
/***   1、用户定义Image2Text类的一个对象的同时要给对象提供构造参数：输入图片的地址      ***/
/***   	例：Image2Text test("C:\\picture.jpg"）                                         ***/
/***   2、根据您想要的输出模式：.txt 或者 .html 选择对应的函数                          ***/
/***    例：test.to_txt( v1, v2, v3 = ,v4 = )可以输出.txt文件                           ***/
/***        第一个参数: 输出地址                                                        ***/
/***        第二个参数: 想输出的类型：NONDESTRUCTIVEIMAGE无损图片                       ***/
/***                                  CHARIMAGE字符画                                   ***/
/***        当选择字符画输出类型的时候，可以选择提供输出的字符画的宽度v3和高度v4        ***/
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

#define NONDESTRUCTIVE_WIDTH 500
#define NONDESTRUCTIVE_HEIGHT 500
#define CHARIMAGE_WIDTH 70
#define CHARIMAGE_HEIGHT 70
enum pictureQuality { NONDESTRUCTIVEIMAGE, CHARIMAGE };
											//枚举

using namespace std;
using namespace cv;

class Image2Text
{
private:
	struct huiduMatrix {					//灰度矩阵结构体，包含灰度矩阵指针，矩阵宽度，矩阵高度
		int * matrix;
		int width;
		int height;
	} huidu_matrix;							//实例化对象：huidu_matrix
	Mat input_image;						//记录输入的图片
	char* char_image;						//字符画数组

	huiduMatrix RGB_to_huiduMatrix(Mat RBG,int width = NONDESTRUCTIVE_WIDTH,int height = NONDESTRUCTIVE_HEIGHT);
											//width:输出矩阵的宽度，height:输出矩阵的高度,参省值为NONDESTRUCTIVE_WIDTH NONDESTRUCTIVE_HEIGHT
											//可以不给长宽参数，默认为NONDESTRUCTIVE_WIDTH和NONDESTRUCTIVE_HEIGHT\
											  可以只给一个参数：width，height会设置为NONDESTRUCTIVE_HEIGHT\
											但注意不能只给height

	char* huiduMatrix_to_charImage(const huiduMatrix &huiduMatrix);
											//灰度矩阵 转 字符画，返回值可以做完.txt 或者 .html的文件内容

	char* huiduMatrix_to_nondestructiveImage(const huiduMatrix &huidu_matrix);
											//灰度矩阵 转 无损画，返回值仅能作为.txt 的文件内容\
											因为html的最小字符太大，不适合做无损
	
public:
	Image2Text(string input_image_address);	//构造函数
	~Image2Text();							//析构函数
	void to_txt(string output_address,int type_of_outputImage,int width = NONDESTRUCTIVE_WIDTH,int height = NONDESTRUCTIVE_HEIGHT);
											//根据用户选择输出.txt，可以是无损，也可以是字符画

	void to_html(string output_address);	//输出.html

};


#endif
